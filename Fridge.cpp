#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Fridge.hpp"
#include "Supplier.hpp"
#include "iohelper.hpp"

#define PERPAGE 10	// Number of records to show per page when editing

using std::cout;
using std::endl;
using std::fstream;
using std::istringstream;

Fridge::Fridge(User* user)
{
	_user = user;
	_supplier = new Supplier();

	_items = Items().GetAll();

	getInventoryFromCsv(INVFILE);
}

void Fridge::getInventoryFromCsv(string filename)
{
	// Note: partially adapted from https://stackoverflow.com/questions/19936483/c-reading-csv-file

	std::ifstream inFile(filename);

	if (inFile.bad())	// Check for error opening file
	{
		std::cout << "FATAL Error reading file: " << filename << std::endl;
		exit(1);
	}

	string line, sku, dateYearStr, dateDayStr, quantStr, qooStr;
	int dy, dd, quant, qoo;

	while (getline(inFile, line))	// Get line from file
	{
		std::istringstream ss(line);	// Split into fields

		getline(ss, sku, ',');
		getline(ss, quantStr, ',');
		getline(ss, qooStr, ',');
		getline(ss, dateYearStr, ',');
		getline(ss, dateDayStr, ',');

		dy = std::stoi(dateYearStr);
		dd = std::stoi(dateDayStr);
		quant = std::stoi(quantStr);
		qoo = std::stoi(qooStr);

		ItemInfo *item = _items.at(sku);	// This will throw an error if sku not in _items!!
		Date ds(dy, dd);

		_contents.push_back(new FridgeItem(item, quant, qoo, ds));
	}

	inFile.close();
}

// Remove some amount(default = 1) from item by sku.  Removes item from contents if quantity < 0.
void Fridge::Use(string sku, int amount)
{
	FridgeItem* item = GetInfoBySku(sku);
	if (item)
	{
		item->quantity -= amount;
		if (item->quantity < 1 && item->quantOnOrder < 1)
		{
			int index = GetIndexBySku(sku);
			_contents.erase(_contents.begin() + index);
			cout << "You have run out of that item" << endl;
		}
		else
		{
			cout << "You have " << item->quantity << " of " << item->itemInfo->displayName << "(s) left in your fridge" << endl;
		}
	}
	else
	{
		cout << "That item does not exist in your Fridge" << endl;
	}
}

void Fridge::ViewQuantity(string sku)
{
	FridgeItem* item = GetInfoBySku(sku);
	if (item)
	{
		cout << "You have " << item->quantity << " of " << item->itemInfo->displayName << "(s) left in your fridge" << endl;
	}
	else
	{
		cout << "That item does not exist in your Fridge" << endl;
	}
}

// Remove some amount(default = 1) from item by sku.  Removes item from contents if quantity < 0.
void Fridge::Update(string sku, int amount)
{
	FridgeItem* item = GetInfoBySku(sku);
	if (item)
	{
		item->quantity = amount;
		if (item->quantity < 1)
		{
			int index = GetIndexBySku(sku);
			_contents.erase(_contents.begin() + index);
			cout << "You have run out of that item" << endl;
			//UpdateQuantityInCSV(sku, 0);
		}
		else
		{
			cout << "You have " << item->quantity << " of " << item->itemInfo->displayName << "(s) left in your fridge" << endl;
			//UpdateQuantityInCSV(sku, item->quantity);
		}
		updateInventory();
	}
	else
	{
		cout << "That item does not exist in your Fridge" << endl;
	}
}

void Fridge::UpdateQuantityInCSV(string sku, int amount)
{
	//Opening accounts file that contains all user information
	fstream fridgeFile(to_string(_user->GetAccount()) + ".csv");

	fstream newFridgeFile("temp.csv");

	if (!fridgeFile.is_open())	// Check if file exists
	{
		//if user account file file doesn't exist, then we can't update anything
		return;
	}
	else
	{
		//Creating variables for searching file
		string nextItemLine;
		string SKU;
		string quantity;
		string year;
		string day;
		string goodFor;
		string nextString;
		vector<string> row;

		//Searching account specific file for a matching SKU
		while (!fridgeFile.eof())
		{
			row.clear();

			//Getting each line of csv and adding separated values to vector
			getline(fridgeFile, nextItemLine);
			istringstream ss(nextItemLine);

			while (getline(ss, nextString, ','))
			{
				row.push_back(nextString);
			}

			//Storing item information to variables
			SKU = row[0];
			quantity = row[1];
			year = row[2];
			day = row[3];
			goodFor = row[4];

			if (SKU == sku)
			{
				quantity = to_string(amount);
			}

			newFridgeFile << SKU << "," << quantity << "," << year << "," << day << "," << goodFor << "\n";

		}
	}

	newFridgeFile.close();
	fridgeFile.close();
}

int Fridge::GetIndexBySku(string sku)
{
	for (unsigned i = 0; i < _contents.size(); ++i)
	{
		if (_contents[i]->itemInfo->sku == sku)
		{
			return i;
		}
	}
	return -1;
}

FridgeItem* Fridge::GetInfoBySku(string sku)
{
	for (unsigned i = 0; i < _contents.size(); ++i)
	{
		if (_contents[i]->itemInfo->sku == sku)
		{
			return _contents[i];
		}
	}
	return nullptr;
}

// returns all favorites regardless of current stock
vector<ItemInfo*> Fridge::GetFavorites()
{
	vector<ItemInfo*> favorites;
	map<string, ItemInfo*>::iterator it = _items.begin();
	while (it != _items.end())
	{
		ItemInfo* item = it->second;
		if (item->favorite)
		{
			favorites.push_back(item);
		}
		++it;
	}
	return favorites;
}

//Adds an item to the list of contents. Runs the "CreateNewItem" function
//if the item is not in contents already. Needs to be adjusted to track
//items that are already known but not in contents
void Fridge::AddItem(string sku, int quantity, int quantOnOrder) {
	//Will return -1 if item was not found
	int place = this->GetIndexBySku(sku);
	if (place < 0) {
		ItemInfo* new_item = GetItemInfoBySku(sku);
		if (new_item == nullptr)
		{
			new_item = Items().CreateNewItem(sku);
		}
		this->_contents.push_back(new FridgeItem(new_item, quantity, quantOnOrder, GetCurrentDate()));
		_items[sku] = new_item;
	}
	else
	{
		this->_contents[place]->quantity += quantity;
	}
	updateInventory();
}


void Fridge::orderLowItems() {

	for (std::map<string, ItemInfo*>::iterator it = _items.begin(); it != _items.end(); it++) {

		//only add the item to the order list if it has a designated minimum quantity threshold
		if (it->second->minQuantity > 0) {

			int index = GetIndexBySku(it->first);

			//if the fridge is not currently stocked with the item
			if (index == -1) {
				int orderMultiple = 1;
				//ensure amount to be ordered is greater than the designated minimum quantity for the item
				while (orderMultiple * it->second->orderQuantity < it->second->minQuantity) orderMultiple++;
				//add item to order list
				orderList.insert({ it->first, orderMultiple });
				/* No need to worry about adding too many items to the orderList if orderLowItems() is called multiple times
				   before an order is received since the orderList is a map and the appropriate quantity will be overwritten
				   at the corresponding sku key rather than indexed multiple times as a {key, value} element in the map */
			}//end if

			//if the fridge has a current quantity for the item and it's below minQuantity
			else if ((_contents[index]->quantity+_contents[index]->quantOnOrder) < it->second->minQuantity) {
				int orderMultiple = 1;
				int currentQuantity = (_contents[index]->quantity + _contents[index]->quantOnOrder) + it->second->orderQuantity;
				while (currentQuantity < it->second->minQuantity) {
					currentQuantity += it->second->orderQuantity;
					orderMultiple++;
				}//end while
				//add item and quantity to order list
				orderList.insert({ it->first, orderMultiple });
			}//end else if

		}//end outer if

	}//end for loop
}//end orderLowItems

void Fridge::updateInventory() {
	std::ofstream outFile(INVFILE, std::ios::trunc);
	for (FridgeItem* fridgeItem : _contents) {
		ItemInfo* item = fridgeItem->itemInfo;
		outFile << item->sku << "," << fridgeItem->quantity << "," << fridgeItem->quantOnOrder << ',' 
				<< fridgeItem->dateStocked.Year << "," << fridgeItem->dateStocked.Days << std::endl;
	}
	outFile.close();
}

void Fridge::printOrderList() {
	if (orderList.size() == 0)
	{
		std::cout << "You are not low on any items at this time." << std::endl;
		return;
	}

	std::cout << "Item name | Order Units | Quantity Ordered" << std::endl << std::endl;
	for (std::map<string, int>::iterator it = orderList.begin(); it != orderList.end(); it++) {
		ItemInfo* orderItem = _items.at(it->first);
		std::cout << orderItem->displayName << " | " << it->second 
			<< " | " << (orderItem->orderQuantity * it->second) << std::endl;
	}
	string output = _supplier->PrettyPrintJson(_supplier->GetOrderListJson(orderList));
	std::cout << output << std::endl;
}

void Fridge::SubmitOrder()
{
	//don't submit an order if the orderList is empty
	if (orderList.empty()) return;

	_supplier->CreateOrder(_user, orderList);
	std::cout << std::endl << "Order placed!" << std::endl;
	//loops through order items and finds matching items in fridge contents to update their 'quantOnOrder' field
	//or if ordered item is not in fridge contents, it is added with a quantity of 0 and appropriate 'quantOnOrder' value
	for (std::map<string, int>::iterator it = orderList.begin(); it != orderList.end(); it++) {
		if (GetIndexBySku(it->first) != -1)
			GetInfoBySku(it->first)->quantOnOrder = it->second * GetItemInfoBySku(it->first)->orderQuantity;
		else
			AddItem(it->first, 0, it->second * GetItemInfoBySku(it->first)->orderQuantity);
	}
	updateInventory();
	std::cout << "Current fridge items 'On-Order Quantity' successfully updated" << std::endl << std::endl;
	orderList.clear();	// Order placed and added to log
}

void Fridge::ReceiveOrder(string orderJson)
{
	vector<FridgeItem*> receivedItems = _supplier->ProcessReceivedOrder(orderJson);
	for (FridgeItem* recvdItem : receivedItems) {
		FridgeItem* currFridgeItem = GetInfoBySku(recvdItem->itemInfo->sku);
		currFridgeItem->quantity += recvdItem->quantity;
		currFridgeItem->quantOnOrder -= recvdItem->quantity;
	}
	updateInventory();
}

// Takes sku string and returns ItemInfo pointer (or nullptr if sku not in items map)
ItemInfo* Fridge::GetItemInfoBySku(string sku)
{
	if (_items.find(sku) != _items.end())
	{
		return _items[sku];
	}
	else
	{
		return nullptr;
	}
}

string Fridge::stringifyContents() {

	string retString = "";
	for (FridgeItem* i : _contents)
	{
		ItemInfo* item = i->itemInfo;
		retString += (item->displayName + "| " + item->fullName + "| " + item->sku + "| ");
		retString += (GetDateString(i->dateStocked) + "| " + std::to_string(i->quantity) + "| " + std::to_string(i->quantOnOrder));
		if (item->favorite) retString += "| Favorite";
		retString += '\n';
	}

	return retString;
}

string Fridge::stringifyFavorites() {

	string retString = "";
	vector<ItemInfo*> favorites = GetFavorites();
	if (favorites.empty()) {
		retString += "No items have been favorited.\n";
		return retString;
	}

	for (ItemInfo* item : favorites)
	{
		retString += ( item->displayName + "| (" + item->sku + ")| " );
		FridgeItem* current = GetInfoBySku(item->sku);
		if (current != nullptr && current->quantity > 0)
		{
			retString += std::to_string(current->quantity);
		}
		else
		{
			retString += "None";
		}
		retString += " in the fridge.\n";
	}

	return retString;

}


// Presents a list of all items (PERPAGE at a time) to select for editing
void Fridge::EditItemMenu()
{
	int page = 0;	// Page number, items listed in 'pages' of ten items
	bool done = false, changes = false;

	// Too messy to iterate through map in chunks, using vector of skus instead
	vector<string> skus;
	// ref: https://stackoverflow.com/questions/110157/
	for (auto const& item : _items) skus.push_back(item.first);

	//sort the vector so that items are appear alphabetically according to their display name
	std::sort(skus.begin(), skus.end(), [&](const string i, const string j) { 
		return GetItemInfoBySku(i)->displayName < GetItemInfoBySku(j)->displayName; 
	});

	int maxPages = (int)skus.size() / PERPAGE + ((int)skus.size() % PERPAGE > 0 ? 1 : 0);

	while (!done)
	{
		printEmptyLines();
		printString("** Edit Item Information **\n\n"
					"Choose an item to edit:\n\n"
					"0  - Return to Main Menu\n"
					"1  - Move Forward One Page\n"
					"2  - Move Backward One Page\n"
		);
		for (int i = 3; i < PERPAGE + 3; ++i)
		{
			int index = (page * PERPAGE) + i - 3;
			if (index >= (int)skus.size()) break;

			ItemInfo* current = GetItemInfoBySku(skus[index]);
			if (current == nullptr) perror("Invalid sku in EditItemMenu");
			else 
			{
				std::cout << i << (i < 10 ? " " : "") << " - " << current->sku << " : " << current->displayName 
					<< std::endl;
			}
		}
		int choice = getInt(0, PERPAGE + 2, "\nEnter Choice: ");

		switch (choice)
		{
		case 0:
			done = true;
			break;
		case 1:
			++page;
			if (page >= maxPages) page = maxPages - 1;
			break;
		case 2:
			--page;
			if (page < 0) page = 0;
			break;
		default:
			int index = (page * PERPAGE) + choice - 3;
			if (!changes) changes = EditItemInfo(skus[index]);
			else EditItemInfo(skus[index]);	// Once changes is true, don't make it false later
			break;
		}
	}
	if (changes) SaveItemsToFile();	// Only rewrite file if an item was actually changed
}

// From EditItemMenu - Edit selected item values (return true if any changes made)
bool Fridge::EditItemInfo(string sku)
{
	bool done = false, changes = false;
	char yesNo[2] = { 'y', 'n' };
	char isFav;

	ItemInfo* item = GetItemInfoBySku(sku);
	if (item == nullptr) perror("Invalid sku in EditItemInfo");

	while (!done)
	{
		printEmptyLines();
		printString("** Edit Item Information **\n\n"
			"Choose information to edit:\n\n"
			"0 - Done - Return to Items List\n"
			"1 - Display (short) name [" + item->displayName + "] :\n"
			"2 - Amazon description [" + item->fullName + "] :\n"
			"3 - Minimum amount to have on hand (0 to not auto-order) [" + std::to_string(item->minQuantity) + "] :\n"
			"4 - Order quantity (number of items in each box) [" + std::to_string(item->orderQuantity) + "] :\n"
			"5 - Mark item as favorite? [" + (item->favorite ? "Yes" : "No") + "] :\n"
		);
		int choice = getInt(0, 5, "\nEnter choice:");
		switch (choice)
		{
		case 0:
			done = true;
			break;
		case 1:
			item->displayName = getString("Enter new display name:");
			changes = true;
			break;
		case 2:
			item->fullName = getString("Enter new description:");
			changes = true;
			break;
		case 3:
			item->minQuantity = getInt(0, 9999, "Enter new minimum amount:");
			changes = true;
			break;
		case 4:
			item->orderQuantity = getInt(1, 9999, "Enter new order quantity:");
			changes = true;
			break;
		case 5:
			isFav = getChar("Mark item as favorite? (y/n):", yesNo, 2);
			item->favorite = (isFav == 'y');
			changes = true;
			break;
		default:
			break;
		}
	}
	return changes;
}

// Replace items file with current values
void Fridge::SaveItemsToFile()
{
	// NOTE: this can't be done in Items because Items reads values from file in constructor
	// which would overwrite the new values we just updated
	std::ofstream outfile(ITEMSFILE, std::ios::trunc);

	for (auto const& item : _items)
	{
		outfile << item.second->displayName << ',' 
			<< item.second->fullName << ',' 
			<< item.second->sku << ',' 
			<< item.second->minQuantity << ',' 
			<< item.second->orderQuantity << ',' 
			<< (item.second->favorite ? "true" : "false") << std::endl;
	}
	outfile.close();
}

//Function to list all current fridge contents.
void Fridge::ListContents(){
	cout << "Current fridge inventory:" << endl;
	cout << "Item 					Amount" << endl;
	for(unsigned int i = 0; i < this->_contents.size(); i++){
		FridgeItem* item = this->_contents[i];
		cout << item->itemInfo->displayName << ": " << item->quantity << endl;
	}
}

void Fridge::LowStockCallback(){
	int lowCount = 0;
	for(unsigned int i = 0; i < this->_contents.size(); i++){
		FridgeItem* item = this->_contents[i];
		if(item->quantity < item->itemInfo->minQuantity){
			if(lowCount == 0){
				cout << "Warning: You are running low on the following items: " << endl;
			}
			lowCount++;
			cout << item->itemInfo->displayName << ". Only " << item->quantity << " Remaining!" << endl;
		}
	}
}
