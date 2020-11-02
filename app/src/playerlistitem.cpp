
#include <QDir>
#include "playerlistitem.h"
#include "ui_playerlistitem.h"

PlayerlistItem::PlayerlistItem(int number, const QString& pathTrack, const QString &trackName, QWidget *parent) :
    QWidget(parent), ui(new Ui::PlayerlistItem), m_pathToTrack(pathTrack), m_trackName(trackName),
    m_fileInfo(new FileTags(pathTrack.toStdString(), trackName.toStdString())) {
    ui->setupUi(this);

    QString name(trackName.begin(), trackName.lastIndexOf("."));
    ui->trackNameAndArtist->setText(QString::number(number) + ": " + name);
    ui->infoAboutTrack->setText(getFormat() + "::" + m_fileInfo->tags.bitrate + "::" + m_fileInfo->tags.channels
                                + "::" + m_fileInfo->tags.sampleRate);
    ui->duration->setText(m_fileInfo->tags.length);

    auto *contextMenu = new QMenu(this);
    auto *editTags = new QAction(tr("Edit file tags"), contextMenu);
    auto *remove = new QAction(tr("Remove from playlist"), contextMenu);
    auto *addToPlaylist = new QAction(tr("Add to playlist"), contextMenu);


    contextMenu->addAction(editTags);
    contextMenu->addAction(remove);
    contextMenu->addAction(addToPlaylist);
    connect(editTags, &QAction::triggered, this, &PlayerlistItem::execEditFileTagsDialog);
    connect(remove, &QAction::triggered, this, &PlayerlistItem::removeFromPlaylist);
    connect(addToPlaylist, &QAction::triggered, this, &PlayerlistItem::addToPlaylist);
    ui->pushButton->setMenu(contextMenu);
}

PlayerlistItem::~PlayerlistItem() {
    delete ui;
}

void PlayerlistItem::execEditFileTagsDialog() {
    auto *tagsDialog = new EditFileTagsDialog(*m_fileInfo, this);
    tagsDialog->exec();
    emit SetImage(m_fileInfo->getImage());
}

QString PlayerlistItem::getFormat() {
    std::string formatFile = m_fileInfo->tags.filename.toStdString();
    return QString::fromStdString(formatFile.substr(formatFile.find_last_of('.') + 1)).toUpper();
}

void PlayerlistItem::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    emit CurrentSong(m_fileInfo);
}

FileTags *PlayerlistItem::song() {
    return m_fileInfo;
}

void PlayerlistItem::removeFromPlaylist() {
    qDebug() << "remove from playlist";
    qDebug() << m_playlists;
    bool ok = false;
    QString text = QInputDialog::getItem(this, tr("Remove track"), tr("Choose playlist:"), m_playlists);
    qDebug() << text;
    //remove
}

void PlayerlistItem::addToPlaylist() {
    qDebug() << "add to playlist";
    //add
}
void PlayerlistItem::updateListPlaylist(QStringList playlists) {
    m_playlists.clear();
    for (const auto &item : playlists) {
        m_playlists << item;
    }
}

