#include <QDir>
#include <QSqlQuery>
#include "SqlDatabase.h"

SqlDatabase::SqlDatabase() :
    m_appPath(qApp->applicationDirPath()), m_toDbPath(m_appPath + "/app/res/db/uamp.db"),
    m_db(QSqlDatabase::addDatabase("QSQLITE", m_toDbPath)) {

    m_db.setDatabaseName(m_toDbPath);
    m_db.setUserName("mmasniy");
    m_db.setHostName("mac");
    m_db.setPassword("pas");

    if (!m_db.open()) {
        qDebug() << "Db opening failed";
        exit(1);
    }
    createTables();
}

void SqlDatabase::createTables() {
    QSqlQuery query(QSqlDatabase::database(m_toDbPath));

    if (!query.exec("SELECT * FROM users")) {
        query.exec("create table if not exists users (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "login varchar(255) NOT NULL UNIQUE,"
                   "password varchar(255));");
        query.exec(R"(INSERT INTO users(login, password) VALUES ("mmasniy", ""))");
        query.exec(R"(INSERT INTO users(login, password) VALUES ("abalabin", ""))");
        query.exec(R"(INSERT INTO users(login, password) VALUES ("", ""))");
    } else {
        qDebug() << "Opening existing Db";
    }
}
