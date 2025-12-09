#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "assettablemodel.h"  // Include your asset table model
#include <QItemSelection> // ADDED

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addAssetButton_clicked();     // Slot for adding an asset
    void on_deleteAssetButton_clicked();  // Slot for deleting an asset
    void enableDropAssetButton(const QItemSelection &selected,
                               const QItemSelection &deselected); // Enable/disable delete button

private:
    Ui::MainWindow *ui;
    AssetTableModel *model;               // Model for the asset table
};

#endif // MAINWINDOW_H
