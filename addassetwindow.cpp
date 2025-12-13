#include "addassetwindow.h"
#include "ui_addassetwindow.h"
#include "database.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <iostream>
#include <QMessageBox>



/*
 *The new dialog window that allows the user to enter asset information
 *then pass the information so it can be add to db
 */


AddAssetWindow::AddAssetWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAssetWindow)
{
    ui->setupUi(this);

}

AddAssetWindow::~AddAssetWindow()
{
    delete ui;
}



// Populates the category drop down with database category names and stores them in a map
void AddAssetWindow::populateCategoryBox()
{
    ui->categoryBox->clear();     // Remove previous items
    categoryMap.clear();          // Clear stored category map

    // Ensure database pointer is valid and open
    if (!db || !db->isOpen()) {
        std::cerr << "Database connection not available\n";

        /*
         * NOTE: This message may appear in the console
         * This will appear because of the integration of the database.cpp
         *
         */



        return;
    }

    QSqlQuery query(*db);
    query.setForwardOnly(true);

    // Query all categories from the Category_Table
    if (!query.exec("SELECT CategoryID, Name FROM Category_Table")) {
        std::cerr << "Category query failed: "
                  << query.lastError().text().toStdString() << std::endl;
        return;
    }

    // Add each category to the combo box and store its ID in the map
    while (query.next()) {
        int id = query.value(0).toInt();
        string name = query.value(1).toString().toStdString();

        if (!name.empty()) {
            ui->categoryBox->addItem(QString::fromStdString(name));
            categoryMap[name] = id;   // store mapping for later use in getter
        }
    }
}



// Getters

string AddAssetWindow::getAssetId() const
{
    return "-1";
    //This is temporary as i didint want to change the other code to delete this one function
    //so when the asset data is pulled back into the mainwindow, we can skip getAssetID
    //when we add the query to insert the new asset we dont add the assetID because its autoincremental
}


// Returns the selected category ID as a string so it can be added to the db without conflicts
// Uses the categoryMap built during populateCategoryBox()
string AddAssetWindow::getCategoryId() const
{
    string selectedName = ui->categoryBox->currentText().toStdString();
    auto it = categoryMap.find(selectedName);

    if (it != categoryMap.end())
        return std::to_string(it->second);

    // fallback was required it should never occur cause our categories are preset
    return "NULL";
}


string AddAssetWindow::getName() const
{
    return ui->NameData->text().toStdString();
}

string AddAssetWindow::getTag() const
{
    return ui->TagData->text().toStdString();
}

string AddAssetWindow::getDescription() const
{
    return ui->DescriptionData->text().toStdString();
}

string AddAssetWindow::getLocation() const
{
    return ui->LocationData->text().toStdString();
}


double AddAssetWindow::getOriginalCost() const
{
    return ui->OriginalCostData->text().toDouble();
}


// Ensures Name and Tag fields are filled in before adding the asset
// These fields are required by the SQL database
void AddAssetWindow::on_AddButton_clicked()
{
    QString name = ui->NameData->text();
    QString tag  = ui->TagData->text();
    QString oringalCost = ui->OriginalCostData->text();

    // Reject submission if name and tag fields are empty
    if (name.isEmpty() || tag.isEmpty()) {
        QMessageBox::warning(this, "Input Error:", "Please fill in all required fields (Name and Tag)");
        return;
    }

    // Reject submission if OriginalCost is not a valid double
    // Only validate if the field is not empty
    if (!oringalCost.isEmpty())
    {
        bool ok = false;
        oringalCost.toDouble(&ok);  // Converts string to double and sets ok = true if conversion succeeds
        if (!ok)
        {
            QMessageBox::warning(this, "Input Error:", "Original Cost must be a valid number");
            return; // Stop here if it's invalid
        }
    }

    // Fields are filled in so it can pass information
    accept();
}


// Setter for database pointer so it can use the Database connection
void AddAssetWindow::setDatabase(QSqlDatabase *database)
{
    this->db = database;  // Store pointer for use in populateCategoryBox
}
