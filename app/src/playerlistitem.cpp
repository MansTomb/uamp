
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
    ui->infoAboutTrack->setText(getFormat() + "::" + m_fileInfo.tags.bitrate + "::" + m_fileInfo.tags.channels
                                + "::" + m_fileInfo.tags.sampleRate);
    ui->duration->setText(m_fileInfo.tags.length);

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

QString PlayerlistItem::getFormat() {
    std::string formatFile = m_fileInfo.tags.filename.toStdString();
    return QString::fromStdString(formatFile.substr(formatFile.find_last_of('.') + 1)).toUpper();
}
void PlayerlistItem::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    emit CurrentSong(&m_fileInfo);
}

