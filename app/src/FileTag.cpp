#include "FileTag.h"

FileTags::FileTags(const std::string &path, const std::string &fileName) {
    std::string fullPath(path + fileName);
    tags.filename = QString::fromStdString(fileName);
    tags.path = QString::fromStdString(fullPath);
    m_fileRef = TagLib::FileRef(fullPath.c_str());

    if (!m_fileRef.isNull() && m_fileRef.tag()) {
        TagLib::Tag *tag = m_fileRef.tag();

        tags.title = tag->title().toCString();
        tags.artist = tag->artist().toCString();
        tags.album = tag->album().toCString();
        tags.genre = tag->genre().toCString();
//        tags.year = QString::number(tag->year());
//        tags.trackNumber = QString::number(tag->track());
    }

    if(!m_fileRef.isNull() && m_fileRef.audioProperties()) {
        TagLib::AudioProperties *properties = m_fileRef.audioProperties();
        int seconds = properties->length() % 60;
        int minutes = (properties->length() - seconds) / 60;

        tags.bitrate = QString::number(properties->bitrate());
        tags.sampleRate = QString::number(properties->sampleRate());
        tags.channels = QString::number(properties->channels());
        tags.length = QString::number(minutes) + ":" + (seconds != 0 ? QString::number(seconds) : "00");
    }
}

std::ostream &operator<<(std::ostream &out, const FileTags &file) {
        qDebug() << file.tags.filename<< "\n"
                        << file.tags.path << "\n"
                        << file.tags.title << "\n"
                        << file.tags.artist << "\n"
                        << file.tags.album << "\n"
                        << file.tags.genre << "\n";
//                        << file.tags.year << "\n"
//                        << file.tags.trackNumber;
    return out;
}

void FileTags::upgradeFileTags(const std::string &new_tags) {
    TagLib::FileRef file(new_tags.c_str());

    file.tag()->setTitle(tags.title.toStdString());
    file.tag()->setArtist(tags.artist.toStdString());
    file.tag()->setAlbum(tags.album.toStdString());
    file.tag()->setGenre(tags.genre.toStdString());
//    file.tag()->setYear(tags.year.toInt());
//    file.tag()->setTrack(tags.trackNumber.toInt());

    file.save();
}
