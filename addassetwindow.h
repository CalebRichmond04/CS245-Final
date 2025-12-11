#ifndef ADDASSETWINDOW_H
#define ADDASSETWINDOW_H

#include <QDialog>
#include <string>
using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class AddAssetWindow; }
QT_END_NAMESPACE

class AddAssetWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddAssetWindow(QWidget *parent = nullptr);
    ~AddAssetWindow();

    // Getters to retrieve data from the dialog as std::string
    string getAssetId() const;
    string getCategoryId() const;
    string getName() const;
    string getTag() const;
    string getDescription() const;
    string getLocation() const;
    string getOriginalCost() const;

private slots:
    void on_categoryBox_activated(int index);

private:
    Ui::AddAssetWindow *ui;
};

#endif // ADDASSETWINDOW_H
