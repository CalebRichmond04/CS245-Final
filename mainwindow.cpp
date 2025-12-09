#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addassetwindow.h"
#include <QMessageBox>
#include <QHeaderView>

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
    connect(
        ui->assetTable->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        &MainWindow::enableDropAssetButton
        );
}

MainWindow::~MainWindow()
{
    // Delete the model and UI
    delete model;
    delete ui;
}

/*
 * Placeholder: Add Asset button clicked
 * This will eventually open the AddAssetDialog
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
 * Deletes the selected Asset with the end user's confirmation.
 */
void MainWindow::on_deleteAssetButton_clicked()
{
    // Get selected row
    QModelIndex index = ui->assetTable->selectionModel()->currentIndex();
    int row = index.row();

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Delete Asset",
        "Are you sure you want to delete this asset?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes)
    {
        // Delete the asset in the model
        model->deleteAsset(row);

        // Clear any selection in the table
        ui->assetTable->selectionModel()->clearSelection();
    }
}

/*
 * Enables the "Delete Asset" button when an asset is selected
 * and disables it when no asset is selected.
 */
void MainWindow::enableDropAssetButton(const QItemSelection &selected,
                                       const QItemSelection &deselected)
{
    bool enabled = selected.size() > 0;
    ui->deleteAssetButton->setEnabled(enabled);
}





//REMEBER TO ADD THINGS LIKE SORT BY CATEGORY EITHER IN A BUTTON THAT UPDATES THE TABLEMODLE







