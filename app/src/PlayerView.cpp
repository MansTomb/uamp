#include "PlayerView.h"

#include "FileTag.h"

PlayerView::PlayerView(QWidget *parent) : QWidget(parent), ui(new Ui::PlayerView) {
    ui->setupUi(this);
//
//    QMediaPlaylist *playlist = new QMediaPlaylist;
//
//    playlist->addMedia(QUrl::fromLocalFile("/Users/abalabin/Desktop/test.mp3"));
//    playlist->addMedia(QUrl::fromLocalFile("/Users/abalabin/Desktop/test.mp3"));
//    playlist->addMedia(QUrl::fromLocalFile("/Users/abalabin/Desktop/test.mp3"));
//
//    playlist->save(QUrl::fromLocalFile("/Users/abalabin/Desktop/pylist.m3u"), "m3u");

//    SetSong(FileTags(QUrl::fromLocalFile("/Users/mmasniy/Desktop/Music For Uamp/Rag'n'Bone Man - Human.mp3")));
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

void PlayerView::SetSong(FileTags *song) {
    ui->playerArtistName->setText(song->tags.artist);
    ui->playerTrackName->setText(song->tags.title);

    m_player.SetSong(song);
}
