#include "database.h"
#include <QtSql/QSqlError>
#include <QMessageBox>

/*
 * Manages connection to the database
 * Attempts to open the connection; shows a
 * error message box if it cannot connect
 */
Database::Database() {
    // Check if connection already exists (had to look this up had erorrs on multiple sql default connections when i integrated this file
    if (QSqlDatabase::contains("AssetManagerConnection"))
    {
        db = QSqlDatabase::database("AssetManagerConnection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QODBC", "AssetManagerConnection");

        db.setDatabaseName(
            "DRIVER={SQL Server};"
            "Server=itassetmanagerserver.database.windows.net;"
            "Database=IT_AssetManager;"
            "Uid=cs245;"
            "Port=1433;"
            "Pwd=Thomas123;"
            "encrypt=true;"
            "trustServerCertificate=false;"
            "hostNameInCertificate=*.database.windows.net;"
            "loginTimeout=30;"
            );

        if (!db.open())
        {
            QMessageBox::critical(nullptr, "Error", "Failed to connect to the database");
        }
    }
}

// Destructor
Database::~Database()
{
    if(db.isOpen())
    {
        db.close();
    }
}

// Returns reference
QSqlDatabase& Database::getConnection()
{
    return db;
}
