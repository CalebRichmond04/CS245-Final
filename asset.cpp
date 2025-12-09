#include "asset.h"

Asset::Asset() {}

Asset::Asset(const string &id, const string &catId, const string &name,
             const string &tag, const string &desc, const string &loc, const string &cost)
    : assetId(id), categoryId(catId), assetName(name),
    tag(tag), description(desc), location(loc), originalCost(cost)
{
}

// Getters
string Asset::getAssetId() const
{
    return assetId;
}

string Asset::getCategoryId() const
{
    return categoryId;
}

string Asset::getAssetName() const
{
    return assetName;
}

string Asset::getTag() const
{
    return tag;
}

string Asset::getDescription() const
{
    return description;
}

string Asset::getLocation() const
{
    return location;
}

string Asset::getOriginalCost() const
{
    return originalCost;
}

// Setters
void Asset::setAssetId(const string &id)
{
    assetId = id;
}

void Asset::setCategoryId(const string &catId)
{
    categoryId = catId;
}

void Asset::setAssetName(const string &name)
{
    assetName = name;
}

void Asset::setTag(const string &t)
{
    tag = t;
}

void Asset::setDescription(const string &desc)
{
    description = desc;
}

void Asset::setLocation(const string &loc)
{
    location = loc;
}

void Asset::setOriginalCost(const string &cost)
{
    originalCost = cost;
}
