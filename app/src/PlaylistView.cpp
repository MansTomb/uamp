#include "PlaylistView.h"

PlaylistView::PlaylistView(QWidget *parent) : QListWidget(parent) {
    for (int i = 1; i < 10; ++i) {
        auto *item = new QListWidgetItem;
        addItem(item);
        item->setSizeHint(QSize(50,75));
        setItemWidget(item, new PlayerlistItem(i));
    }
}
