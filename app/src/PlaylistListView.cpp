#include "PlaylistListView.h"
#include "MenuPlaylistItemView.h"
#include "ImportPlaylistView.h"

PlaylistListView::PlaylistListView(QWidget *parent) : QWidget(parent), ui(new Ui::PlaylistsView) {
    ui->setupUi(this);
    LoadDefaultPlaylist();

    QMenu *addmenu = new QMenu();

    addmenu->addAction("Create");
    addmenu->addAction("Import m3u");
    connect(addmenu->actions().at(0), &QAction::triggered, this, &PlaylistListView::CreatePlaylistView);
    connect(addmenu->actions().at(1), &QAction::triggered, this, &PlaylistListView::CreateImportPlaylistView);

    ui->add->setMenu(addmenu);
}

void PlaylistListView::CreateNewPlaylist(QString playlistName) {
    if (ui->list->findChild<MenuPlaylistItemView *>(playlistName) != Q_NULLPTR) {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Playlist already exists");
        msg->exec();
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
    emit Playlists(ui->list->findChildren<MenuPlaylistItemView *>());
    emit PlaylistCreated(playlistName);
}

void PlaylistListView::LoadDefaultPlaylist() {
    CreateNewPlaylist(QString("Default"));
}

void PlaylistListView::AddSongToPlaylist(const QString &playlistName, FileTags *song) {
    ui->list->findChild<MenuPlaylistItemView *>(playlistName)->AddSong(song);
}

void PlaylistListView::RemoveSongFromPlaylist(const QString &playlistName, FileTags *song) {
    ui->list->findChild<MenuPlaylistItemView *>(playlistName)->RemoveSong(song);
}

void PlaylistListView::RenamePlaylist(const QString& old, QString newName) {
    if (ui->list->findChild<MenuPlaylistItemView *>(newName) == Q_NULLPTR)
        ui->list->findChild<MenuPlaylistItemView *>(old)->Rename(std::move(newName));
    else {
        QErrorMessage *msg = new QErrorMessage(this);
        msg->showMessage("Playlist with that name already exists!");
    }
}

void PlaylistListView::DeletePlaylist(const QString& playlistName) {
    delete ui->list->findChild<MenuPlaylistItemView *>(playlistName)->ParentItem();
    emit Playlists(ui->list->findChildren<MenuPlaylistItemView *>());
    emit PlaylistDeleted(playlistName);
}

void PlaylistListView::CreatePlaylistView() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Create New Playlist"),
                                         tr("Playlist Name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !text.isEmpty())
        CreateNewPlaylist(text);
    else {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Playlist must have a name!");
        msg->exec();
    }
}

void PlaylistListView::CreateImportPlaylistView() {
    auto dialog = new ImportPlaylistView(this);
    dialog->exec();
}
