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

private:
    Ui::PlayerView *ui;
    PlayerController m_player;
};

#endif // PLAYERVIEW_H
