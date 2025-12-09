#include <QBrush>
#include <QColor>
#include <QFont>
#include "assettablemodel.h"
#include "asset.h"

using std::vector;

AssetTableModel::AssetTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    createAssets();
}

/*
 * Create some example Asset objects
 */
void AssetTableModel::createAssets()
{
    Asset a1("A001", "C001", "Caleb_Laptop-01", "Laptop", "LT-01", "Unkown", "1200");
    Asset a2("A002", "C002", "Speakers", "Peripheral", "CH-01", "Librarye", "250");
    Asset a3("A003", "C003", "Projector-AD202", "Misc", "PJ-01", "AD-202", "800");

    assets.push_back(a1);
    assets.push_back(a2);
    assets.push_back(a3);
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
        case 1: return QString("Category ID");
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
        case 6: return QString::fromStdString(assets[row].getOriginalCost());
        }
    }

    // Optional: Bold the Name column ust a place holder we can use to change for other things
    if (role == Qt::FontRole && col == 2)
    {
        QFont boldFont;
        boldFont.setBold(true);
        return boldFont;
    }

    // Optional: color the Tag column red just a place holder we can use to change for other things
    if (role == Qt::ForegroundRole && col == 3)
    {
        return QBrush(QColor(255, 255, 255));
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
    assets.push_back(asset);
    this->setData(index(0,0), 0);
}

/*
 * Delete an asset by index
 */
void AssetTableModel::deleteAsset(int index)
{
    if (index >= 0 && index < assets.size())
    {
        assets.erase(assets.begin() + index);
        this->setData(this->index(0,0), 0);
    }
}

