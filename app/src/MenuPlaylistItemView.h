#pragma once

#include <QWidget>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QMenu>
#include <QFileDialog>

#include "ui_MenuPlaylistItemView.h"
#include "FileTag.h"

namespace Ui {
class MenuPlaylistItemView;
}

class MenuPlaylistItemView : public QWidget
{
    Q_OBJECT
public:
    explicit MenuPlaylistItemView(QString playlistName,
                                  QListWidgetItem *wItem,
                                  QWidget *parent);
    explicit MenuPlaylistItemView(QMediaPlaylist *playlist,
                                  QString playlistName,
                                  QListWidgetItem *wItem,
                                  QWidget *parent);
    ~MenuPlaylistItemView();

    void AddSong(FileTags *song) {m_playlist.append(song); emit SongAdded(m_name, song);};
    void RemoveSong(FileTags *song) {m_playlist.removeOne(song); emit SongRemoved(m_name, song);};
    void Rename(QString newName) {emit PlaylistRenamed(m_name, newName);m_name = newName; ui->playListname->setText(m_name); setObjectName(m_name);};

    QString PlaylistName() const {return m_name;};
    QList<FileTags *> Playlist() const {return m_playlist;};
    QListWidgetItem *ParentItem() {return m_parent;};

 signals:
    void SongRemoved(QString name, FileTags *song);
    void SongAdded(QString name, FileTags *song);
    void PlaylistRenamed(QString old, QString newName);
    void DeletePlaylist(const QString& playlistName);

    void PlaylistChoosed(MenuPlaylistItemView *playlist);

 private slots:
    void actionLoad();
    void actionRename();
    void actionExport();
    void actionDelete();

private:
    QListWidgetItem *m_parent {Q_NULLPTR};
    Ui::MenuPlaylistItemView *ui;
    QList<FileTags *> m_playlist;
    QString m_name;

    QList<FileTags *> loadPlaylist();
    void AddSongsFromPlaylistMedia(const QMediaPlaylist *playlist);
 protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};
