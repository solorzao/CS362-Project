#pragma once

#include <vector>
#include <string>

using std::vector;
using std::to_string;

#include "Items.hpp"
#include "User.hpp"
#include "DateConverter.hpp"

#define INVFILE "inventory_list.csv"
#define ORDERLOG "order_log.csv"
#define RECVLOG "received_log.csv"



struct FridgeItem
{
	ItemInfo *itemInfo;
	int quantity;
	int quantOnOrder;
	Date dateStocked;

	FridgeItem(ItemInfo *itemInfo, int quantity, int qoo, Date dateStocked) :
		itemInfo(itemInfo), quantity(quantity), quantOnOrder(qoo), dateStocked(dateStocked)
	{ }
};

class Supplier;	// Forward declaration

class Fridge
{
private:
	User* _user;
	Supplier* _supplier;
	map<string, ItemInfo*> _items;
	vector<FridgeItem*> _contents;
	map<string, int> groceryList;
	map<string, int> orderList;
	void getInventoryFromCsv(string sku);

public:
	Fridge(User* user);
	User* GetUser() { return _user; }
	vector<FridgeItem*> GetContents() { return _contents; }
	map<string, ItemInfo*> GetAllItems() { return _items; }
	bool isFavorite(string sku) { return _items.at(sku)->favorite; }
	void Use(string sku, int amount = 1);
	void ViewQuantity(string sku);
	void Update(string sku, int amount);
	void UpdateQuantityInCSV(string sku, int amount);
	int GetIndexBySku(string sku);
	FridgeItem* GetInfoBySku(string sku);
	vector<ItemInfo*> GetFavorites();
	void AddItem(string sku, int quantity, int quantOnOrder=0);
	void orderLowItems();
	void updateInventory();
	void printOrderList();
	void SubmitOrder();
	void ReceiveOrder(string orderJson);
	ItemInfo* GetItemInfoBySku(string sku);
	string stringifyContents();
	string stringifyFavorites();
	void EditItemMenu();
	bool EditItemInfo(string sku);
	void SaveItemsToFile();
	void ListContents();
	void LowStockCallback();
};
