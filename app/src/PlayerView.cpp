#include "PlayerView.h"
#include "ui_playerview.h"

PlayerView::PlayerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerView) {
    ui->setupUi(this);
}

PlayerView::~PlayerView() {
    delete ui;
}

void PlayerView::ChangeVolume(int value) {
    m_player.SetVolume(value);
}

void PlayerView::Play(bool state) {
    if (state)
        m_player.Play();
    else
        m_player.Pause();
}

void PlayerView::Mute(bool state) {
    m_player.Mute(state);
}

void PlayerView::Next() {
    m_player.Next();
}

void PlayerView::Previous() {
    m_player.Previous();
}

void PlayerView::Backward() {
    m_player.Backward();
}

void PlayerView::Forward() {
    m_player.Forward();
}
