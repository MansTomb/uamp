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

/*
 * void MainWindow::setLyrics(std::string songText) {
    if (!m_songPath.empty()) {
        TagLib::MPEG::File file(m_songPath.c_str());
        TagLib::ID3v2::FrameList frames = file.ID3v2Tag()->frameListMap()["USLT"];
        auto *frame = new TagLib::ID3v2::UnsynchronizedLyricsFrame;

        if (!file.ID3v2Tag()->frameListMap()["USLT"].isEmpty()) {
            file.ID3v2Tag()->removeFrames(file.ID3v2Tag()->frameListMap()["USLT"].front()->frameID());
        }
        frame->setText(songText);
        file.ID3v2Tag()->addFrame(frame);
        file.save();
    } else {
        QMessageBox::warning(this, "Warning", "Select a one of files in the main window!");
    }
}



std::string MainWindow::getLyrics(std::string path) {
    TagLib::String lyrics;
    TagLib::MPEG::File file(path.c_str());
    TagLib::ID3v2::FrameList frames = file.ID3v2Tag()->frameListMap()["USLT"];
    TagLib::ID3v2::UnsynchronizedLyricsFrame *frame = NULL;

    if (!frames.isEmpty()) {
        frame = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frames.front());
        if (frame)
            lyrics = frame->text();
    }
    return std::string(lyrics.toCString());
}

void MainWindow::on_saveLyrics_clicked() {
    if (!m_songPath.empty()) {
        std::string text = m_ui->lyrics->toPlainText().toStdString();
        if (!text.empty()) {
            setLyrics(text);
            m_ui->textBrowser->insertPlainText(QTime::currentTime().toString() + " : Lyrics set!\n");
        } else {
            QMessageBox::StandardButton but = QMessageBox::question(this, "Save", "Do you want to save empty lyrics?",
                                                                QMessageBox::Yes | QMessageBox::No);
            if (but == QMessageBox::Yes) {
                setLyrics(text);
                m_ui->textBrowser->insertPlainText(QTime::currentTime().toString() + " : Lyrics set!\n");
            } else {
                QMessageBox::about(this, "Information", "Empty lyrics wasn`t saved to file!");
            }
        }
        m_ui->lyrics->setPlainText("");
    } else {
        QMessageBox::warning(this, "Warning", "Select a one of files in the main window!");
    }
}
 void MainWindow::setImage(const char *file_path, const char *image_path) {
    TagLib::MPEG::File mpegFile(file_path);
    TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
    TagLib::ID3v2::AttachedPictureFrame *frame = 0;
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

void MainWindow::getImage(std::string path) {
    TagLib::MPEG::File file(path.c_str());
    TagLib::ID3v2::Tag *m_tag = file.ID3v2Tag(true);
    TagLib::ID3v2::FrameList frameList = m_tag->frameList("APIC");

    if(frameList.isEmpty()) {
        m_ui->pictureLabel->setPixmap(m_picture->scaled(m_ui->pictureLabel->width(),
                                                        m_ui->pictureLabel->height(), Qt::KeepAspectRatio));
        return ;
    }
    auto *coverImg = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());

    QImage coverQImg;
    coverQImg.loadFromData((const uchar *)coverImg->picture().data(),
                            coverImg->picture().size());
    QPixmap pic = QPixmap::fromImage(coverQImg);
    m_ui->pictureLabel->setPixmap(pic.scaled(m_ui->pictureLabel->width(),
                                             m_ui->pictureLabel->height(), Qt::KeepAspectRatio));
}

}
 */
