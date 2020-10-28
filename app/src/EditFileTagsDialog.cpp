#include "EditFileTagsDialog.h"
#include "ui_EditFileTagsDialog.h"

EditFileTagsDialog::EditFileTagsDialog(FileTags& file, QWidget *parent) :
    QDialog(parent), ui(new Ui::EditFileTagsDialog) {
    ui->setupUi(this);
    setWindowTitle(tr("Edit audio-file tags"));
    ui->trackName->setText(QString(file.tags.filename.begin(), file.tags.filename.lastIndexOf(".")));
    ui->artistText_2->setPlainText(file.tags.artist);
    ui->titleText_2->setPlainText(file.tags.title);
    ui->albumText_2->setPlainText(file.tags.album);
    ui->genreText_2->setPlainText(file.tags.album);
    ui->pathText_2->setPlainText(file.tags.path);
    ui->pathText_2->setReadOnly(true);
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
