#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>

#include "ui_SideMenu.h"

namespace Ui {
class SideMenu;
}

class SideMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SideMenu(QWidget *parent = nullptr);
    ~SideMenu();

 public slots:
    void AddSongToPlaylist(QString playlistName, FileTags *song) { ui->playlists->AddSongToPlaylist(playlistName, song);};
    void RemoveSongToPlaylist(QString playlistName, FileTags *song) {ui->playlists->RemoveSongFromPlaylist(playlistName,song);};
    void SongAddedToPlaylistSlot(QString playlistName, FileTags *song) {emit SongAddedToPlaylist(playlistName, song);};
    void SongRemovedFromPlaylistSlot(QString playlistName, FileTags *song) {emit SongRemovedFromPlaylist(playlistName, song);};
    void PlaylistRenamedSlot(QString old, QString newName) {emit PlaylistRenamed(old, newName);};
    void PlaylistDeletedSlot(QString playlistName) {emit PlaylistDeleted(playlistName);};
    void PlaylistCreatedSlot(QString playlistName) {emit PlaylistCreated(playlistName);};

 signals:
    void SongAddedToPlaylist(QString playlistName, FileTags *song);
    void SongRemovedFromPlaylist(QString playlistName, FileTags *song);
    void PlaylistRenamed(QString old, QString newName);
    void PlaylistDeleted(QString playlistName);
    void PlaylistCreated(QString playlistName);

private:
    Ui::SideMenu *ui;
};

#endif // SIDEMENU_H
