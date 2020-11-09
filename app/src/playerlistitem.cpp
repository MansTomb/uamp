
#include <QDir>
#include "playerlistitem.h"
#include "ui_playerlistitem.h"

PlayerlistItem::PlayerlistItem(QListWidgetItem *parentItem, const QString& pathTrack, const QString &trackName, QWidget *parent) :
    QWidget(parent), ui(new Ui::PlayerlistItem), m_pathToTrack(pathTrack), m_trackName(trackName),
    m_fileInfo(new FileTags(pathTrack.toStdString(), trackName.toStdString())), m_parent(parentItem) {
    ui->setupUi(this);
    setObjectName(pathTrack + trackName);

    CreateMenu();
    createInfo(trackName);
    m_playlists << "Default";
    emit AddTracktoPlaylist(m_playlistName, m_fileInfo);
}

PlayerlistItem::PlayerlistItem(QListWidgetItem *parentItem, FileTags *song, QWidget *parent) :
    QWidget(parent), ui(new Ui::PlayerlistItem), m_pathToTrack(song->tags.path), m_trackName(song->tags.filename),
m_fileInfo(song), m_parent(parentItem) {
    ui->setupUi(this);
    setObjectName(song->tags.path + song->tags.filename);
    qDebug() << song->tags.path;
    CreateMenu();
    createInfo(song->tags.filename);
}

void PlayerlistItem::CreateMenu() {
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

void PlayerlistItem::createInfo(const QString &trackName) {
    QString name(trackName.begin(), trackName.lastIndexOf("."));
    ui->trackNameAndArtist->setText(name);
    ui->infoAboutTrack->setText(
        getFormat() + "::" + m_fileInfo->tags.bitrate + "::" + m_fileInfo->tags.channels
            + "::" + m_fileInfo->tags.sampleRate);

    ui->duration->setText(m_fileInfo->tags.length);
}

PlayerlistItem::~PlayerlistItem() {
    for (auto *item : ui->pushButton->menu()->actions()) {
        delete item;
    }
    delete ui->pushButton->menu();
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
    qDebug() << m_playlistName;
    emit RemoveTrackFromPlaylist(m_fileInfo, m_pathToTrack + m_trackName);
}

void PlayerlistItem::addToPlaylist() {
    qDebug() << "add to playlist";
    QString playlist = QInputDialog::getItem(this, tr("Add track to..."), tr("Choose playlist:"), m_playlists);
    qDebug() << playlist;
    emit AddTracktoPlaylist(playlist, m_fileInfo);
}
void PlayerlistItem::updateListPlaylist(QStringList playlists) {
    m_playlists.clear();
    for (const auto &item : playlists) {
        m_playlists << item;
//        qDebug() << item;
    }
}



