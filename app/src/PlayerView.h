#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QWidget>

#include "PlayerController.h"

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




 private:
    Ui::PlayerView *ui;
    PlayerController m_player;
};

#endif // PLAYERVIEW_H
