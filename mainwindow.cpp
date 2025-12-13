#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addassetwindow.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <iostream>
#include <map>

using std::string;


/*
 *main code that uses all the other code files to show the assets filter add and delete assets, display assets
 *basicly manages the interactions from the user to be used in the code files
 *
 *YOU CAN DOUBLE CLICK DESCRIPTION OF A ASSET AND IT WILL OPEN A WINDOW TO DISPLAY THE FULL TEXT
 */


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create new asset model & apply it to the table view
    model = new AssetTableModel(this); // AssetTableModel now uses centralized Database
    ui->assetTable->setModel(model);

    // Set the table view to allow the columns to expand
    ui->assetTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Stretch the column headers to fit the width of the table view
    ui->assetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Resize the columns to fit their contents
    ui->assetTable->resizeColumnsToContents();

    // Set the table view to enable/disable the delete button based on selection
    connect(ui->assetTable->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::enableDropAssetButton);

    // Connect to show full description when Description column is double clicked
    connect(ui->assetTable, &QTableView::doubleClicked, this, &MainWindow::showFullDescription);

    // Populate the category combo box
    populateCategoryBox();
}

MainWindow::~MainWindow()
{
    delete model; // Clean up model
    delete ui;    // Clean up UI
}

/*
 * Add Asset button clicked
 * Opens the AddAssetWindow dialog and inserts new asset into database
 */
void MainWindow::on_addAssetButton_clicked()
{
    // Reuse the EXISTING database connection
    QSqlDatabase &db = dbManager.getConnection();

    // Ensure the database connection is open BEFORE showing the dialog
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database");
        return;
    }

    AddAssetWindow dialog(this);

    // Pass the database connection to the dialog
    dialog.setDatabase(&db);

    // Populate the category drop down now that the database is set
    dialog.populateCategoryBox();

    dialog.setModal(true);

    if(dialog.exec() == QDialog::Accepted)
    {
        // Retrieve user input from the dialog or drop down
        int categoryId   = std::stoi(dialog.getCategoryId()); // convert categoryId to int to fix SQL type
        string name         = dialog.getName();
        string tag          = dialog.getTag();
        string description  = dialog.getDescription();
        string location     = dialog.getLocation();
        double originalCost = dialog.getOriginalCost();

        // SQL INSERT statements
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO Asset_Table "
            "(CategoryID, Name, Tag, Description, Location, OrginalCost) "
            "VALUES "
            "(:categoryId, :name, :tag, :description, :location, :originalCost)"
            );

        // Bind user input to the SQL parameters
        query.bindValue(":categoryId", categoryId);
        query.bindValue(":name", QString::fromStdString(name));
        query.bindValue(":tag", QString::fromStdString(tag));
        query.bindValue(":description", QString::fromStdString(description));
        query.bindValue(":location", QString::fromStdString(location));
        query.bindValue(":originalCost", originalCost);

        // Execute INSERT query
        if (!query.exec()) {
            // Insert failed, show error message
            QMessageBox::critical(
                this,
                "Database Error",
                "Failed to insert asset: " + query.lastError().text()
                );
            return;
        }

        // Reload assets from the database so the table updates
        model->loadAssetsFromDatabase();
    }
}

/*
 * Deletes the selected Asset with confirmation window
 * Also deletes the asset from the database
 */
void MainWindow::on_deleteAssetButton_clicked()
{
    QModelIndex index = ui->assetTable->selectionModel()->currentIndex();
    int row = index.row();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Delete Asset",
        "Are you sure you want to delete this asset?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes)
    {
        // Get the asset to delete from the model
        Asset assetToDelete = model->getAsset(row);
        std::string assetId = assetToDelete.getAssetId();

        // Use centralized Database connection
        QSqlDatabase &db = dbManager.getConnection();

        if (!db.isOpen()) {
            QMessageBox::critical(this, "Database Error", "Failed to connect to the database");
            return;
        }

        // Prepare DELETE query for the database
        QSqlQuery query(db);
        query.prepare("DELETE FROM Asset_Table WHERE AssetID = :id");
        query.bindValue(":id", QString::fromStdString(assetId));

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error",
                                  "Failed to delete asset: " + query.lastError().text());
            return;
        }

        // Delete asset from model to update the table view
        model->deleteAsset(row);

        // Clear selection in table
        ui->assetTable->selectionModel()->clearSelection();
    }
}

/*
 * Enables the "Delete Asset" button when an asset is selected
 */
void MainWindow::enableDropAssetButton(const QItemSelection &selected,
                                       const QItemSelection &deselected)
{
    bool enabled = selected.size() > 0;
    ui->deleteAssetButton->setEnabled(enabled);
}

/*
 * Populates the category drop down with database category names
 */
void MainWindow::populateCategoryBox()
{
    ui->categoryBox->clear(); // Clear any existing items

    // Add "All" as the first option
    ui->categoryBox->addItem("All");

    // Use centralized Database class instead of creating a new connection
    QSqlDatabase &db = dbManager.getConnection();
    if (!db.isOpen()) {
        std::cerr << "Database connection failed\n";
        return;
    }

    // Query categories from database
    QSqlQuery query(db);
    query.setForwardOnly(true);

    if (!query.exec("SELECT Name FROM Category_Table")) {
        std::cerr << "Category query failed: "
                  << query.lastError().text().toStdString() << std::endl;
        return;
    }

    // Add categories to combo box
    while (query.next()) {
        QString categoryName = query.value(0).toString();
        if (!categoryName.isEmpty()) {
            ui->categoryBox->addItem(categoryName);
        }
    }

    // No need to close the database: Database class manages the connection
}

/*
 * Calls the DB to get the most recent assets, then filters them based on the selected category
 */
void MainWindow::on_categoryBox_activated(int index)
{
    QString selectedCategory = ui->categoryBox->itemText(index);

    // Use centralized Database connection
    QSqlDatabase &db = dbManager.getConnection();
    if (!db.isOpen()) {
        std::cout << "Database not open\n";
        return;
    }

    // Load Categories into a map
    std::map<int, std::string> categoryMap;
    QSqlQuery categoryQuery(db);
    categoryQuery.setForwardOnly(true);

    if (categoryQuery.exec("SELECT CategoryID, Name FROM Category_Table")) {
        while (categoryQuery.next()) {
            int id = categoryQuery.value(0).toInt();
            std::string name = categoryQuery.value(1).toString().toStdString();
            categoryMap[id] = name;
        }
    } else {
        std::cout << "Category query failed: "
                  << categoryQuery.lastError().text().toStdString() << std::endl;
    }

    // Load Assets and filter by category
    std::vector<Asset> filteredAssets;
    QSqlQuery query(db);
    query.setForwardOnly(true);

    if (!query.exec("SELECT AssetID, CategoryID, Tag, Name, Description, Location, OrginalCost FROM Asset_Table")) {
        std::cout << "Asset query failed: "
                  << query.lastError().text().toStdString() << std::endl;
        return;
    }

    while (query.next()) {
        int assetIdInt = query.value(0).toInt();
        int categoryIdInt = query.value(1).isNull() ? 0 : query.value(1).toInt();

        std::string tag          = query.value(2).toString().toStdString();
        std::string name         = query.value(3).toString().toStdString();
        std::string description  = query.value(4).isNull() ? "" : query.value(4).toString().toStdString();
        std::string location     = query.value(5).isNull() ? "" : query.value(5).toString().toStdString();
        std::string orginnalCost = query.value(6).isNull() ? "" : query.value(6).toString().toStdString();

        std::string assetIdStr = std::to_string(assetIdInt);
        std::string categoryName = categoryMap.count(categoryIdInt) ? categoryMap[categoryIdInt] : "N/A";

        // Include all assets if "All" is selected; if not, filter by category
        if (selectedCategory == "All" || categoryName == selectedCategory.toStdString()) {
            Asset asset(assetIdStr, categoryName, name, tag, description, location, orginnalCost);
            filteredAssets.push_back(asset);
        }
    }

    // Update the model
    model->setModelData(filteredAssets);
}

/*
 * Shows a popup with the full description when the Description column is double clicked
 */
void MainWindow::showFullDescription(const QModelIndex &index)
{
    int row = index.row();
    int col = index.column();

    // Only trigger for Description column
    if(col == 4) {
        string fullDesc = model->getAsset(row).getDescription();
        QMessageBox::information(this, "Full Description",
                                 QString::fromStdString(fullDesc));
    }
}
