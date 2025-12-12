#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>

class Database {
public:
    Database();
    ~Database();

    QSqlDatabase& getConnection();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H

