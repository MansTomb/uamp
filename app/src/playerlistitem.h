#ifndef PLAYERLISTITEM_H
#define PLAYERLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QInputDialog>
#include <QList>


#include "FileTag.h"
#include "EditFileTagsDialog.h"
#include "MenuPlaylistItemView.h"

namespace Ui {
class PlayerlistItem;
}

class PlayerlistItem : public QWidget{
    Q_OBJECT

public:
    explicit PlayerlistItem(QListWidgetItem *parentItem, const QString &pathTrack, const QString &trackName, QWidget *parent = nullptr);
    ~PlayerlistItem() override;
    FileTags *song();
    QListWidgetItem *getParentToDelete() {return m_parent;};

public slots:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void execEditFileTagsDialog();
    void removeFromPlaylist();
    void addToPlaylist();
    void updateListPlaylist(QStringList playlists);
    void setPlaylistName(QString name) {m_playlistName = name;};

signals:
    void CurrentSong(FileTags *song);
    void SetImage(QPixmap *img);
    void RemoveTrackFromPlaylist(FileTags *file, const QString &objectName);
    void AddTracktoPlaylist(const QString& objectName, FileTags *file);


private:
    QString getFormat();
    Ui::PlayerlistItem *ui;
    QListWidgetItem *m_parent;
    QString m_pathToTrack;
    QString m_trackName;
    FileTags *m_fileInfo;
    QStringList m_playlists;
    QString m_playlistName = "Default";
};

#endif // PLAYERLISTITEM_H
