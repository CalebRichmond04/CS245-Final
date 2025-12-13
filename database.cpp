#include "database.h"
#include <QtSql/QSqlError>
#include <QMessageBox>

/*
 * Manages connection to the database
 * Attempts to open the connection; shows a
 * error message box if it cannot connect
 */
Database::Database() {
    // Check if connection already exists
    // Prevents multiple default connection errors when integrating this file
    if (QSqlDatabase::contains("AssetManagerConnection"))
    {
        // Reuse existing connection
        db = QSqlDatabase::database("AssetManagerConnection");
    }
    else
    {
        // Create a new ODBC connection with a unique name
        db = QSqlDatabase::addDatabase("QODBC", "AssetManagerConnection");

        // Set up connection string with SQL Server credentials
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

        // Attempt to open the database
        if (!db.open())
        {
            // Show error if connection fails
            QMessageBox::critical(nullptr, "Error", "Failed to connect to the database");
        }
    }
}

// Destructor
Database::~Database()
{
    // Close the database connection if open
    if(db.isOpen())
    {
        db.close();
    }
}

// Returns reference to the centralized database connection
QSqlDatabase& Database::getConnection()
{
    return db;
}
