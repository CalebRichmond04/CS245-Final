#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addassetwindow.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QHeaderView>
#include <iostream>
#include <ostream>
#include <string>
using std::string;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create new asset model & apply it to the table view
    model = new AssetTableModel(this);
    ui->assetTable->setModel(model);

    // Set the table view to allow the columns to expand
    ui->assetTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Stretch the column headers to fit the width of the table view
    ui->assetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Resize the columns to fit their contents
    ui->assetTable->resizeColumnsToContents();

    // Set the table view's selection listener
    connect(ui->assetTable->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::enableDropAssetButton);

    // Connect to show full description when double clicked
    connect(ui->assetTable, &QTableView::doubleClicked, this, &MainWindow::showFullDescription);

    // Populate the category combo box
    populateCategoryBox();
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}

/*
 * Add Asset button clicked
 */
void MainWindow::on_addAssetButton_clicked()
{
    AddAssetWindow dialog(this);
    dialog.setModal(true);

    if(dialog.exec() == QDialog::Accepted)
    {
        Asset newAsset(
            dialog.getAssetId(),
            dialog.getCategoryId(),
            dialog.getName(),
            dialog.getTag(),
            dialog.getDescription(),
            dialog.getLocation(),
            dialog.getOriginalCost()
            );

        model->addAsset(newAsset);
    }
}

/*
 * Deletes the selected Asset with the end user's confirmation
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
        model->deleteAsset(row);
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




void MainWindow::populateCategoryBox()
{
    ui->categoryBox->clear(); // Clear any existing items

    // Create a database connection object
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
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

    if (!db.open()) {
        std::cout << "Database connection failed: "
                  << db.lastError().text().toStdString() << std::endl;
        return;
    }

    // Query categories
    QSqlQuery query(db);
    query.setForwardOnly(true);

    if (!query.exec("SELECT Name FROM Category_Table")) {
        std::cout << "Category query failed: "
                  << query.lastError().text().toStdString() << std::endl;
        db.close();
        return;
    }

    // Populate the combo box
    while (query.next()) {
        QString categoryName = query.value(0).toString();
        ui->categoryBox->addItem(categoryName);
    }
    db.close();
}



/*
 * Handles category selection changes
 */
void MainWindow::on_categoryBox_activated(int index)
{
    // TODO: Implement filtering by category if desired
}



/*
 * Shows a popup with the full description when the Description column is clicked
 */
void MainWindow::showFullDescription(const QModelIndex &index)
{
    int row = index.row();
    int col = index.column();

    // Only trigger for Description
    if(col == 4) {
        string fullDesc = model->getAsset(row).getDescription();
        QMessageBox::information(this, "Full Description",
                                 QString::fromStdString(fullDesc));
    }
}
