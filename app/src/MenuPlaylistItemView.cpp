#include "MenuPlaylistItemView.h"

MenuPlaylistItemView::MenuPlaylistItemView(QString playlistName,
                                           QListWidgetItem *wItem,
                                           QWidget *parent) :
        m_name(playlistName),
        m_parent(wItem),
        QWidget(parent),
        ui(new Ui::MenuPlaylistItemView) {
    ui->setupUi(this);
    m_playlist = loadPlaylist();

    setObjectName(m_name);
    ui->playListname->setText(m_name);
}

MenuPlaylistItemView::~MenuPlaylistItemView() {
    delete ui;
}

QList<FileTags *> MenuPlaylistItemView::loadPlaylist() {
    return QList<FileTags *>();
}
