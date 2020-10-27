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

    SetSong(QUrl::fromLocalFile("/Users/abalabin/Desktop/test.mp3"));
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

void PlayerView::SetSong(QUrl song) {
    QString path(song.toString().remove(0, 7));
    QString fname(song.toString());
    FileTags tagger(path.remove(path.lastIndexOf("/"), path.size()), fname.remove(0, fname.lastIndexOf("/")));

    ui->playerArtistName->setText(tagger.tags.artist);
    ui->playerTrackName->setText(tagger.tags.name);

    m_player.SetSong(song);
}
