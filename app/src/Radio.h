#pragma once

#include <QDialog>
#include <QDebug>
#include <QMessageBox>

#include "bass.h"
#include "ui_Radio.h"

namespace Ui {
class Radio;
}

class Radio : public QDialog {
 Q_OBJECT

 public:
    explicit Radio(QWidget *parent = nullptr);
    ~Radio();

 public slots:
    void OnPlayPause() {if (m_play) BASS_ChannelPlay(m_stream, false); else BASS_ChannelPause(m_stream);m_play = !m_play;};
    void OnVolumeChanged(int value) {BASS_ChannelSetAttribute(m_stream,BASS_ATTRIB_VOL,static_cast<float>(value) / 100);};
    void OnConnect() {
        m_stream = BASS_StreamCreateURL(ui->lineEdit->text().toLatin1(), 0, 0, nullptr, nullptr);
        if (m_stream == 0)
            QMessageBox::about(this, "Wrong URL", "Cant connect to this url!");
        else
            QMessageBox::about(this, "Connected", "Succesfully connected!");
    };

 private:
    Ui::Radio *ui;

    bool m_play{true};
    HSTREAM m_stream;
};
