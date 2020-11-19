#include <QInputDialog>
#include <QErrorMessage>
#include "Equalizer.h"

Equalizer::Equalizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Equalizer)
{
    ui->setupUi(this);
    SqlDatabase::instance().createDefaultPreset();

    m_presetsMap = SqlDatabase::instance().getPreset();
    m_presetsNames = m_presetsMap.keys();

    ui->comboBox->addItems(m_presetsNames);
    OnDefault();
}

Equalizer::~Equalizer()
{
    delete ui;
}

void Equalizer::LoadAllPresetsFromDB() {
}

void Equalizer::OnSavePresetSlot() {
    QMap<QString, int> preset;
    bool ok = false;
    QString text = QInputDialog::getText(this, tr("New preset"),
                                         tr("Preset name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !text.isEmpty()) {
        if (m_presetsNames.contains(text)) {
            auto msg = new QErrorMessage(this);
            msg->showMessage("Preset already exists");
            msg->exec();
            delete msg;
            return;
        }
    }
    else if (text.isEmpty()) {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Text cant be empty");
        msg->exec();
        delete msg;
        return;
    }
    preset["gain"] = ui->GainSlider->value();
    preset["echo"] = ui->Echo->value();
    preset["distortion"] = ui->Distortion->value();
    preset["compressor"] = ui->Compressor->value();
    preset["chorus"] = ui->Chorus->value();
    SqlDatabase::instance().addPreset(text, preset);
    m_presetsNames << text;
    ui->comboBox->insertItem(0, text);
}

void Equalizer::OnPresetChanged(QString presetName) {
    ui->GainSlider->setValue(m_presetsMap[presetName]["gain"]);
    ui->Echo->setValue(m_presetsMap[presetName]["echo"]);
    ui->Distortion->setValue(m_presetsMap[presetName]["distortion"]);
    ui->Compressor->setValue(m_presetsMap[presetName]["compressor"]);
    ui->Chorus->setValue(m_presetsMap[presetName]["chorus"]);
}
void Equalizer::OnDefault() {
    ui->comboBox->setCurrentText("Default");
}
