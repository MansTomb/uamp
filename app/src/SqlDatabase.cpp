#include <QBuffer>
#include "SqlDatabase.h"

UINT64 SqlDatabase::getRandom(const UINT64 &begin, const UINT64 &end) {
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<unsigned long long> distr;

    return distr(eng);
};

void SqlDatabase::createTables() {
    QSqlQuery query(QSqlDatabase::database(m_toDbPath));

    if (!query.exec("SELECT * FROM users")) {
        //create users table
        query.exec("create table if not exists users (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "login VARCHAR(255) NOT NULL UNIQUE,"
                   "password VARCHAR(255));");

        //create playlists table

        query.exec("create table if not exists playlists (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name VARCHAR(255) NOT NULL,"
                   "userName VARCHAR(255) NOT NULL,"
                   "countTracks int,"
                   "pathToPlaylist VARCHAR(255),"
                   "image BLOB,"
                   "FOREIGN KEY (userName) REFERENCES users(login));");

        //create songs table
        query.exec("create table if not exists songs (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "songsName VARCHAR(255) NOT NULL," //artist + title
                   "filename VARCHAR(255) NOT NULL,"
                   "pathToFile VARCHAR(255) NOT NULL UNIQUE,"
                   "title VARCHAR(255) NOT NULL,"
                   "artist VARCHAR(255) NOT NULL,"
                   "album VARCHAR(255) NOT NULL,"
                   "genre VARCHAR(255) NOT NULL,"
                   "year INTEGER,"
                   "trackNumber INTEGER,"
                   "bitrate INTEGER,"
                   "sampleRate INTEGER,"
                   "channels INTEGER,"
                   "length VARCHAR(255) NOT NULL,"
                   "lyrics VARCHAR(5120),"
                   "picture BLOB);");

        //create songs_info table
        query.exec("create table if not exists songs_info (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "pathToTrack VARCHAR(255) NOT NULL,"
                   "pathToPlaylist VARCHAR(255),"
                   "FOREIGN KEY (pathToTrack) REFERENCES songs(songsName),"
                   "FOREIGN KEY (pathToPlaylist) REFERENCES playlists(pathToPlaylist));");

        query.exec("create table if not exists equalizer (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "login VARCHAR(255) NOT NULL,"
                   "namePreset VARCHAR(255),"
                   "gain INTEGER,"
                   "echo INTEGER,"
                   "distortion INTEGER,"
                   "compressor INTEGER,"
                   "chorus INTEGER,"
                   "FOREIGN KEY (login) REFERENCES users(login));");
    } else {
        qDebug() << "Opening existing Db";
    }
}
void SqlDatabase::createDefaultPreset() {
    addPreset("Default", {
        {"gain", 0},
        {"echo", 50},
        {"distortion", 15},
        {"compressor", 10},
        {"chorus", 10}
    });
    addPreset("Bleed", {
        {"gain", 5},
        {"echo", 75},
        {"distortion", 50},
        {"compressor", 30},
        {"chorus", 20}
    });
    addPreset("Death", {
        {"gain", 15},
        {"echo", 100},
        {"distortion", 100},
        {"compressor", 500},
        {"chorus", 99}
    });
    addPreset("Living", {
        {"gain", -15},
        {"echo", 0},
        {"distortion", 0},
        {"compressor", 0},
        {"chorus", 0}
    });
    addPreset("???", {
        {"gain", 15},
        {"echo", 0},
        {"distortion", 100},
        {"compressor", 0},
        {"chorus", 0}
    });
}

SqlDatabase &SqlDatabase::instance() {
    static SqlDatabase sql;
    return sql;
}

void SqlDatabase::connectDataBase() {
    m_appPath = qApp->applicationDirPath();
    m_toDbPath = m_appPath + "/app/res/db/uamp.db";
    m_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", m_toDbPath));

    m_db->setDatabaseName(m_toDbPath);
    m_db->setUserName("mmasniy");
    m_db->setHostName("mac");
    m_db->setPassword("pas");

    if (!m_db->open()) {
        qDebug() << "Db opening failed";
        exit(1);
    }
    createTables();
}

bool SqlDatabase::CheckCredentials(const QString& login, const QString& pass) {
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    char command[1024];

    //здесь баг, когда пустые поля, оно какого-то чуда заходит в приложение!!
    //убрать коммент перед релизом!!
//    if (login.isEmpty() && pass.isEmpty())
//        return false;

    std::sprintf(command, R"(SELECT login, password FROM users WHERE users.login = "%s" AND users.password = "%s")",
                 login.toStdString().c_str(), pass.toStdString().c_str());
    query.exec(command);
    query.first();

    if (login == query.value(0).toString() && pass == query.value(1).toString()) {
        m_login = login;
        return true;
    }
    qDebug() << "login: " + query.value(0).toString() + " | pass: " + query.value(1).toString();
    return false;
}

QString SqlDatabase::getLogin(const QString& login) const {
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    char command[1024];

    std::sprintf(command, "SELECT login FROM users WHERE users.login=\"%s\"", login.toStdString().c_str());
    query.exec(command);
    query.first();
    qDebug() << "get login " + query.value(0).toString();
    return query.value(0).toString();
}

SqlDatabase::~SqlDatabase() {
    qDebug() << "disconnect BD";
    m_db->close();
}

void SqlDatabase::addUserToDataBase(const QString& login, const QString& pass) {
    qDebug() << "add user " + login;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    char command[1024];

    std::sprintf(command, R"(INSERT INTO users(login, password) VALUES("%s", "%s"))",
                 login.toStdString().c_str(), pass.toStdString().c_str());
    query.exec(command);
    m_login = login;
    addNewPlaylist("Default", QPixmap());
}

void SqlDatabase::addInfoAboutSong(FileTags *tag, const QString &name, const QString &path) {
    qDebug() << "add new song to db";
    if (tag) {
        QSqlQuery query(QSqlDatabase::database(PATHTODB));
        query.prepare(
            "INSERT INTO songs(songsName, filename, pathToFile, title, artist, album, genre, year,"
            "trackNumber, bitrate, sampleRate, channels, length, lyrics, picture) "
            "VALUES(:songsName, :filename, :pathToFile, :title, :artist, :album, :genre, :year,"
            ":trackNumber, :bitrate, :sampleRate, :channels, :length, :lyrics, :picture)");
        query.bindValue(":songsName",
                        tag->tags.artist + " - " + tag->tags.title);
        query.bindValue(":filename", tag->tags.filename);
        query.bindValue(":pathToFile", tag->tags.path);
        query.bindValue(":title", tag->tags.title);
        query.bindValue(":artist", tag->tags.artist);
        query.bindValue(":album", tag->tags.album);
        query.bindValue(":genre", tag->tags.genre);
        query.bindValue(":year", tag->tags.year);
        query.bindValue(":trackNumber", tag->tags.trackNumber);
        query.bindValue(":bitrate", tag->tags.bitrate);
        query.bindValue(":sampleRate", tag->tags.sampleRate);
        query.bindValue(":channels", tag->tags.channels);
        query.bindValue(":length", tag->tags.length);
        query.bindValue(":lyrics", tag->tags.lyrics);
        query.bindValue(":picture", tag->tags.picture);
        query.exec();

        addSongNameToSongInfo(tag->tags.path,
                              qApp->applicationDirPath()
                                  + "/app/res/playlists/" + m_login + "_" + path
                                  + ".m3u");
    } else {
        qDebug() << "File does not open!!";
    }
}

void SqlDatabase::addSongNameToSongInfo(const QString &pathToTrack, const QString &pathToPlaylist) {
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    query.prepare("INSERT INTO songs_info(pathToTrack, pathToPlaylist) VALUES(:track, :playlist)");
    query.bindValue(":track", pathToTrack);
    query.bindValue(":playlist", pathToPlaylist);
    query.exec();
}

void SqlDatabase::addNewPlaylist(const QString &playlistName, QPixmap picture) {
    qDebug() << "add new playlist: " + playlistName;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";

    query.prepare("INSERT INTO playlists(name, userName, countTracks, pathToPlaylist, image) VALUES(:name, :userName,"
                  ":countTracks, :pathToPlaylist, :image)");
    query.bindValue(":name", playlistName);
    query.bindValue(":userName", m_login);
    query.bindValue(":countTracks", 0);
    query.bindValue(":pathToPlaylist", path);
    QByteArray byte;
    QBuffer buffer(&byte);
    buffer.open(QIODevice::WriteOnly);
    picture.save(&buffer, "PNG");
    if (byte.isEmpty()) {
        qDebug() << "image do not add in db!";
    }
    query.bindValue(":image", byte);

    query.exec();
}

void SqlDatabase::setLogin(const QString &login) {
    m_login = login;
}

void SqlDatabase::addNewUsersGoogle(const QString &userName) {
    m_login = userName;
    if (userName != getLogin(userName)) {
        qDebug() << "add user from google";
        addUserToDataBase(userName.toStdString().c_str(), QString::number(getRandom(1000000, 10000000)));
    }
    qDebug() << m_login;
}

void SqlDatabase::deletePlaylist(const QString &playlistName) {
    qDebug() << "delete playlist: " + playlistName;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";

    query.prepare("DELETE FROM playlists WHERE playlists.name=(:playlistName) AND playlists.pathToPlaylist=(:path)");
    query.bindValue(":playlistName", playlistName);
    query.bindValue(":path", path);
    query.exec();
}

QStringList SqlDatabase::getAllPlaylist() const {
    qDebug() << "get ALL Playlist!";
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QStringList values;

    query.prepare("SELECT name FROM playlists WHERE playlists.userName=(:login)");
    qDebug() << m_login;
    query.bindValue(":login", m_login);
    query.exec();

    for (; query.next();) {
        values << query.value("name").toString();
    }
    qDebug() << values;
    return values;
}

void SqlDatabase::renamePlaylist(const QString &oldName, const QString &newName) {
    QSqlQuery query(QSqlDatabase::database(PATHTODB));

    qDebug() << "rename playlist from: " + oldName + " to: " + newName;

    query.prepare("UPDATE playlists SET name=:new WHERE name=:old");
    query.bindValue(":new", newName);
    query.bindValue(":old", oldName);
    query.exec();
}

void SqlDatabase::deleteTrackFromPlaylist(const QString &pathToTrack, const QString &playlistName) {
    qDebug() << "delete song: " + pathToTrack + "\n from playlist:" + playlistName;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";

    query.prepare("DELETE FROM songs_info WHERE songs_info.pathToTrack=(:song) AND songs_info.pathToPlaylist=(:path)");
    query.bindValue(":song", pathToTrack);
    query.bindValue(":path", path);
    query.exec();
}

QList<FileTags *> SqlDatabase::getAllTracksFromPlaylist(const QString &playlistName) {
    qDebug() << "\ngetAllTracksFromPlaylist!!!\n================================\n";

    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";
    QList<FileTags *> tracks;

    query.prepare("SELECT * FROM songs INNER JOIN songs_info ON "
                  "songs_info.pathToTrack=songs.pathToFile AND songs_info.pathToPlaylist=(:path)");
    query.bindValue(":path", path);
    query.exec();
    while(query.next()) {
        if (QFileInfo::exists(query.value(3).toString())) {
            auto song = new FileTags(query);
            tracks.push_back(song);
        } else {
            deleteTrackFromPlaylist(query.value(3).toString(), playlistName);
            if (playlistName != "Default") {
                deleteTrackFromPlaylist(query.value(3).toString(), "Default");
            }
        }
    }
    return tracks;
}

void SqlDatabase::addPreset(const QString &preset, const QMap<QString, int> &presets) {
    qDebug() << "\naddPreset\n";
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    query.prepare("INSERT INTO equalizer(login, namePreset, gain, echo, distortion, compressor, chorus)"
                  "VALUES(:login, :namePreset, :gain, :echo, :distortion, :compressor, :chorus)");
    query.bindValue(":login", m_login);
    query.bindValue(":namePreset", preset);
    query.bindValue(":gain", presets["gain"]);
    query.bindValue(":echo", presets["echo"]);
    query.bindValue(":distortion", presets["distortion"]);
    query.bindValue(":compressor", presets["compressor"]);
    query.bindValue(":chorus", presets["chorus"]);
    query.exec();
}

QMap<QString, QMap<QString, int>> SqlDatabase::getPreset() {
    QMap<QString, QMap<QString, int>> presets;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));

    query.prepare("SELECT * FROM equalizer WHERE equalizer.login=(:login)");
    query.bindValue(":login", m_login);
    query.exec();

    for (;query.next();) {
        presets[query.value(2).toString()] = {
            {"gain", query.value(3).toInt()},
            {"echo", query.value(4).toInt()},
            {"distortion", query.value(5).toInt()},
            {"compressor", query.value(6).toInt()},
            {"chorus", query.value(7).toInt()}
        };
    }

    return presets;
}

void SqlDatabase::clearPlaylist(const QString &playlistName) {
    qDebug() << "clear playlist:" + playlistName;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";

    query.prepare("DELETE FROM songs_info WHERE songs_info.pathToPlaylist=(:path)");
    query.bindValue(":path", path);
    query.exec();
}

QStringList SqlDatabase::getAllTracksFromDefault() const {
    qDebug() << "Tracks from DEFAULT!\n";
    QStringList setOfPaths;
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_Default.m3u";
    QSqlQuery query(QSqlDatabase::database(PATHTODB));

    query.prepare("SELECT * FROM songs_info WHERE songs_info.pathToPlaylist=(:path)");
    query.bindValue(":path", path);
    query.exec();

    for (;query.next();) {
        qDebug() << query.value(1).toString();
        setOfPaths << query.value(1).toString();
    }
    qDebug() << setOfPaths;
    qDebug() << "============";
    return setOfPaths;
}

