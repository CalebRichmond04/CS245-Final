#include "addassetwindow.h"
#include "ui_addassetwindow.h"
#include <QMessageBox>



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

// Getters

//this will be used as a drop down for the user to add the category to the asset
//make sure when adding the asset it adds it as a int for the category ID
void AddAssetWindow::on_categoryBox_activated(int index)
{

}

//this is temp code it will need to retreive it from the database or vector
//this increments by 1 and starts at 1 so if we can get the vector length and incremnt the Id by 1 then that would work maybe
//cant be empty
string AddAssetWindow::getAssetId() const
{
    return ui->NameData->text().toStdString();
}

//this is temp code it will need to retreive it from the two dbs or vector
//categories are stored as ints starting at 1
//maybe change this so it will get category name instead
string AddAssetWindow::getCategoryId() const
{
    return ui->NameData->text().toStdString();
}


//Name cant be empty try might be the best method or other means
string AddAssetWindow::getName() const
{
    try
    {
        if (ui->NameData->text().toStdString() != "")
        {

        return ui->NameData->text().toStdString();

        }
    }
    catch (...)
    {

    }

}

//tag cant be empty will work the same as the name just for tag
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

