#include "PlaylistListView.h"
#include "MenuPlaylistItemView.h"

PlaylistListView::PlaylistListView(QWidget *parent) : QWidget(parent), ui(new Ui::PlaylistsView) {
    ui->setupUi(this);
    LoadDefaultPlaylist();
}

void PlaylistListView::CreateNewPlaylist(QString playlistName) {
    auto *item = new QListWidgetItem;
    auto playlist = new MenuPlaylistItemView(playlistName, item, this);

    ui->list->addItem(item);
    item->setSizeHint(QSize(50, 75));
    ui->list->setItemWidget(item, playlist);

    connect(playlist, &MenuPlaylistItemView::PlaylistRenamed, this, &PlaylistListView::PlaylistRenamed);
    connect(playlist, &MenuPlaylistItemView::SongAdded, this, &PlaylistListView::SongAddedToPlaylist);
    connect(playlist, &MenuPlaylistItemView::SongRemoved, this, &PlaylistListView::SongRemovedFromPlaylist);
    emit PlaylistCreated(playlistName);
}

void PlaylistListView::LoadDefaultPlaylist() {
    CreateNewPlaylist(QString("Default"));
}

void PlaylistListView::AddSongToPlaylist(const QString &playlistName, FileTags *song) {
    ui->list->findChild<MenuPlaylistItemView *>(playlistName)->AddSong(song);
    qDebug() << ui->list->findChild<MenuPlaylistItemView *>(playlistName)->Playlist().count();
}

void PlaylistListView::RemoveSongFromPlaylist(const QString &playlistName, FileTags *song) {
    ui->list->findChild<MenuPlaylistItemView *>(playlistName)->RemoveSong(song);
}

void PlaylistListView::RenamePlaylist(const QString& old, QString newName) {
    ui->list->findChild<MenuPlaylistItemView *>(old)->Rename(std::move(newName));
}

void PlaylistListView::DeletePlaylist(const QString& playlistName) {
    delete ui->list->findChild<MenuPlaylistItemView *>(playlistName)->ParentItem();
    emit PlaylistDeleted(playlistName);
}
