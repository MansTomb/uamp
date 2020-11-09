#include <QDir>
#include <QSqlQuery>
#include "SqlDatabase.h"

void SqlDatabase::createTables() {
    QSqlQuery query(QSqlDatabase::database(m_toDbPath));

    if (!query.exec("SELECT * FROM users")) {
        //create users table
        query.exec("create table if not exists users (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "login VARCHAR(255) NOT NULL UNIQUE,"
                   "password VARCHAR(255));");
        query.exec(R"(INSERT INTO users(login, password) VALUES ("mmasniy", ""))");
        query.exec(R"(INSERT INTO users(login, password) VALUES ("abalabin", ""))");
        query.exec(R"(INSERT INTO users(login, password) VALUES ("", ""))");

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

    //здесь баг, когда пустые поля, оно какого-то чуда заходит в проложение!!
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
}

