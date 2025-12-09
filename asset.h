#ifndef ASSET_H
#define ASSET_H

#include <string>
using std::string;

class Asset
{
private:
    string assetId;
    string categoryId;
    string assetName;
    string tag;
    string description;
    string location;
    string originalCost;

public:
    Asset();
    Asset(const string &id, const string &catId, const string &name,
          const string &tag, const string &desc, const string &loc, const string &cost);

    // Getters
    string getAssetId() const;
    string getCategoryId() const;
    string getAssetName() const;
    string getTag() const;
    string getDescription() const;
    string getLocation() const;
    string getOriginalCost() const;

    // Setters
    void setAssetId(const string &id);
    void setCategoryId(const string &catId);
    void setAssetName(const string &name);
    void setTag(const string &tag);
    void setDescription(const string &desc);
    void setLocation(const string &loc);
    void setOriginalCost(const string &cost);
};

#endif
