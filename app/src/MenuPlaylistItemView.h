#pragma once

#include <QWidget>

#include "ui_MenuPlaylistItemView.h"

namespace Ui {
class MenuPlaylistItemView;
}

class MenuPlaylistItemView : public QWidget
{
    Q_OBJECT
public:
    explicit MenuPlaylistItemView(QString playlistName, QWidget *parent = nullptr);
    ~MenuPlaylistItemView();

private:
    Ui::MenuPlaylistItemView *ui;
};
