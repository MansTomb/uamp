#pragma once
//
//#include "../../taglib/taglib/tag.h"
//#include <../taglib/fileref.h>
//#include <toolkit/tpropertymap.h>
//#include <mpegfile.h>
//#include <id3v2tag.h>

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
