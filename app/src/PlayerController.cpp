#include "PlayerController.h"

PlayerController::PlayerController() {

}

void PlayerController::SetSong(const FileTags *song) {
    BASS_StreamFree(sample);
    sample = BASS_StreamCreateFile(FALSE, song->tags.path.toStdString().c_str(), 0, 0, BASS_ASYNCFILE);
    channel = BASS_SampleGetChannel(sample, false);
}

void PlayerController::ApplySettings(FXData &data) {
    Pause();
    fxChorusHandle = BASS_ChannelSetFX(sample, BASS_FX_DX8_CHORUS, 1);
    fxCompressorHandle = BASS_ChannelSetFX(sample, BASS_FX_DX8_COMPRESSOR, 2);
    fxEchoHandle = BASS_ChannelSetFX(sample, BASS_FX_DX8_ECHO, 3);
    fxDistortionHandle = BASS_ChannelSetFX(sample, BASS_FX_DX8_DISTORTION, 4);
    fxParameqHandle = BASS_ChannelSetFX(sample, BASS_FX_DX8_PARAMEQ, 1);

    BASS_FXGetParameters(fxParameqHandle, &m_parameq);
    BASS_FXGetParameters(fxCompressorHandle, &m_compressor);
    BASS_FXGetParameters(fxChorusHandle, &m_chorus);
    BASS_FXGetParameters(fxEchoHandle, &m_echo);
    BASS_FXGetParameters(fxDistortionHandle, &m_distrotion);
    m_parameq.fGain = data.Gain;
    m_echo.fFeedback = data.Echo;
    m_compressor.fAttack = data.Compressor;
    m_distrotion.fEdge = data.Distortion;
    m_chorus.fDepth = data.Chorus;
    BASS_FXSetParameters(fxChorusHandle, &m_chorus);
    BASS_FXSetParameters(fxEchoHandle, &m_echo);
    BASS_FXSetParameters(fxCompressorHandle, &m_compressor);
    BASS_FXSetParameters(fxDistortionHandle, &m_distrotion);
    BASS_FXSetParameters(fxParameqHandle, &m_parameq);
    Play();
}
