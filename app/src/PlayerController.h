#pragma once

#include <QtMultimedia>
#include <QtMultimediaWidgets>

#include "FileTag.h"
#include "bass.h"

//class PlayerController : public QObject {
//    Q_OBJECT
// public:
//    explicit PlayerController();
//
//    void SetSong(const FileTags *song) {m_player->setMedia(QUrl::fromLocalFile(song->tags.path));};
//    void SetVolume(int value) {m_player->setVolume(value);};
//    void Play() {m_player->play();};
//    void Pause() {m_player->pause();};
//    void Next() {};
//    void Previous() {};
//    void Backward() {m_player->setPosition(m_player->position() - 10000);};
//    void Forward() {m_player->setPosition(m_player->position() + 10000);};
//    void Mute(bool state) {m_player->setMuted(state);};
// private:
//    QMediaPlayer *m_player;
//};

class PlayerController : public QObject {
 Q_OBJECT
 public:
    explicit PlayerController();

    void SetSong(const FileTags *song);
    void SetVolume(int value) { if (!m_muted) BASS_ChannelSetAttribute(sample, BASS_ATTRIB_VOL, static_cast<float>(value) / 100);};
    void Play() { BASS_ChannelPlay(sample, FALSE); };
    void Pause() { BASS_ChannelPause(sample); };
    void Next() {};
    void Previous() {};
    void Backward() {BASS_ChannelSetPosition(sample,BASS_ChannelGetPosition(sample, BASS_POS_BYTE) - BASS_ChannelSeconds2Bytes(sample, 10),BASS_POS_BYTE);};
    void Forward() {BASS_ChannelSetPosition(sample,BASS_ChannelGetPosition(sample, BASS_POS_BYTE) + BASS_ChannelSeconds2Bytes(sample, 10), BASS_POS_BYTE);};
    void SetPosition(int sec) {BASS_ChannelSetPosition(sample, BASS_ChannelSeconds2Bytes(sample, sec), BASS_POS_BYTE);}
    void Mute(bool state) {
        if (state) {
            BASS_ChannelGetAttribute(sample, BASS_ATTRIB_VOL, &m_lastVolume);
            BASS_ChannelSetAttribute(sample, BASS_ATTRIB_VOL, 0);
            m_muted = true;
        }
        else {
            BASS_ChannelSetAttribute(sample, BASS_ATTRIB_VOL, m_lastVolume);
            m_muted = false;
        }
    };
    double GetCurrentSongTime() {return BASS_ChannelBytes2Seconds(sample, BASS_ChannelGetPosition(sample, BASS_POS_BYTE));}
    double GetSongTime() {return BASS_ChannelBytes2Seconds(sample, BASS_ChannelGetLength(sample, BASS_POS_BYTE));}
 public slots:
    void ChangeCurrentPos(float value) {emit CurrentPosChanged(value + 1);};
signals:
    void CurrentPosChanged(float value);
    void SongEnded();
 private:
    bool m_muted{false};
    float m_lastVolume;
    HCHANNEL channel;
    HSAMPLE sample;
};
