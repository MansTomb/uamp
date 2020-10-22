#include "playerlistitem.h"
#include "ui_playerlistitem.h"

PlayerlistItem::PlayerlistItem(int number, QWidget *parent) :
    QWidget(parent), ui(new Ui::PlayerlistItem) {
    ui->setupUi(this);

    ui->trackNameAndArtist->setText("track one line");
    ui->infoAboutTrack->setText("more info");
    ui->duration->setText("3:34");
}

PlayerlistItem::~PlayerlistItem() {
    delete ui;
}
