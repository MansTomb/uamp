#pragma once

#define PATHTOPL ((dynamic_cast<QApplication *>(QCoreApplication::instance()))->applicationDirPath() + "/app/res/playlists/")
#define PATHTODB ((dynamic_cast<QApplication *>(QCoreApplication::instance()))->applicationDirPath() + "/app/res/db/uamp.db")

#include <iostream>
#include <random>
#include <limits>

#include <QSqlDatabase.h>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QApplication>


#include "FileTag.h"

typedef unsigned long long int UINT64;

class SqlDatabase {
public:
    SqlDatabase(SqlDatabase &other) = delete;
    void operator=(const SqlDatabase &other) = delete;

    static SqlDatabase &instance();

    void connectDataBase();
    void setLogin(const QString &login);
    void addUserToDataBase(const QString& login, const QString& pass);
    void addInfoAboutSong(FileTags *tag, const QString &name, const QString &path);
    void addSongNameToSongInfo(const QString& name, const QString& path);
    void addNewPlaylist(const QString& playlistName);
    void addNewUsersGoogle(const QString& userName);

    bool CheckCredentials(const QString& login, const QString& pass);

    [[nodiscard]] QString getLogin(const QString& login) const;
    ~SqlDatabase();
private:
    SqlDatabase() {};

    QString m_appPath;
    QString m_toDbPath;
    QString m_login;
    QSqlDatabase *m_db;

    void createTables();
    UINT64 getRandom(const UINT64 &begin = 1000000, const UINT64 &end = 10000000000);
};
