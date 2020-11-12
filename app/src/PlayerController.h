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

    void SetSong(const FileTags *song);
    void SetVolume(int value) { if (!m_muted) BASS_ChannelSetAttribute(sample, BASS_ATTRIB_VOL, static_cast<float>(value) / 100);};
    void Play() { BASS_ChannelPlay(sample, FALSE); };
    void Pause() { BASS_ChannelPause(sample); };
    void Stop() {Pause(); SetPosition(0);};
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

    void ApplySettings(FXData& data);

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

    BASS_DX8_CHORUS m_chorus;
    BASS_DX8_COMPRESSOR m_compressor ;
    BASS_DX8_ECHO m_echo;
    BASS_DX8_DISTORTION m_distrotion;
    BASS_DX8_PARAMEQ m_parameq;

    int fxChorusHandle;
    int fxCompressorHandle;
    int fxEchoHandle;
    int fxDistortionHandle;
    int fxParameqHandle;

    HCHANNEL channel;
    HSAMPLE sample;
};
