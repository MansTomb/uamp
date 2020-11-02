#pragma once
//
//#include "../../taglib/taglib/tag.h"
//#include <../taglib/fileref.h>
//#include <toolkit/tpropertymap.h>
//#include <mpegfile.h>
//#include <id3v2tag.h>

#include <QListWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDir>
#include <QDebug>

#include "playerlistitem.h"
#include "MenuPlaylistItemView.h"

class PlaylistView : public QListWidget {

    Q_OBJECT

public:
    explicit PlaylistView(QWidget *parent = Q_NULLPTR);
    void addWidget(const QString& pathTrack, const QString &trackName);

public slots:
    void ChangeCurrentSong(FileTags *song) {emit CurrentSongChanged(song);};
    void updatePlaylists(QStringList playlists) {emit SetPlaylists(playlists);};

signals:
    void CurrentSongChanged(FileTags *song);
    void FileAdded(QString playlistName, FileTags *song);
    void SetImage(QPixmap *img);
    void SetPlaylists(QStringList playlists);

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
};
