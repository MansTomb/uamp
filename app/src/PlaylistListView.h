#pragma once

#include <QListWidget>
#include <QDebug>
#include <QErrorMessage>
#include <QMenu>
#include <QInputDialog>
#include <QDir>

#include "ui_playlistlistview.h"
#include "FileTag.h"
#include "MenuPlaylistItemView.h"
#include "SqlDatabase.h"

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
    bool ImportNewPlaylist(QString filepath, QString playlistName);

 signals:
    void SongAddedToPlaylist(QString playlistName, FileTags *song);
    void SongRemovedFromPlaylist(QString playlistName, FileTags *song);
    void PlaylistRenamed(QString old, QString newName);
    void PlaylistDeleted(QString playlistName, MenuPlaylistItemView *playlist);
    void PlaylistCreated(QString playlistName);
    void Playlists(QStringList playlists);
    void PlaylistChoosed(MenuPlaylistItemView *playlist);
    void PlaylistChoosedName(QString playlistName);

 private:
    Ui::PlaylistsView *ui;

    void LoadPlaylists();
    void CreatePlaylistView();
    void CreateImportPlaylistView();
    void SetupPlaylist(const QString &playlistName, QListWidgetItem *item, MenuPlaylistItemView *playlist);
    void ThrowPlaylists();
};
