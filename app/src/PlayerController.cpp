#include "PlayerController.h"

PlayerController::PlayerController() {
    m_player = new QMediaPlayer;
    m_player->setMedia(QUrl::fromLocalFile("/Users/abalabin/Desktop/test.mp3"));
    m_player->setAudioRole(QAudio::Role::MusicRole);
}
