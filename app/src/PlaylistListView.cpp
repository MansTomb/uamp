#include "PlaylistListView.h"
#include "MenuPlaylistItemView.h"
#include "ImportPlaylistView.h"

PlaylistListView::PlaylistListView(QWidget *parent) : QWidget(parent), ui(new Ui::PlaylistsView) {
    ui->setupUi(this);
    QMenu *addmenu = new QMenu();

    addmenu->addAction("Create");
    addmenu->addAction("Import m3u");
    connect(addmenu->actions().at(0), &QAction::triggered, this, &PlaylistListView::CreatePlaylistView);
    connect(addmenu->actions().at(1), &QAction::triggered, this, &PlaylistListView::CreateImportPlaylistView);

    ui->add->setMenu(addmenu);
    LoadPlaylists();
}

void PlaylistListView::CreateNewPlaylist(QString playlistName) {
    if (ui->list->findChild<MenuPlaylistItemView *>(playlistName) != Q_NULLPTR) {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Playlist already exists");
        msg->exec();
        delete msg;
        return;
    }

    auto *item = new QListWidgetItem;
    auto playlist = new MenuPlaylistItemView(playlistName, item, this);
    SetupPlaylist(playlistName, item, playlist);
}

bool PlaylistListView::ImportNewPlaylist(QString filepath, QString playlistName) {
    if (ui->list->findChild<MenuPlaylistItemView *>(playlistName) != Q_NULLPTR)
        return false;

    auto *item = new QListWidgetItem;
    auto playlistMedia = new QMediaPlaylist();
    playlistMedia->load(QUrl::fromLocalFile(filepath));

    auto playlist = new MenuPlaylistItemView(playlistMedia, playlistName, item, this);

    SetupPlaylist(playlistName, item, playlist);
    return true;
}

void PlaylistListView::ThrowPlaylists() {
    auto list = ui->list->findChildren<MenuPlaylistItemView *>();
    QStringList slist;

    for (const auto &item : list)
        if (item->NotGonnaBeDeleted())
            slist.insert(slist.size(), item->PlaylistName());
    emit Playlists(slist);
}

void PlaylistListView::SetupPlaylist(const QString &playlistName,
                                     QListWidgetItem *item,
                                     MenuPlaylistItemView *playlist) {
    ui->list->addItem(item);
    item->setSizeHint(QSize(50, 75));
    ui->list->setItemWidget(item, playlist);

    connect(playlist, &MenuPlaylistItemView::PlaylistRenamed, this, &PlaylistListView::PlaylistRenamed);
    connect(playlist, &MenuPlaylistItemView::SongAdded, this, &PlaylistListView::SongAddedToPlaylist);
    connect(playlist, &MenuPlaylistItemView::SongRemoved, this, &PlaylistListView::SongRemovedFromPlaylist);
    connect(playlist, &MenuPlaylistItemView::DeletePlaylist, this, &PlaylistListView::DeletePlaylist);
    connect(playlist, &MenuPlaylistItemView::PlaylistChoosed, this, &PlaylistListView::PlaylistChoosed);
    connect(playlist, &MenuPlaylistItemView::PlaylistChoosed, this, [playlist, this] {emit PlaylistChoosedName(playlist->PlaylistName());});
    ThrowPlaylists();
    emit PlaylistCreated(playlistName);
}

void PlaylistListView::LoadPlaylists() {
    qDebug() << "Load playlists from db!";
    auto slist = SqlDatabase::instance().getAllPlaylist();
    for (const auto &playlist : slist) {
        qDebug() << "Playlist: " + playlist;
        CreateNewPlaylist(playlist);
    }
    emit Playlists(slist);
}

void PlaylistListView::AddSongToPlaylist(const QString &playlistName, FileTags *song) {
    qDebug() << playlistName << ": added song - " << song->tags.title;
    ui->list->findChild<MenuPlaylistItemView *>(playlistName)->AddSong(song);
}

void PlaylistListView::RemoveSongFromPlaylist(const QString &playlistName, FileTags *song) {
    auto i = 0;
    for (auto &item : ui->list->findChildren<MenuPlaylistItemView *>())
        if (item->ContainsSong(song))
            i++;
    qDebug() << playlistName << " " << song->tags.title;
    if (i != 1)
        ui->list->findChild<MenuPlaylistItemView *>(playlistName)->RemoveSong(song);
    else {
        qDebug() << "Unsafe function usage PlaylistListView::RemoveSongFromPlaylist, message for debug. DO NOT DELETE THIS MESSAGE BEFORE END!";
        ui->list->findChild<MenuPlaylistItemView *>(playlistName)->RemoveSong(song);
        delete song;
    }
}

void PlaylistListView::RenamePlaylist(const QString& old, QString newName) {
    if (ui->list->findChild<MenuPlaylistItemView *>(newName) == Q_NULLPTR) {
        ui->list->findChild<MenuPlaylistItemView *>(old)->Rename(std::move(newName));
    } else {
        auto *msg = new QErrorMessage(this);
        msg->showMessage("Playlist with that name already exists!");
        msg->exec();
        delete msg;
    }
}

void PlaylistListView::DeletePlaylist(const QString& playlistName) {
    delete ui->list->findChild<MenuPlaylistItemView *>(playlistName)->ParentItemForDelete();
    SqlDatabase::instance().deletePlaylist(playlistName);
    SqlDatabase::instance().clearPlaylist(playlistName);
    ThrowPlaylists();
    emit PlaylistDeleted(playlistName, ui->list->findChild<MenuPlaylistItemView *>("Default"));
}

void PlaylistListView::CreatePlaylistView() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Create New Playlist"),
                                         tr("Playlist Name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !text.isEmpty()) {
        if (ui->list->findChild<MenuPlaylistItemView *>(text) != Q_NULLPTR) {
            auto msg = new QErrorMessage(this);
            msg->showMessage("Playlist already exists");
            msg->exec();
            delete msg;
            return;
        }
        CreateNewPlaylist(text);
        SqlDatabase::instance().addNewPlaylist(text, QPixmap());//ui->list->findChild<MenuPlaylistItemView *>(text)->getPicture()
    } else {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Playlist must have a name!");
        msg->exec();
        delete msg;
    }
}

void PlaylistListView::CreateImportPlaylistView() {
    auto dialog = new ImportPlaylistView(this);
    dialog->exec();
    delete dialog;
}
