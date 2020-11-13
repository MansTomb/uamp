#pragma once

#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <string>
#include <map>
#include <fstream>

#include "../../taglib/taglib/tag.h"
#include <../taglib/fileref.h>
#include <toolkit/tpropertymap.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <QString>
#include <QDebug>
#include <QPixmap>
#include <QSqlQuery>

#include "ImageFile.h"

class FileTags {

    public:
    struct  Tags{
        QString filename;
        QString path;
        QString title;
        QString artist;
        QString album;
        QString genre;
        QString year;
        QString trackNumber;
        QString bitrate;
        QString sampleRate;
        QString channels;
        QString length;
        QString lyrics;
        QString picture;
    } tags;

    FileTags(const std::string &path, const std::string &fileName);
    explicit FileTags(const QSqlQuery& query);
    void upgradeFileTags(const std::string &new_tags);
    friend std::ostream& operator<<(std::ostream& out, const FileTags& file);
    void setLyrics();
    void getLyrics();
    void setImage(const char *image_path);
    QPixmap *getImage();
    void setM_picture(QPixmap *picture);

public:
    signals:


    private:
    TagLib::FileRef m_fileRef;
    QPixmap *m_picture = nullptr;
};
