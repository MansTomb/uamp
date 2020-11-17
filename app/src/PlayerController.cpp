#include "PlayerController.h"

PlayerController::PlayerController() {

}

void PlayerController::SetSong(FileTags *song) {
    m_song = song;
    BASS_StreamFree(m_sample);
    m_sample = BASS_StreamCreateFile(FALSE, song->tags.path.toStdString().c_str(), 0, 0, BASS_ASYNCFILE);
    m_channel = BASS_SampleGetChannel(m_sample, false);
}

void PlayerController::ApplySettings(FXData &data) {
    Pause();
    m_fxChorusHandle = BASS_ChannelSetFX(m_sample, BASS_FX_DX8_CHORUS, 1);
    m_fxCompressorHandle = BASS_ChannelSetFX(m_sample, BASS_FX_DX8_COMPRESSOR, 2);
    m_fxEchoHandle = BASS_ChannelSetFX(m_sample, BASS_FX_DX8_ECHO, 3);
    m_fxDistortionHandle = BASS_ChannelSetFX(m_sample, BASS_FX_DX8_DISTORTION, 4);
    m_fxParameqHandle = BASS_ChannelSetFX(m_sample, BASS_FX_DX8_PARAMEQ, 1);

    BASS_FXGetParameters(m_fxParameqHandle, &m_parameq);
    BASS_FXGetParameters(m_fxCompressorHandle, &m_compressor);
    BASS_FXGetParameters(m_fxChorusHandle, &m_chorus);
    BASS_FXGetParameters(m_fxEchoHandle, &m_echo);
    BASS_FXGetParameters(m_fxDistortionHandle, &m_distrotion);
    m_parameq.fGain = data.Gain;
    m_echo.fFeedback = data.Echo;
    m_compressor.fAttack = data.Compressor;
    m_distrotion.fEdge = data.Distortion;
    m_chorus.fDepth = data.Chorus;
    BASS_FXSetParameters(m_fxChorusHandle, &m_chorus);
    BASS_FXSetParameters(m_fxEchoHandle, &m_echo);
    BASS_FXSetParameters(m_fxCompressorHandle, &m_compressor);
    BASS_FXSetParameters(m_fxDistortionHandle, &m_distrotion);
    BASS_FXSetParameters(m_fxParameqHandle, &m_parameq);
    Play();
}
