#pragma once

#include <QListWidget>
#include <QDragEnterEvent>

#include "playerlistitem.h"

class PlaylistView : public QListWidget {
 Q_OBJECT
 public:
    explicit PlaylistView(QWidget *parent = Q_NULLPTR);

 protected:
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
