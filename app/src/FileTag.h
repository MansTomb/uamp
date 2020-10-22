#pragma once

#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <string>
#include <map>

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
        QString name;
        QString artist;
        QString album;
        QString genre;
        QString year;
        QString trackNumber;
    } tags;

    FileTags(const std::string& path, QString& fileName);
    void upgradeFileTags(const FileTags& new_tags);
    friend std::ostream& operator<<(std::ostream& out, const FileTags& file);

    private:
    TagLib::FileRef m_fileRef;
    std::map<std::string, QString> m_file_tags;
};
