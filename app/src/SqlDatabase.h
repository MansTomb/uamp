#pragma once

#define PATHTOPL ((dynamic_cast<QApplication *>(QCoreApplication::instance()))->applicationDirPath() + "/app/res/playlists/")
#define PATHTODB ((dynamic_cast<QApplication *>(QCoreApplication::instance()))->applicationDirPath() + "/app/res/db/uamp.db")

#include <QSqlDatabase.h>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QApplication>

class SqlDatabase {
public:
    SqlDatabase(SqlDatabase &other) = delete;
    void operator=(const SqlDatabase &other) = delete;

    static SqlDatabase &instance();

    void connectDataBase();
    void addUserToDataBase(const QString& login, const QString& pass);

    bool CheckCredentials(const QString& login, const QString& pass);
//    bool CheckInputData(const QString& login, const QString& pass1, const QString& pass2);

//    QSqlQuery &selectFrom(const QString);

    QString getLogin(const QString& login) const;
    ~SqlDatabase();
private:
    SqlDatabase() {};
    QString m_appPath;
    QString m_toDbPath;
    QSqlDatabase *m_db;

    void createTables();
};
