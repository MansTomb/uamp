#pragma once
//
//#include "../../taglib/taglib/tag.h"
//#include <../taglib/fileref.h>
//#include <toolkit/tpropertymap.h>
//#include <mpegfile.h>
//#include <id3v2tag.h>

#include <QListWidget>

#include "playerlistitem.h"

class PlaylistView : public QListWidget {
    Q_OBJECT
    public:
    explicit PlaylistView(QWidget *parent = Q_NULLPTR);
};
