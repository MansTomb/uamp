#include "EditFileTagsDialog.h"
#include "ui_EditFileTagsDialog.h"

EditFileTagsDialog::EditFileTagsDialog(FileTags& file, QWidget *parent) :
    QDialog(parent), ui(new Ui::EditFileTagsDialog) {
    ui->setupUi(this);

    setObjectName(tr("Edit audio-file tags"));
}

EditFileTagsDialog::~EditFileTagsDialog() {
    delete ui;
}

/*
void Preferences::accept() {
    m_preferences["font"] = ui->comboBox->currentText();
    m_preferences["size_font"] = ui->comboBox_2->currentText();
    m_preferences["theme"] = ui->comboBox_3->currentText();
    m_preferences["language"] = ui->comboBox_4->currentText();
    emit ReturnValues(m_preferences);
    QDialog::accept();
}


 */
