#include "assettablemodel.h"
#include "asset.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <iostream>
#include <map>

using std::vector;
using std::string;

// Constructor
AssetTableModel::AssetTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Automatically load assets from the centralized Database
    loadAssetsFromDatabase();
}


/*
 * Header names for columns
 */
QVariant AssetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: return QString("Asset ID");
        case 1: return QString("Category");
        case 2: return QString("Name");
        case 3: return QString("Tag");
        case 4: return QString("Description");
        case 5: return QString("Location");
        case 6: return QString("Original Cost");
        default: return QVariant();
        }
    }
    return QVariant();
}


/*
 * Number of rows = number of assets
 */
int AssetTableModel::rowCount(const QModelIndex &parent) const
{
    return assets.size();
}


/*
 * Number of columns = 7
 */
int AssetTableModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}


/*
 * Returns data for each cell in the table
 */
QVariant AssetTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case 0: return QString::fromStdString(assets[row].getAssetId());
        case 1: return QString::fromStdString(assets[row].getCategoryId());
        case 2: return QString::fromStdString(assets[row].getAssetName());
        case 3: return QString::fromStdString(assets[row].getTag());
        case 4: return QString::fromStdString(assets[row].getDescription());
        case 5: return QString::fromStdString(assets[row].getLocation());
        case 6:
        {
            // Format original cost with $ and two decimals
            double cost = QString::fromStdString(assets[row].getOriginalCost()).toDouble();
            return QString("$%1").arg(cost, 0, 'f', 2);
        }
        }
    }

    return QVariant();
}


/*
 * Notify Qt that the model changed
 */
bool AssetTableModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        emit dataChanged(index(0,0), index(rowCount()-1, columnCount()-1));
        emit layoutChanged();
        return true;
    }
    return false;
}


/*
 * Add a new asset to the table
 */
void AssetTableModel::addAsset(const Asset &asset)
{
    int newRow = assets.size();

    // Notify Qt that a new row is being inserted
    beginInsertRows(QModelIndex(), newRow, newRow);

    assets.push_back(asset);

    // Notify Qt that row insertion is complete
    endInsertRows();
}


/*
 * Delete an asset by index
 */
void AssetTableModel::deleteAsset(int index)
{
    if(index >= 0 && index < assets.size()) {
        // Notify Qt that a row is being removed
        beginRemoveRows(QModelIndex(), index, index);

        assets.erase(assets.begin() + index);

        // Notify Qt that row removal is complete
        endRemoveRows();
    }
}


// Returns the Asset object stored at the clicked row
Asset AssetTableModel::getAsset(int row) const
{
    return assets[row];
}


/*
 * Load assets from SQL Server database into the model
 */
void AssetTableModel::loadAssetsFromDatabase()
{
    // Use the centralized Database class
    QSqlDatabase &db = dbManager.getConnection();
    if (!db.isOpen()) {
        std::cout << "Database not open\n";
        return;
    }

    // Load categories into a map
    std::map<int, string> categoryMap;
    QSqlQuery CategoryQuery(db);
    CategoryQuery.setForwardOnly(true);

    if (CategoryQuery.exec("SELECT CategoryID, Name FROM Category_Table")) {
        while (CategoryQuery.next()) {
            int id = CategoryQuery.value(0).toInt();
            string name = CategoryQuery.value(1).toString().toStdString();
            categoryMap[id] = name;
        }
    } else {
        std::cout << "Category query failed: "
                  << CategoryQuery.lastError().text().toStdString() << std::endl;
    }

    // Load assets from database
    QSqlQuery query(db);
    query.setForwardOnly(true);

    if (!query.exec("SELECT AssetID, CategoryID, Tag, Name, Description, Location, OrginalCost FROM Asset_Table")) {
        std::cout << "Asset query failed: "
                  << query.lastError().text().toStdString() << std::endl;
        return;
    }

    while (query.next()) {
        int assetIdInt    = query.value(0).toInt();
        int categoryIdInt = query.value(1).isNull() ? 0 : query.value(1).toInt();

        string tag          = query.value(2).toString().toStdString();
        string name         = query.value(3).toString().toStdString();
        string description  = query.value(4).isNull() ? "" : query.value(4).toString().toStdString();
        string location     = query.value(5).isNull() ? "" : query.value(5).toString().toStdString();
        string orginnalCost = query.value(6).isNull() ? "" : query.value(6).toString().toStdString();

        string assetIdStr = std::to_string(assetIdInt);
        string categoryName = categoryMap.count(categoryIdInt) ? categoryMap[categoryIdInt] : "N/A";

        Asset asset(assetIdStr, categoryName, name, tag, description, location, orginnalCost);
        addAsset(asset);
    }
}


/*
 * Update the model with a new vector of assets
 */
void AssetTableModel::setModelData(std::vector<Asset> updatedAssets)
{
    // Clear old items
    this->assets.clear();

    // Copy new assets if vector is not empty
    if (updatedAssets.size() > 0)
    {
        this->assets = updatedAssets;
    }

    // Refresh the view
    this->setData(this->index(0,0), 0);
}
