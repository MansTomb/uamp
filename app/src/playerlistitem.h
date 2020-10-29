#ifndef PLAYERLISTITEM_H
#define PLAYERLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>


#include "FileTag.h"
#include "EditFileTagsDialog.h"

namespace Ui {
class PlayerlistItem;
}

class PlayerlistItem : public QWidget{
    Q_OBJECT

public:
    explicit PlayerlistItem(int number, const QString &pathTrack, const QString &trackName, QWidget *parent = nullptr);
    ~PlayerlistItem() override;

public slots:
 protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
 public:
    void print();
    void execEditFileTagsDialog();

signals:
    void CurrentSong(FileTags *song);

private:
    QString getFormat();
    Ui::PlayerlistItem *ui;
    QString m_pathToTrack;
    QString m_trackName;
    FileTags m_fileInfo;
};

#endif // PLAYERLISTITEM_H
