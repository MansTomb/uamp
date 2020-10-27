#pragma once

#include <QtMultimedia>
#include <QtMultimediaWidgets>

class PlayerController : public QObject {
    Q_OBJECT
 public:
    explicit PlayerController();

    void SetVolume(int value) {m_player->setVolume(value);};
    void Play() {m_player->play(); qDebug() << "play";};
    void Pause() {m_player->pause();};
    void Next() {};
    void Previous() {};
    void Backward() {m_player->setPosition(m_player->position() - 10000);};
    void Forward() {m_player->setPosition(m_player->position() + 10000); qDebug() << "frowrd";};
    void Mute(bool state) {m_player->setMuted(state);};
 private:
    QMediaPlayer *m_player;
};
