#include <QFileDialog>
#include "EditFileTagsDialog.h"
#include "ui_EditFileTagsDialog.h"

EditFileTagsDialog::EditFileTagsDialog(FileTags& file, QWidget *parent) :
    QDialog(parent), ui(new Ui::EditFileTagsDialog), m_file(file) {
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

void EditFileTagsDialog::accept() {
    m_file.tags.artist = ui->artistText_2->toPlainText();
    m_file.tags.title = ui->titleText_2->toPlainText();
    m_file.tags.album = ui->albumText_2->toPlainText();
    m_file.tags.genre = ui->genreText_2->toPlainText();
    m_file.upgradeFileTags(m_file.tags.path.toStdString());
    if (m_changeImg) {
        m_file.setImage(m_pathToImg.toStdString().c_str());
        QImage img(m_pathToImg.toStdString().c_str());
        m_file.setM_picture(new QPixmap(QPixmap::fromImage(img)));
    }
    QDialog::accept();
}

void EditFileTagsDialog::on_pushButton_clicked() {
    m_pathToImg = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home",
                                               tr("Image Files (*.png *.jpg *.bmp)"));
    m_changeImg = true;
}
