#include "Equalizer.h"

Equalizer::Equalizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Equalizer)
{
    ui->setupUi(this);
}

Equalizer::~Equalizer()
{
    delete ui;
}

void Equalizer::LoadAllPresetsFromDB() {
//    m_presets = Database::instance::GetAllPresetsForThisUser;
//    int index = 0;
//    for (const auto &preset : presets) {
//        if (preset.LastPresetWhichUserApplied)
//            index++;
//        ui->comboBox->insertItem(index, preset.name);
//    }
//    ui->comboBox->setCurrentIndex(0);
}

void Equalizer::OnSavePresetSlot() {
    // Insert this this preset to DB
}

void Equalizer::OnPresetChanged(QString presetName) {
//    auto it = std::find(m_presets.begin(), m_presets.end(), [presetName](EQPreset& preset){ return preset.name == presetName;});
//
//    ui->GainSlider->setValue(it->preset.Gain);
//    ui->Echo->setValue(it->preset.Echo);
//    ui->Distortion->setValue(it->preset.Distortion);
//    ui->Compressor->setValue(it->preset.Compressor);
//    ui->Chorus->setValue(it->preset.Chorus);
}
