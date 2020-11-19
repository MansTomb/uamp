#include <QErrorMessage>
#include "PlaylistView.h"

#include "bass.h"

PlaylistView::PlaylistView(QWidget *parent) : QListWidget(parent), m_playlistName("Default") {
    m_playlistNames = SqlDatabase::instance().getAllPlaylist();
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

void PlaylistView::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls())
        return;
    auto droppedData = event->mimeData()->urls();
    for (const auto &url : droppedData) {
        QFileInfo fInfo(url.toString().remove(0, 7));
        if (fInfo.isDir())
            AddDirWithSongs(url);
        else if (fInfo.isFile())
            AddOneSong(url);
    }
}
bool PlaylistView::CheckIfSongAlreadyAdded(const QFileInfo &fInfo) const {
    bool skip = false;
    for (const auto &widget : m_widgets)
        if (widget->song()->tags.path == fInfo.filePath())
            skip = true;
    return skip;
}

void PlaylistView::AddOneSong(const QUrl &fileUrl) {
    QFileInfo fileInfo(fileUrl.toString().remove(0, 7));
    QString format = fileInfo.fileName();
    format.remove(0, format.lastIndexOf(".") + 1);
    if (!CheckIfSongAlreadyAdded(fileInfo) && checkPerm(fileInfo) && FILE_FORMATS )
        AddSongToListAndToDB(fileInfo.absolutePath() + "/", fileInfo.fileName());
}

void PlaylistView::AddDirWithSongs(const QUrl &fileUrls) {
    QDir dir(fileUrls.toString().remove(0, 7));
    dir.setNameFilters(QStringList() << "*.mp3" << "*.flac" << "*.mp4"
                                     << "*.wav" << "*.m4a" << "*.ogg");
    auto dirPath(dir.absolutePath() + "/");
    for (auto &item : dir.entryList())
            AddOneSong("file://" + dirPath + item);
}

bool PlaylistView::checkPerm(const QFileInfo &fileInfo) {
    return fileInfo.isReadable() || fileInfo.isWritable() || (fileInfo.isReadable() ^ fileInfo.isWritable());
}

void PlaylistView::RemoveTrackFromPlaylistSlot(FileTags *file, const QString &objectName) {
    auto item = findChild<PlayerlistItem *>(objectName);
    m_widgets.removeOne(item);
    auto parentItem = item->getParentToDelete();
    auto index = indexFromItem(parentItem).row();

    delete item;
    delete parentItem;
    SqlDatabase::instance().deleteTrackFromPlaylist(file->tags.path, m_playlistName);
    emit RemoveTrackFromPlaylist(m_playlistName, file);
    if (count() > index) {
        setCurrentRow(index);
        emit CurrentSongChanged(GetWidgetByItem(currentItem())->song());
    }
}

void PlaylistView::PlaylistDeleted(QString deletedPlaylist,
                                   MenuPlaylistItemView *defaultPlaylist) {
    if (deletedPlaylist == m_playlistName) {
        clearingListWidget();
        PlaylistChoosedSlot(defaultPlaylist);
        emit ThrowPlaylistName(m_playlistName);
    }
}

void PlaylistView::PlaylistChoosedSlot(MenuPlaylistItemView *playlist) {
    clearingListWidget();
    m_playlistName = playlist->PlaylistName();
    for (const auto &song : playlist->Playlist())
        AddSongToListWithoutDB(song);
}

void PlaylistView::clearingListWidget() {
    for(auto &item : m_widgets) {
        delete item->getParentToDelete();
        delete item;
    }
    m_widgets.clear();
    clear();
}

void PlaylistView::GetLogin(QString login) {
    m_login = login;
}

void PlaylistView::NextSong() {
    m_playedSongs.push_front(currentIndex().row());
    if (m_playedSongs.count() <= m_widgets.count()) {
        if (currentRow() + 1 < count()) {
            if (!m_shuffle) {
                setCurrentRow(currentRow() + 1);
                emit CurrentSongChanged(GetWidgetByItem(currentItem())->song());
            }
            else {
                int rand = getRandom(0, count() - 1);
                for (int i = 0; m_playedSongs.contains(rand) && i < 50; i++)
                    rand = getRandom(0, count() - 1);

                setCurrentRow(rand);
                emit CurrentSongChanged(GetWidgetByItem(item(rand))->song());
            }
        }
        else if (m_repeat) {
            setCurrentRow(0);
            m_playedSongs.clear();
            emit CurrentSongChanged(GetWidgetByItem(item(0))->song());
        }
    }
    else if (m_repeat) {
        setCurrentRow(0);
        m_playedSongs.clear();
        emit CurrentSongChanged(GetWidgetByItem(item(0))->song());
    }
}

void PlaylistView::PreviousSong() {
    m_playedSongs.push_front(currentIndex().row());
    if (currentRow() -1 >= 0) {
        setCurrentRow(currentRow() - 1);
        emit CurrentSongChanged(GetWidgetByItem(currentItem())->song());
    }
}

int PlaylistView::getRandom(int from, int to) {
        std::random_device rd;
        std::mt19937_64 eng(rd());
        std::uniform_int_distribution<int> distr(from, to);
        return distr(eng);
}

void PlaylistView::AddSongToListAndToDB(const QString &pathTrack, const QString &trackName) {
    QFileInfo fileInfo(pathTrack);
    auto *item = new QListWidgetItem;
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
    emit SetPlaylists(m_playlistNames);
    emit FileAdded(m_playlistName, widget->song());
    if (m_playlistName != "Default" && !containedDefault(widget->song()->tags.path)) {
        emit FileAdded("Default", widget->song());
        SqlDatabase::instance().addInfoAboutSong(widget->song(), m_login, m_playlistName);
        SqlDatabase::instance().addInfoAboutSong(widget->song(), m_login, "Default");
    } else {
        SqlDatabase::instance().addInfoAboutSong(widget->song(), m_login, m_playlistName);
    }
}

void PlaylistView::AddSongToListWithoutDB(FileTags *song) {
    auto *item = new QListWidgetItem;
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
    emit SetPlaylists(m_playlistNames);
}

void PlaylistView::InvalidFileOnPlayer(FileTags *song) {
    auto item = findChild<PlayerlistItem *>(song->tags.path);
    auto parentItem = item->getParentToDelete();
    this->removeItemWidget(parentItem);
    delete parentItem;
    SqlDatabase::instance().deleteTrackFromPlaylist(song->tags.path, m_playlistName);
    emit RemoveTrackFromPlaylist(m_playlistName, song);
}

bool PlaylistView::containedDefault(const QString& pathToTrack) {
    m_default = SqlDatabase::instance().getAllTracksFromDefault();

    if (m_default.contains(pathToTrack)) {
        return true;
    }
    return false;
}
