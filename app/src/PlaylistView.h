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
#include "FileTag.h"

#define FILE_FORMATS format == "mp3" || format == "flac" || format == "mp4" || format == "wav" || format == "m4a" || format == "ogg"
class PlaylistView : public QListWidget {

    Q_OBJECT

public:
    explicit PlaylistView(QWidget *parent = Q_NULLPTR);
    void addWidget(const QString &pathTrack,
                   const QString &trackName,
                   PlayerlistItem *widget,
                   QListWidgetItem *item);
    void addWidget(FileTags *song);
    void clearingListWidget();

public slots:
    void ChangeCurrentSong(FileTags *song) {emit CurrentSongChanged(song);};
    void updatePlaylists(QStringList playlists) {m_playlistNames = playlists; emit SetPlaylists(playlists);};
    void RemoveTrackFromPlaylistSlot(FileTags *file, const QString &objectName);
    void AddTracktoPlaylistSlot(const QString& toPlaylist, FileTags *file) {emit AddTrackToPlaylist(toPlaylist, file);};
    void PlaylistDeleted(QString deletedPlaylist,
                         MenuPlaylistItemView *defaultPlaylist);
    void PlaylistChoosedSlot(MenuPlaylistItemView *playlist);
    void GetLogin(QString login);

    void setShuffle(bool state) {m_shuffle = state;};
    void setRepeat(bool state) {m_repeat = state;};
    void setPlaylistName(QString name) {
        m_playlistName = name;
        emit ThrowPlaylistName(name);
    };

    void NextSong();
    void PreviousSong();

    void InvalidFileOnPlayer(FileTags *song);

    PlayerlistItem *GetWidgetByItem(QListWidgetItem *item) {return dynamic_cast<PlayerlistItem *>(itemWidget(currentItem()));}

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
    int getRandom(int from, int to);
    static bool checkPerm(const QFileInfo &fileInfo);
    void AddOneSong(const QUrl &fileUrl);
    void AddDirWithSongs(const QUrl &fileUrls);
    void AddSongToListAndToDB(const QString &pathTrack, const QString &trackName);
    void AddSongToListWithoutDB(FileTags* song);
    bool containedDefault(const QString& pathToTrack);
    static QStringList m_allFiles;

    bool m_shuffle {false};
    bool m_repeat {false};
    QString m_playlistName;
    QString m_login;
    QStringList m_playlistNames;
    QStringList m_default;
    QList<int> m_playedSongs;
    QList<PlayerlistItem *> m_widgets;
    bool CheckIfSongAlreadyAdded(const QFileInfo &fInfo) const;
};
