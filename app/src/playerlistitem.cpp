#include <QMenu>
#include "playerlistitem.h"
#include "ui_playerlistitem.h"

PlayerlistItem::PlayerlistItem(int number, const QString& pathTrack, const QString &trackName, QWidget *parent) :
    QWidget(parent), ui(new Ui::PlayerlistItem), m_pathToTrack(pathTrack), m_trackName(trackName),
    m_fileInfo(pathTrack.toStdString(), trackName.toStdString()) {
    ui->setupUi(this);

    QString name(trackName.begin(), trackName.lastIndexOf("."));
    ui->trackNameAndArtist->setText(QString::number(number) + ": " + name);
    ui->infoAboutTrack->setText("more info");
    ui->duration->setText("3:34");
}

PlayerlistItem::~PlayerlistItem() {
    delete ui;
}

void PlayerlistItem::on_pushButton_clicked() {
    connect(this, &PlayerlistItem::customContextMenuRequested, this, &PlayerlistItem::ShowContextMenu);
}

void PlayerlistItem::ShowContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context Menu"), this);
    QAction editTags(tr("Edit file tags"), this);

    contextMenu.addAction(&editTags);
    connect(&editTags, &QAction::trigger, this, [] (){qDebug() << "Edit Tags";});
    contextMenu.exec(mapToGlobal(pos));
}
