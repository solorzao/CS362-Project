#pragma once

#include <string>
using std::string;
#include <map>
using std::map;
#include <vector>

#define ITEMSFILE "items_list.csv"

struct ItemInfo {
    string displayName;
    string fullName;
    string sku;
    int minQuantity;
    int orderQuantity;
    bool favorite;
	ItemInfo(string sku, string displayname, string fullname, int minQuantity, int orderQuantity, bool favorite)
		: displayName(displayname), fullName(fullname), sku(sku), minQuantity(minQuantity), 
          orderQuantity(orderQuantity), favorite(favorite) { }
    ItemInfo(string sku)
        : sku(sku), displayName("<NewItem>"), fullName("<NewItem>"), minQuantity(0), orderQuantity(1), favorite(false) { }
};

class Items
{
private:
    map<string, ItemInfo*> _items;
    void getItemsFromCsv(string filename);

public:
    Items();
    map<string, ItemInfo*> GetAll() { return _items; }
    ItemInfo* GetBySku(string sku) { return _items[sku]; }
    bool isFavorite(string sku);
    void SetMinQuantity(string sku, int amount);
    void SetFavorite(string sku, bool fav = true);
    ItemInfo* GetItemInfoBySku(string sku);
    ItemInfo* CreateNewItem(string sku);
};

