#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QWidget>
#include <QPixmap>

#include "PlayerController.h"
#include "ui_playerview.h"

namespace Ui {
class PlayerView;
}

class PlayerView : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerView(QWidget *parent = nullptr);
    ~PlayerView();


public slots:
    void ChangeVolume(int value);
    void Play(bool state);
    void Mute(bool state);
    void Previous();
    void Next();
    void Backward();
    void Forward();
    void SetSong(FileTags *song);
    void SetImage(QPixmap *img);

 private:
    void setDefaultImage();
    Ui::PlayerView *ui;
    PlayerController m_player;
    QPixmap *m_picture;
};

#endif // PLAYERVIEW_H
