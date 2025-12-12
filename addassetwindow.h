#ifndef ADDASSETWINDOW_H
#define ADDASSETWINDOW_H

#include <QDialog>
#include <string>
using std::string;
#include <map>
using std::map;


QT_BEGIN_NAMESPACE
namespace Ui { class AddAssetWindow; }
QT_END_NAMESPACE

class AddAssetWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddAssetWindow(QWidget *parent = nullptr);
    ~AddAssetWindow();

    // Getters to retrieve data from the dialog
    string getAssetId() const;
    string getCategoryId() const;
    string getName() const;
    string getTag() const;
    string getDescription() const;
    string getLocation() const;
    string getOriginalCost() const;

    // Populates the category drop down with database category names
    void populateCategoryBox();

private slots:

    void on_AddButton_clicked(); // now validates required fields before accepting

private:
    Ui::AddAssetWindow *ui;

    map<std::string,int> categoryMap;  // Map category name -> ID;


};

#endif // ADDASSETWINDOW_H
