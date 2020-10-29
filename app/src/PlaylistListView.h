#pragma once

#include <QListWidget>
#include <QDebug>

#include "ui_playlistlistview.h"
#include "FileTag.h"
#include "MenuPlaylistItemView.h"

namespace Ui {
class PlaylistsView;
}

class PlaylistListView : public QWidget
{
    Q_OBJECT
 public:
    explicit PlaylistListView(QWidget *parent = nullptr);

 public slots:
    void AddSongToPlaylist(const QString &playlistName, FileTags *song);
    void RemoveSongFromPlaylist(const QString &playlistName, FileTags *song);
    void RenamePlaylist(const QString& old, QString newName);
    void DeletePlaylist(const QString& playlistName);
    void CreateNewPlaylist(QString playlistName);

 signals:
    void SongAddedToPlaylist(QString playlistName, FileTags *song);
    void SongRemovedFromPlaylist(QString playlistName, FileTags *song);
    void PlaylistRenamed(QString old, QString newName);
    void PlaylistDeleted(QString playlistName);
    void PlaylistCreated(QString playlistName);
 private:
    Ui::PlaylistsView *ui;
};
