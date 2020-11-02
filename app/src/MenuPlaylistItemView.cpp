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
    return QList<FileTags *>();
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
    menu->addAction("Export");
    menu->addAction("Delete");
    auto actionsList = menu->actions();
    connect(actionsList.at(0), &QAction::triggered, this, &MenuPlaylistItemView::actionLoad);
    connect(actionsList.at(1), &QAction::triggered, this, &MenuPlaylistItemView::actionRename);
    connect(actionsList.at(2), &QAction::triggered, this, &MenuPlaylistItemView::actionExport);
    connect(actionsList.at(3), &QAction::triggered, this, &MenuPlaylistItemView::actionDelete);
    ui->menu->setMenu(menu);
}

void MenuPlaylistItemView::AddSongsFromPlaylistMedia(const QMediaPlaylist *playlist) {
    for (int i = 0; i < playlist->mediaCount(); ++i) {
        auto string = playlist->media(i).request().url().toString();
        QString path = playlist->media(i).request().url().toString().remove(0, 7);
        auto filename = playlist->media(i).request().url().toString().remove(0, string.lastIndexOf("/") + 1);

        path.remove(path.lastIndexOf("/") + 1, path.size());
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
    }
}

void MenuPlaylistItemView::actionExport() {
    QMediaPlaylist playlist;
    QString filename = QFileDialog::getSaveFileName(this, "Save Playlist", "/", "Playlist (*.m3u)" );
    qDebug() << "name is : " << filename;
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
    }
}
