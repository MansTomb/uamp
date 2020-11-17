#include "MenuPlaylistItemView.h"
#include "PlaylistListView.h"

MenuPlaylistItemView::MenuPlaylistItemView(QString playlistName,
                                           QListWidgetItem *wItem,
                                           QWidget *parent) :
                                           MenuPlaylistItemView(Q_NULLPTR, playlistName, wItem, parent) {}

MenuPlaylistItemView::~MenuPlaylistItemView() {
    delete ui;
}

QList<FileTags *> MenuPlaylistItemView::loadPlaylist() {
    const QList<FileTags *> &lst = SqlDatabase::instance().getAllTracksFromPlaylist(m_name);
    qDebug() << lst.count();
    return lst;
}

MenuPlaylistItemView::MenuPlaylistItemView(QMediaPlaylist *playlist,
                                           QString playlistName,
                                           QListWidgetItem *wItem,
                                           QWidget *parent) :
        m_name(playlistName),
        m_parent(wItem),
        QWidget(parent),
        ui(new Ui::MenuPlaylistItemView) {
    ui->setupUi(this);
    m_playlist = loadPlaylist();

    setObjectName(m_name);
    ui->playListname->setText(m_name);

    if (playlist)
        AddSongsFromPlaylistMedia(playlist);

    auto menu = new QMenu();
    menu->addAction("Load");
    menu->addAction("Rename");
    menu->addAction("Change image");
    menu->addAction("Export");
    menu->addAction("Delete");
    auto actionsList = menu->actions();
    connect(actionsList.at(0), &QAction::triggered, this, &MenuPlaylistItemView::actionLoad);
    connect(actionsList.at(1), &QAction::triggered, this, &MenuPlaylistItemView::actionRename);
    connect(actionsList.at(2), &QAction::triggered, this, &MenuPlaylistItemView::actionChangeImage);
    connect(actionsList.at(3), &QAction::triggered, this, &MenuPlaylistItemView::actionExport);
    connect(actionsList.at(4), &QAction::triggered, this, &MenuPlaylistItemView::actionDelete);
    ui->menu->setMenu(menu);
    delete playlist;
}

void MenuPlaylistItemView::AddSongsFromPlaylistMedia(const QMediaPlaylist *playlist) {
    for (int i = 0; i < playlist->mediaCount(); ++i) {
        bool has_in_list = false;
        auto string = playlist->media(i).request().url().toString();
        QString path = playlist->media(i).request().url().toString().remove(0, 7);
        auto filename = playlist->media(i).request().url().toString().remove(0, string.lastIndexOf("/") + 1);

        path.remove(path.lastIndexOf("/") + 1, path.size());
        for (const auto &song : m_playlist)
            if (song->tags.path == path + filename)
                has_in_list = true;
        if (!has_in_list)
            AddSong(new FileTags(path.toStdString(), filename.toStdString()));
    }
}

void MenuPlaylistItemView::actionLoad() {
    emit PlaylistChoosed(this);
}

void MenuPlaylistItemView::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);

    emit PlaylistChoosed(this);
}

void MenuPlaylistItemView::actionRename() {
    if (m_name == "Default") {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Default playlist cant be renamed");
        msg->exec();
        delete msg;
        return;
    }
    bool ok;
    QString text = QInputDialog::getText(this, tr("Rename Playlist"),
                                         tr("Playlist Name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !text.isEmpty())
        Rename(text);
    else {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Name cant be empty");
        msg->exec();
        delete msg;
    }
}

void MenuPlaylistItemView::actionExport() {
    QMediaPlaylist playlist;
    QString filename = QFileDialog::getSaveFileName(this, "Save Playlist", "/", "Playlist (*.m3u)" );

    for (const auto &song : m_playlist)
        playlist.addMedia(QUrl::fromLocalFile(song->tags.path));
    playlist.save(QUrl::fromLocalFile(filename), "m3u");
}

void MenuPlaylistItemView::actionDelete() {
    if (m_name != "Default")
        emit DeletePlaylist(m_name);
    else {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Default playlist cant be deleted");
        msg->exec();
        delete msg;
    }
}

void MenuPlaylistItemView::actionChangeImage() {
    QPixmap icon = QPixmap(QFileDialog::getOpenFileName(this,tr("Image"), "/", tr("Image (*.png *.jpg *.jpeg)")));
    if (icon.isNull() == false)
        ui->icon->setPixmap(icon);
}

void MenuPlaylistItemView::Rename(QString newName) {
    emit PlaylistRenamed(m_name, newName);
    SqlDatabase::instance().renamePlaylist(m_name, newName);
    m_name = newName;
    ui->playListname->setText(m_name);
    setObjectName(m_name);
}
