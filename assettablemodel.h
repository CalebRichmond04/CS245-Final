#ifndef ASSETTABLEMODEL_H
#define ASSETTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "asset.h"
#include "database.h" // Include the Database class

class AssetTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    std::vector<Asset> assets;     // Stores Asset objects
    void createAssets();            // Helper function (if needed)
    Database dbManager;             // Database manager

public:
    explicit AssetTableModel(QObject *parent = nullptr);

    // Override QAbstractTableModel methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Modifiers
    void addAsset(const Asset &asset);
    void deleteAsset(int index);
    void loadAssetsFromDatabase();
    Asset getAsset(int row) const;
    void setModelData(std::vector<Asset> updatedAssets);
};

#endif // ASSETTABLEMODEL_H
