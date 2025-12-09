#ifndef ASSETTABLEMODEL_H
#define ASSETTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "asset.h"

class AssetTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    std::vector<Asset> assets;     // stores Asset objects
    void createAssets();           // helper function to create a few default assets

public:
    explicit AssetTableModel(QObject *parent = nullptr);

    // Override QAbstractTableModel methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Modifiers
    void addAsset(const Asset &asset);   // add a new asset
    void deleteAsset(int index);         // delete an asset by row index
};

#endif // ASSETTABLEMODEL_H

