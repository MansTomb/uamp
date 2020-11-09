#pragma once
//
//#include "../../taglib/taglib/tag.h"
//#include <../taglib/fileref.h>
//#include <toolkit/tpropertymap.h>
//#include <mpegfile.h>
//#include <id3v2tag.h>

#include <deque>

#include <QListWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDir>
#include <QDebug>

#include "playerlistitem.h"
#include "MenuPlaylistItemView.h"
#include "SqlDatabase.h"

class PlaylistView : public QListWidget {

    Q_OBJECT

public:
    explicit PlaylistView(QWidget *parent = Q_NULLPTR);
    void addWidget(const QString& pathTrack, const QString &trackName);
    void addWidget(FileTags *song);
    void clearingListWidget();

public slots:
    void ChangeCurrentSong(FileTags *song) {emit CurrentSongChanged(song);};
    void updatePlaylists(QStringList playlists) {emit SetPlaylists(playlists);};
    void RemoveTrackFromPlaylistSlot(FileTags *file, const QString &objectName);
    void AddTracktoPlaylistSlot(const QString& toPlaylist, FileTags *file) {emit AddTrackToPlaylist(toPlaylist, file);};
    void PlaylistDeleted(QString deletedPlaylist,
                         MenuPlaylistItemView *defaultPlaylist);
    void PlaylistChoosedSlot(MenuPlaylistItemView *playlist);
    void GetLogin(QString login);
    void setPlaylistName(QString name) {
        m_playlistName = name;
        emit ThrowPlaylistName(name);
    };


signals:
    void CurrentSongChanged(FileTags *song);
    void FileAdded(QString playlistName, FileTags *song);
    void SetImage(QPixmap *img);
    void SetPlaylists(QStringList playlists);
    void ThrowPlaylistName(QString name);
    void RemoveTrackFromPlaylist(const QString& namePlaylist, FileTags *file);
    void AddTrackToPlaylist(const QString& toPlaylist, FileTags *file);

protected:
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    bool checkPerm(const QFileInfo &fileInfo);
    void addSong(const QFileInfo &fileInfo);
    void addLotOfSongs(const QList<QUrl> &droppedData);
    void addSongsDir(const QList<QUrl> &droppedData);
    static QStringList m_allFiles;
    QString m_playlistName;
    QString m_login;
    QList<PlayerlistItem *> m_widgets;
};
