#include "MenuPlaylistItemView.h"

MenuPlaylistItemView::MenuPlaylistItemView(QString playlistName, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MenuPlaylistItemView) {
    ui->setupUi(this);

    ui->playListname->setText(playlistName);
}

MenuPlaylistItemView::~MenuPlaylistItemView()
{
    delete ui;
}
