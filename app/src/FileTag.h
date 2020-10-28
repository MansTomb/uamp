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
    } tags;

    FileTags(const std::string &path, const std::string &fileName);
    void upgradeFileTags(const std::string &new_tags);
    friend std::ostream& operator<<(std::ostream& out, const FileTags& file);

    private:
    TagLib::FileRef m_fileRef;
};
