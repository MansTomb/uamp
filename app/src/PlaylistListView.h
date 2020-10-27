#pragma once

#include <QListWidget>
#include <QDebug>

#include "ui_playlistlistview.h"

namespace Ui {
class PlaylistsView;
}

class PlaylistListView : public QWidget
{
    Q_OBJECT
 public:
    explicit PlaylistListView(QWidget *parent = nullptr);

 private:
    Ui::PlaylistsView *ui;
};
