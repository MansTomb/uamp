#include <QDir>
#include <QSqlQuery>
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
                   "countTracks int,"
                   "pathToPlaylist VARCHAR(255));");

        //create songs table
        query.exec("create table if not exists songs (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "songsName VARCHAR(255) NOT NULL," //artist + title
                   "filename VARCHAR(255) NOT NULL,"
                   "pathToFile VARCHAR(255) NOT NULL,"
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

        //create user_playlists table
        query.exec("create table if not exists user_playlists (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "login VARCHAR(255) NOT NULL,"
                   "path VARCHAR(255),"
                   "FOREIGN KEY (path) REFERENCES playlists(pathToPlaylist),"
                   "FOREIGN KEY (login) REFERENCES users(login));");

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
    SqlDatabase::instance().setLogin(login);
    addNewPlaylist("Default");
}

void SqlDatabase::addInfoAboutSong(FileTags *tag, const QString &name, const QString &path) {
    qDebug() << "add new song to db";
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    query.prepare("INSERT INTO songs(songsName, filename, pathToFile, title, artist, album, genre, year,"
                  "trackNumber, bitrate, sampleRate, channels, length, lyrics, picture) "
                  "VALUES(:songsName, :filename, :pathToFile, :title, :artist, :album, :genre, :year,"
                  ":trackNumber, :bitrate, :sampleRate, :channels, :length, :lyrics, :picture)");
    query.bindValue(":songsName", tag->tags.artist + tag->tags.title);
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

    addSongNameToSongInfo(tag->tags.artist + tag->tags.title, qApp->applicationDirPath()
                            + "/app/res/playlist/" + name + "_" + path + ".m3u");
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
    QString path = qApp->applicationDirPath() + "/app/res/playlists/" + m_login + "_" + playlistName + ".m3u";

    query.prepare("INSERT INTO playlists(name, countTracks, pathToPlaylist) VALUES(:name, :countTracks, :pathToPlaylist)");
    query.bindValue(":name", playlistName);
    query.bindValue(":countTracks", 0);
    query.bindValue(":pathToPlaylist", path);
    query.exec();

    query.clear();

    query.prepare("INSERT INTO user_playlists(login, path) VALUES(:login, :path)");
    query.bindValue(":login", m_login);
    query.bindValue(":path", path);
    query.exec();
}

void SqlDatabase::setLogin(const QString &login) {
    m_login = login;
}

void SqlDatabase::addNewUsersGoogle(const QString &userName) {
    qDebug() << "add user from google";
    QSqlQuery query(QSqlDatabase::database(PATHTODB));
    char command[1024];
    qDebug() << "Google: " + getLogin(userName);
    if (userName != getLogin(userName)) {
        std::sprintf(command,
                     R"(INSERT INTO users(login, password) VALUES("%s", "%llu"))",
                     userName.toStdString().c_str(),
                     getRandom(1000000, 10000000));
        query.exec(command);
    }
    SqlDatabase::instance().setLogin(userName);
    addNewPlaylist("Default");
}

