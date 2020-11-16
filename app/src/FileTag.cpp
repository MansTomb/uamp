#include <QMessageBox>
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
        tags.year = QString::number(tag->year());
        tags.trackNumber = QString::number(tag->track());
        getLyrics();
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
                        << file.tags.genre << "\n"
                        << file.tags.year << "\n"
                        << file.tags.trackNumber;
    return out;
}

void FileTags::upgradeFileTags(const std::string &new_tags) {
    TagLib::FileRef file(new_tags.c_str());

    file.tag()->setTitle(tags.title.toStdString());
    file.tag()->setArtist(tags.artist.toStdString());
    file.tag()->setAlbum(tags.album.toStdString());
    file.tag()->setGenre(tags.genre.toStdString());
    file.tag()->setYear(tags.year.toInt());
    file.tag()->setTrack(tags.trackNumber.toInt());

    file.save();
}

QPixmap *FileTags::getImage() {
    TagLib::MPEG::File file(tags.path.toStdString().c_str());
    TagLib::ID3v2::Tag *m_tag = file.ID3v2Tag(true);
    TagLib::ID3v2::FrameList frameList = m_tag->frameList("APIC");

    if(frameList.isEmpty()) {
        return nullptr;
    }
    auto *coverImg = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());

    QImage coverQImg;
    coverQImg.loadFromData((const uchar *)coverImg->picture().data(),
                           coverImg->picture().size());
    m_picture = new QPixmap(QPixmap::fromImage(coverQImg));
    return m_picture;
}

void FileTags::setImage(const char *image_path) {
    TagLib::MPEG::File mpegFile(tags.path.toStdString().c_str());
    TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
    TagLib::ID3v2::AttachedPictureFrame *frame = nullptr;

    if(frames.isEmpty()) {
        frame = new TagLib::ID3v2::AttachedPictureFrame;
        tag->addFrame(frame);
    }
    else {
        frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
    }

    ImageFile imageFile(image_path);
    TagLib::ByteVector imageData = imageFile.data();
    frame->setMimeType("image/jpeg");
    frame->setPicture(imageData);
    mpegFile.save();

}

void FileTags::setM_picture(QPixmap *picture) {
    m_picture = picture;
}

void FileTags::getLyrics() {
    TagLib::String lyrics;
    TagLib::MPEG::File file(tags.path.toStdString().c_str());
    TagLib::ID3v2::FrameList frames = file.ID3v2Tag()->frameListMap()["USLT"];
    TagLib::ID3v2::UnsynchronizedLyricsFrame *frame = NULL;

    if (!frames.isEmpty()) {
        frame = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frames.front());
        if (frame)
            lyrics = frame->text();
    }
    tags.lyrics = QString(lyrics.toCString());
}

void FileTags::setLyrics() {
    if (!tags.path.toStdString().empty()) {
        TagLib::MPEG::File file(tags.path.toStdString().c_str());
        TagLib::ID3v2::FrameList
            frames = file.ID3v2Tag()->frameListMap()["USLT"];
        auto *frame = new TagLib::ID3v2::UnsynchronizedLyricsFrame;

        if (!file.ID3v2Tag()->frameListMap()["USLT"].isEmpty()) {
            file.ID3v2Tag()->removeFrames(file.ID3v2Tag()->frameListMap()["USLT"].front()->frameID());
        }
        frame->setText(tags.lyrics.toStdString());
        file.ID3v2Tag()->addFrame(frame);
        file.save();
    }
}

FileTags::FileTags(const QSqlQuery &query) {
    tags.filename = query.value(2).toString();
    tags.path = query.value(3).toString();
    tags.title = query.value(4).toString();
    tags.artist = query.value(5).toString();
    tags.album = query.value(6).toString();
    tags.genre = query.value(7).toString();
    tags.year = query.value(8).toString();
    tags.trackNumber = query.value(9).toString();
    tags.bitrate = query.value(10).toString();
    tags.sampleRate = query.value(11).toString();
    tags.channels = query.value(12).toString();
    tags.length = query.value(13).toString();
    tags.lyrics = query.value(14).toString();
    tags.picture = query.value(15).toString();
}
