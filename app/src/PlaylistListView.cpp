#include "PlaylistListView.h"
#include "MenuPlaylistItemView.h"

PlaylistListView::PlaylistListView(QWidget *parent) : QWidget(parent),  ui(new Ui::PlaylistsView) {
    ui->setupUi(this);

    for (int i = 1; i < 10; ++i) {
        auto *item = new QListWidgetItem;
        ui->list->addItem(item);
        item->setSizeHint(QSize(50,75));
        ui->list->setItemWidget(item, new MenuPlaylistItemView("New"));
    }
}
