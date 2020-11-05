#include <QDir>
#include <QSqlQuery>
#include "SqlDatabase.h"

SqlDatabase::SqlDatabase() :
    m_appPath(qApp->applicationDirPath()), m_toDbPath(m_appPath + "/app/res/db/uamp.db"),
    m_db(QSqlDatabase::addDatabase("QSQLITE", m_toDbPath)) {

    m_db.setDatabaseName(m_toDbPath);
    m_db.setUserName("mmasniy");
    m_db.setHostName("mac");
    m_db.setPassword("pis'ka_za_schekoy");

    if (!m_db.open()) {
        qDebug() << "Db opening failed";
        exit(1);
    }
    createTables();
}

void SqlDatabase::createTables() {
    QSqlQuery query(QSqlDatabase::database("uamp.db"));

}
