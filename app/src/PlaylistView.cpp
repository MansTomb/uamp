#include "PlaylistView.h"

PlaylistView::PlaylistView(QWidget *parent) : QListWidget(parent) {
    for (int i = 1; i < 10; ++i) {
        auto *item = new QListWidgetItem;
        addItem(item);
        item->setSizeHint(QSize(50,75));
        setItemWidget(item, new PlayerlistItem(i));
    }
}

void PlaylistView::dragMoveEvent(QDragMoveEvent *event) {
}

void PlaylistView::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void PlaylistView::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void PlaylistView::dropEvent(QDropEvent *event) {
    event->accept();
}
