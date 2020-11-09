#pragma once

#define PATHTOPL ((static_cast<QApplication *>(QCoreApplication::instance()))->applicationDirPath() + "/app/res/playlists/")

#include <QSqlDatabase.h>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QApplication>

class SqlDatabase {
    public:
    explicit SqlDatabase();
    ~SqlDatabase();

    private:
    QString m_appPath;
    QString m_toDbPath;
    QSqlDatabase m_db;
    void createTables();
};
