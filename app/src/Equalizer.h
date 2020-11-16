#pragma once

#include <QDialog>
#include <QDebug>

#include "ui_Equalizer.h"

namespace Ui {
class Equalizer;
}

typedef struct s_FXData {
    float Gain;
    float Echo;
    float Distortion;
    float Compressor;
    float Chorus;
} FXData;

typedef struct s_Preset {
    QString name;
    FXData preset;
} EQPreset;

class Equalizer : public QDialog {
 Q_OBJECT

 public:
    explicit Equalizer(QWidget *parent = nullptr);
    ~Equalizer();

 public slots:
    void OnApplyClickedSlot() {
        emit OnApplyClicked({static_cast<float>(ui->GainSlider->value()),
                             static_cast<float>(ui->Echo->value()),
                             static_cast<float>(ui->Distortion->value()),
                             static_cast<float>(ui->Compressor->value()),
                             static_cast<float>(ui->Chorus->value())});
    };

    void OnSavePresetSlot();
    void OnPresetChanged(QString presetName);
    void OnDefault();

 signals:
    void OnApplyClicked(FXData);

 private:
    Ui::Equalizer *ui;
    QList<EQPreset> m_presets;

    void LoadAllPresetsFromDB();
};
