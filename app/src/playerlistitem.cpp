
#include "playerlistitem.h"
#include "ui_playerlistitem.h"

void PlayerlistItem::print() {
    qDebug() << "Edit Tags";
}

PlayerlistItem::PlayerlistItem(int number, const QString& pathTrack, const QString &trackName, QWidget *parent) :
    QWidget(parent), ui(new Ui::PlayerlistItem), m_pathToTrack(pathTrack), m_trackName(trackName),
    m_fileInfo(pathTrack.toStdString(), trackName.toStdString()) {
    ui->setupUi(this);

    QString name(trackName.begin(), trackName.lastIndexOf("."));
    ui->trackNameAndArtist->setText(QString::number(number) + ": " + name);
    ui->infoAboutTrack->setText("more info");
    ui->duration->setText("3:34");

    auto *contextMenu = new QMenu(this);
    auto *editTags = new QAction(tr("Edit file tags"), contextMenu);

    contextMenu->addAction(editTags);
    connect(editTags, &QAction::triggered, this, &PlayerlistItem::execEditFileTagsDialog);
    ui->pushButton->setMenu(contextMenu);
}

PlayerlistItem::~PlayerlistItem() {
    delete ui;
}
void PlayerlistItem::execEditFileTagsDialog() {
    auto *tagsDialog = new EditFileTagsDialog(m_fileInfo, this);
    tagsDialog->exec();
}

