#pragma once

#include <QtMultimedia>
#include <QtMultimediaWidgets>

#include "FileTag.h"
#include "Equalizer.h"
#include "bass.h"

class PlayerController : public QObject {
 Q_OBJECT
 public:
    explicit PlayerController();

    void SetSong(FileTags *song);
    FileTags *GetSong() {return m_song;}
    void SetVolume(int value) { if (!m_muted) BASS_ChannelSetAttribute(m_sample, BASS_ATTRIB_VOL, static_cast<float>(value) / 100);};
    bool Play() {return BASS_ChannelPlay(m_sample, FALSE);};
    void Pause() { BASS_ChannelPause(m_sample); };
    void Stop() {Pause(); SetPosition(0);};
    void Next() {};
    void Previous() {};
    void Backward() {BASS_ChannelSetPosition(m_sample, BASS_ChannelGetPosition(m_sample, BASS_POS_BYTE) - BASS_ChannelSeconds2Bytes(m_sample, 10), BASS_POS_BYTE);};
    void Forward() {BASS_ChannelSetPosition(m_sample, BASS_ChannelGetPosition(m_sample, BASS_POS_BYTE) + BASS_ChannelSeconds2Bytes(m_sample, 10), BASS_POS_BYTE);};
    void SetPosition(int sec) {BASS_ChannelSetPosition(m_sample, BASS_ChannelSeconds2Bytes(m_sample, sec), BASS_POS_BYTE);}
    void Mute(bool state) {
        if (state) {
            BASS_ChannelGetAttribute(m_sample, BASS_ATTRIB_VOL, &m_lastVolume);
            BASS_ChannelSetAttribute(m_sample, BASS_ATTRIB_VOL, 0);
            m_muted = true;
        }
        else {
            BASS_ChannelSetAttribute(m_sample, BASS_ATTRIB_VOL, m_lastVolume);
            m_muted = false;
        }
    };

    void ApplySettings(FXData& data);

    double GetCurrentSongTime() {return BASS_ChannelBytes2Seconds(m_sample, BASS_ChannelGetPosition(m_sample, BASS_POS_BYTE));}
    double GetSongTime() {return BASS_ChannelBytes2Seconds(m_sample, BASS_ChannelGetLength(m_sample, BASS_POS_BYTE));}
 public slots:
    void ChangeCurrentPos(float value) {emit CurrentPosChanged(value + 1);};
signals:
    void CurrentPosChanged(float value);
    void SongEnded();
 private:
    bool m_muted{false};
    float m_lastVolume;

    BASS_DX8_CHORUS m_chorus;
    BASS_DX8_COMPRESSOR m_compressor ;
    BASS_DX8_ECHO m_echo;
    BASS_DX8_DISTORTION m_distrotion;
    BASS_DX8_PARAMEQ m_parameq;

    int m_fxChorusHandle;
    int m_fxCompressorHandle;
    int m_fxEchoHandle;
    int m_fxDistortionHandle;
    int m_fxParameqHandle;

    HCHANNEL m_channel;
    HSAMPLE m_sample;

    FileTags *m_song {Q_NULLPTR};
};
