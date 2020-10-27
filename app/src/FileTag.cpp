#include "FileTag.h"

FileTags::FileTags(QString &path, QString &fileName) {
    QString fullPath(path + fileName);
    tags.filename = fileName;
    tags.path = fullPath;
    m_fileRef = TagLib::FileRef(fullPath.toStdString().c_str());

    if (!m_fileRef.isNull() && m_fileRef.tag()) {
        TagLib::Tag *tag = m_fileRef.tag();

        tags.name = tag->title().toCString();
        tags.artist = tag->artist().toCString();
        tags.album = tag->album().toCString();
        tags.genre = tag->genre().toCString();
        tags.year = QString::number(tag->year());
        tags.trackNumber = QString::number(tag->track());
    }
}

std::ostream &operator<<(std::ostream &out, const FileTags &file) {
        qDebug() << file.tags.filename<< "\n"
                        << file.tags.path << "\n"
                        << file.tags.name << "\n"
                        << file.tags.artist << "\n"
                        << file.tags.album << "\n"
                        << file.tags.genre << "\n"
                        << file.tags.year << "\n"
                        << file.tags.trackNumber;
    return out;
}

void FileTags::upgradeFileTags(const FileTags &new_tags) {
    TagLib::FileRef file(new_tags.tags.path.toStdString().c_str());

    file.tag()->setTitle(new_tags.tags.name.toStdString());
    tags.name = new_tags.tags.name;
    file.tag()->setArtist(new_tags.tags.artist.toStdString());
    tags.artist = new_tags.tags.artist;
    file.tag()->setAlbum(new_tags.tags.album.toStdString());
    tags.album = new_tags.tags.album;
    file.tag()->setGenre(new_tags.tags.genre.toStdString());
    tags.genre = new_tags.tags.genre;
    file.tag()->setYear(new_tags.tags.year.toInt());
    tags.year = new_tags.tags.year;
    file.tag()->setTrack(new_tags.tags.trackNumber.toInt());
    tags.trackNumber = new_tags.tags.trackNumber;
    file.save();
}
