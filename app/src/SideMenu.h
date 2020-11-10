#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>

#include "Equalizer.h"
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
    void RemoveSongFromPlaylist(QString playlistName, FileTags *song) {ui->playlists->RemoveSongFromPlaylist(playlistName,song);};

    void SongAddedToPlaylistSlot(QString playlistName, FileTags *song) {emit SongAddedToPlaylist(playlistName, song);};
    void SongRemovedFromPlaylistSlot(QString playlistName, FileTags *song) {emit SongRemovedFromPlaylist(playlistName, song);};
    void PlaylistRenamedSlot(QString old, QString newName) {emit PlaylistRenamed(old, newName); emit PlaylistRenamedForLabel(newName);};
    void PlaylistDeletedSlot(QString playlistName, MenuPlaylistItemView *playlist) {emit PlaylistDeleted(playlistName, playlist);};
    void PlaylistCreatedSlot(QString playlistName) {emit PlaylistCreated(playlistName);};
    void PlaylistChoosedNameSlot(QString playlistName) {emit PlaylistChoosedName(playlistName);};
    void PlaylistChoosedSlot(MenuPlaylistItemView *playlist) {emit PlaylistChoosed(playlist);};
    void PlaylistsSlot(QStringList playlists) {emit ThrowPlaylists(playlists);};

    void OpenEqualizerSlot();

 signals:
    void SongAddedToPlaylist(QString playlistName, FileTags *song);
    void SongRemovedFromPlaylist(QString playlistName, FileTags *song);
    void PlaylistRenamed(QString old, QString newName);
    void PlaylistRenamedForLabel(QString newName);
    void PlaylistDeleted(QString playlistName, MenuPlaylistItemView *playlist);
    void PlaylistCreated(QString playlistName);
    void PlaylistChoosedName(QString playlistName);
    void PlaylistChoosed(MenuPlaylistItemView *playlist);
    void ThrowPlaylists(QStringList playlists);

    void EqualizerChanged(FXData);

private:
    Ui::SideMenu *ui;
    Equalizer *m_eq {Q_NULLPTR};
};

#endif // SIDEMENU_H
