#pragma once

#include <QWidget>
#include <QMediaPlaylist>
#include <QListWidgetItem>

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
    ~MenuPlaylistItemView();

    void AddSong(FileTags *song) {m_playlist.append(song); emit SongAdded(m_name, song);};
    void RemoveSong(FileTags *song) {m_playlist.removeOne(song); emit SongRemoved(m_name, song);};

    void Rename(QString newName) {m_name = newName; ui->playListname->setText(m_name);};

    QList<FileTags *> Playlist() const {return m_playlist;};
    QListWidgetItem *ParentItem() {return m_parent;};

 signals:
    void SongRemoved(QString name, FileTags *song);
    void SongAdded(QString name, FileTags *song);

private:
    QListWidgetItem *m_parent {Q_NULLPTR};
    Ui::MenuPlaylistItemView *ui;
    QList<FileTags *> m_playlist;
    QString m_name;

    QList<FileTags *> loadPlaylist();
};
