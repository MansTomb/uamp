#include <QErrorMessage>
#include "PlaylistView.h"


#include "bass.h"

PlaylistView::PlaylistView(QWidget *parent) : QListWidget(parent), m_playlistName("Default") {
}

void PlaylistView::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}

void PlaylistView::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void PlaylistView::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

/*

 Сделать проверку на дубль файла в плейлисте !!!

*/

void PlaylistView::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls())
        return;
    auto droppedData = event->mimeData()->urls();
    if (droppedData.count() == 1) {
        QFileInfo fileInfo(droppedData.at(0).toString().remove(0, 7));
        if (fileInfo.isDir()) {
            addSongsDir(droppedData);
        } else if(fileInfo.isFile()) {
            addSong(fileInfo);
        }
    } else {
        addLotOfSongs(droppedData);
    }
}

void PlaylistView::addWidget(const QString &pathTrack, const QString &trackName) {
    QListWidgetItem *item = new QListWidgetItem;
    addItem(item);
    item->setSizeHint(QSize(50,75));
    auto *widget = new PlayerlistItem(item, pathTrack, trackName);
    m_widgets.push_back(widget);
    setItemWidget(item, widget);
    connect(widget, &PlayerlistItem::CurrentSong, this, &PlaylistView::CurrentSongChanged);
    connect(widget, &PlayerlistItem::SetImage, this, &PlaylistView::SetImage);
    connect(widget, &PlayerlistItem::RemoveTrackFromPlaylist, this, &PlaylistView::RemoveTrackFromPlaylistSlot);
    connect(widget, &PlayerlistItem::AddTracktoPlaylist, this, &PlaylistView::AddTracktoPlaylistSlot);
    connect(this, &PlaylistView::SetPlaylists, widget, &PlayerlistItem::updateListPlaylist);
    connect(this, &PlaylistView::ThrowPlaylistName, widget, &PlayerlistItem::setPlaylistName);
    emit FileAdded(m_playlistName, widget->song());
}

void PlaylistView::addSong(const QFileInfo &fileInfo) {

    //чекнуть на формат файла!!

    if (checkPerm(fileInfo)) {
        addWidget(fileInfo.absolutePath() + "/", fileInfo.fileName());
    }
}

void PlaylistView::addSongsDir(const QList<QUrl> &droppedData) {
    QDir dir(droppedData.at(0).toString().remove(0, 7));
    dir.setNameFilters(QStringList() << "*.mp3" << "*.flac" << "*.mp4"
                                     << "*.wav" << "*.m4a" << "*.m3u"
                                     << "*.ogg");
    auto dirPath(dir.absolutePath() + "/");
    if (!dir.exists())
        return;
//            clear();
    for (auto &item : dir.entryList()) {
//                if (!m_allFiles.contains(dirPath + item)) {
//                    m_allFiles << dirPath + item;
        addWidget(dirPath, item);
//                }
    }
}

bool PlaylistView::checkPerm(const QFileInfo &fileInfo) {
    return fileInfo.isReadable() || fileInfo.isWritable() || (fileInfo.isReadable() ^ fileInfo.isWritable());
}

void PlaylistView::addLotOfSongs(const QList<QUrl> &droppedData) {

    //чекнуть на формат файла!!

    for (const auto &item : droppedData) {
        QFileInfo fileInfo(item.toString().remove(0, 7));
        if (fileInfo.isFile() && checkPerm(fileInfo))
            addWidget(fileInfo.absolutePath() + "/", fileInfo.fileName());
    }
}

void PlaylistView::RemoveTrackFromPlaylistSlot(FileTags *file, const QString &objectName) {
    qDebug() << "\n\n"  << objectName << " deleted";
    emit RemoveTrackFromPlaylist(m_playlistName, file);
    auto item = findChild<PlayerlistItem *>(objectName);
    auto parentItem = item->getParentToDelete();
    delete item;
    delete parentItem;
    qDebug() << "==============================\n\n";
}

void PlaylistView::PlaylistDeleted(QString deletedPlaylist,
                                   MenuPlaylistItemView *defaultPlaylist) {
    qDebug() << "\n\n"  << deletedPlaylist << " deleted";
    if (deletedPlaylist == m_playlistName) {
        //clear()
        qDebug() << "chooed";
        PlaylistChoosedSlot(defaultPlaylist);
        emit ThrowPlaylistName(m_playlistName);
    }
    qDebug() << "==============================\n\n";
}

void PlaylistView::PlaylistChoosedSlot(MenuPlaylistItemView *playlist) {
//    clearingListWidget();
    clear();
    m_playlistName = playlist->PlaylistName();
    qDebug() << "\n\n"  << m_playlistName << " selected";
    for (const auto &playlist_name : playlist->Playlist()) {
        addWidget(playlist_name);
    }
    qDebug() << "==============================\n\n";
}

void PlaylistView::clearingListWidget() {
    for(auto &item : m_widgets) {
        delete item->getParentToDelete();
        delete item;
    }
}

void PlaylistView::addWidget(FileTags *song) {
    QListWidgetItem *item = new QListWidgetItem;
    addItem(item);
    item->setSizeHint(QSize(50,75));
    auto *widget = new PlayerlistItem(item, song);
    m_widgets.push_back(widget);
    setItemWidget(item, widget);
    connect(widget, &PlayerlistItem::CurrentSong, this, &PlaylistView::CurrentSongChanged);
    connect(widget, &PlayerlistItem::SetImage, this, &PlaylistView::SetImage);
    connect(widget, &PlayerlistItem::RemoveTrackFromPlaylist, this, &PlaylistView::RemoveTrackFromPlaylistSlot);
    connect(widget, &PlayerlistItem::AddTracktoPlaylist, this, &PlaylistView::AddTracktoPlaylistSlot);
    connect(this, &PlaylistView::SetPlaylists, widget, &PlayerlistItem::updateListPlaylist);
    connect(this, &PlaylistView::ThrowPlaylistName, widget, &PlayerlistItem::setPlaylistName);
}
