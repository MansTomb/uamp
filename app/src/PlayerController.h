#pragma once

#include <QtMultimedia>
#include <QtMultimediaWidgets>

//#include "PlayerView.h"

class PlayerController : public QObject {
    Q_OBJECT
 public:
    explicit PlayerController();
 private:
    QMediaPlayer *m_player;
};
