#ifndef PLAYERLISTITEM_H
#define PLAYERLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Ui {
class PlayerlistItem;
}

class PlayerlistItem : public QWidget{
    Q_OBJECT

public:
    explicit PlayerlistItem(int number, QWidget *parent = nullptr);
    ~PlayerlistItem();

private:
    Ui::PlayerlistItem *ui;
    QWidget *track;
    QLabel *trackNameAndArtist;
    QLabel *infoAboutTrack;
    QLabel *duration;
    QPushButton *menu;
};

#endif // PLAYERLISTITEM_H
