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
#include <QMessageBox>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>

#include "FileTag.h"

typedef unsigned long long int UINT64;

class SqlDatabase {
public:
    SqlDatabase(SqlDatabase &other) = delete;
    void operator=(const SqlDatabase &other) = delete;

    static SqlDatabase &instance();

    void connectDataBase();
    void createDefaultPreset();
    void setLogin(const QString &login);
    void addUserToDataBase(const QString& login, const QString& pass);
    void addInfoAboutSong(FileTags *tag, const QString &name, const QString &path);
    void addSongNameToSongInfo(const QString& name, const QString& path);
    void addNewPlaylist(const QString &playlistName, QPixmap picture);
    void addNewUsersGoogle(const QString& userName);
    void deletePlaylist(const QString &playlistName);
    void clearPlaylist(const QString &playlistName);
    void renamePlaylist(const QString &oldName, const QString &newName);
    void deleteTrackFromPlaylist(const QString &pathToTrack, const QString &playlistName);
    void addPreset(const QString &preset, const QMap<QString, int> &presets);

    QMap<QString, QMap<QString, int>> getPreset();

    bool CheckCredentials(const QString& login, const QString& pass);
//    bool CheckCredentials(const QString& );

    [[nodiscard]] QString getLogin(const QString& login) const;

    [[nodiscard]] QStringList getAllPlaylist() const;
    [[nodiscard]] QStringList getAllTracksFromDefault() const;
    QList<FileTags *> getAllTracksFromPlaylist(const QString &playlistName);
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
