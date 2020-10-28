#include "PlaylistView.h"

PlaylistView::PlaylistView(QWidget *parent) : QListWidget(parent) {
//    setAcceptDrops(true);
//    for (int i = 1; i < 10; ++i) {
//        auto *item = new QListWidgetItem;
//        addItem(item);
//        item->setSizeHint(QSize(50,75));
//        setItemWidget(item, new PlayerlistItem(i));
//    }
}

void PlaylistView::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}

void PlaylistView::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void PlaylistView::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void PlaylistView::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls())
        return;
    auto droppedData = event->mimeData()->urls();
    if (droppedData.count() == 1) {
        QDir dir(droppedData.at(0).toString().remove(0, 7));
        dir.setNameFilters(QStringList() << "*.mp3" << "*.flac" << "*.mp4"
                            << "*.wav" << "*.m4a" << "*.m3u" << "*.ogg");
        auto dirPath(dir.absolutePath() + "/");
        if (!dir.exists())
            return;
        clear();
        for (auto &item : dir.entryList()) {
            qDebug() << item;
            addWidget(dirPath, item);
        }
    }
}

void PlaylistView::addWidget(const QString &pathTrack, const QString &trackName) {
    auto *item = new QListWidgetItem;
    addItem(item);
    item->setSizeHint(QSize(50,75));
    setItemWidget(item, new PlayerlistItem(count(), pathTrack, trackName));
}

