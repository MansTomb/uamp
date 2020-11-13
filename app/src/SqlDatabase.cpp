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
                   "songsName VARCHAR(255) NOT NULL,"
                   "playlist VARCHAR(255),"
                   "FOREIGN KEY (songsName) REFERENCES songs(songsName),"
                   "FOREIGN KEY (playlist) REFERENCES playlists(pathToPlaylist));");

    } else {
        qDebug() << "Opening existing Db";
    }
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
    addNewPlaylist("Default");
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
        query.bindValue(":picture", 0);
        query.exec();

        addSongNameToSongInfo(tag->tags.artist + " - " + tag->tags.title,
                              qApp->applicationDirPath()
                                  + "/app/res/playlists/" + m_login + "_" + path
                                  + ".m3u");
    } else {
        qDebug() << "File does not open!!";
    }
}

void SqlDatabase::addSongNameToSongInfo(const QString &name, const QString &path) {
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    query.prepare("INSERT INTO songs_info(songsName, playlist) VALUES(:songsName, :playlist)");
    query.bindValue(":songsName", name);
    query.bindValue(":playlist", path);
    query.exec();
}

void SqlDatabase::addNewPlaylist(const QString &playlistName) {
    qDebug() << "add new playlist: " + playlistName;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    qDebug() << m_login << "addsdasdsadsad";
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";

    query.prepare("INSERT INTO playlists(name, userName, countTracks, pathToPlaylist) VALUES(:name, :userName, :countTracks, :pathToPlaylist)");
    query.bindValue(":name", playlistName);
    query.bindValue(":userName", m_login);
    query.bindValue(":countTracks", 0);
    query.bindValue(":pathToPlaylist", path);
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

void SqlDatabase::deleteTrackFromPlaylist(const QString &songName, const QString &playlistName) {
    qDebug() << "delete song: " + songName + " from playlist:" + playlistName;
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";

    query.prepare("DELETE FROM songs_info WHERE songs_info.songsName=(:song) AND songs_info.playlist=(:path)");
    query.bindValue(":song", songName);
    query.bindValue(":path", path);
    query.exec();
}

QList<FileTags *>& SqlDatabase::getAllTracksFromPlaylist(const QString &playlistName) {
    qDebug() << "\ngetAllTracksFromPlaylist!!!\n================================\n";

    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";
    QList<FileTags *> tracks;

    query.prepare("SELECT * FROM songs INNER JOIN songs_info ON "
                  "songs_info.songsName=songs.songsName AND songs_info.playlist=(:path)");
    query.bindValue(":path", path);
    query.exec();
    for (;query.next();) {
        tracks.push_back(new FileTags(query));
    }
    return tracks;
}
