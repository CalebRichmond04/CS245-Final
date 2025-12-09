#include "addassetwindow.h"
#include "ui_addassetwindow.h"

AddAssetWindow::AddAssetWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAssetWindow)
{
    ui->setupUi(this);

    // Connect the AddButton to accept the dialog
    connect(ui->AddButton, &QPushButton::clicked, this, &AddAssetWindow::accept);
}

AddAssetWindow::~AddAssetWindow()
{
    delete ui;
}

// Getters returning std::string
string AddAssetWindow::getAssetId() const
{
    return ui->AssetIdData->text().toStdString();
}

string AddAssetWindow::getCategoryId() const
{
    return ui->CategoryIdData->text().toStdString();
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

string AddAssetWindow::getOriginalCost() const
{
    return ui->OriginalCostData->text().toStdString();
}

//NEED TO ADD A CHECK TO MAKE SURE THAT THE REQUIRED FIELDS ARE THERE BEFORE UPLOADING THEM TO THE DATABSE

//THIS CODE WILL CHNAGE WITH TRACY CODE IMPLEMNTATION ALONG WITH THE MAINWINDOW.CPP FILE
//MAINWINDOW>CPP FILE WILL CHANGE SLIGHTYLY TO ACCOMIDATE THE DATABSE INTAGRAION
