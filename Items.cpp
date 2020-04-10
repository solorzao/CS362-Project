#include "Items.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "iohelper.hpp"

Items::Items()
{
	getItemsFromCsv(ITEMSFILE);
}

bool Items::isFavorite(string sku)
{
	return (_items[sku]->favorite == true);
}

// Change minQuanity for an item (by sku)
void Items::SetMinQuantity(string sku, int amount)
{
	if (_items.find(sku) != _items.end())
	{
		_items[sku]->minQuantity = amount;
	}
}

// Set favorite status(bool, default = true) of item by sku.
void Items::SetFavorite(string sku, bool fav)
{
	if (_items.find(sku) != _items.end())
	{
		_items[sku]->favorite = fav;
	}
}

void Items::getItemsFromCsv(string filename)
{
	// Note: partially adapted from https://stackoverflow.com/questions/19936483/c-reading-csv-file

	std::ifstream inFile(filename);

	if (inFile.bad())	// Check for error opening file
	{
		std::cout << "Error reading file: " << filename << std::endl;
		return;
	}

	string line, displayname, fullname, sku, quantStr, mqStr, oqStr, favStr;
	int mq, oq;
	bool fav;

	while (getline(inFile, line))	// Get line from file
	{
		std::istringstream ss(line);	// Split into fields

		getline(ss, displayname, ',');
		getline(ss, fullname, ',');
		getline(ss, sku, ',');
		getline(ss, mqStr, ',');
		getline(ss, oqStr, ',');
		getline(ss, favStr, ',');

		mq = std::stoi(mqStr);
		oq = std::stoi(oqStr);

		if (favStr == "true")
		{
			fav = true;
		}
		else
		{
			fav = false;
		}

		ItemInfo* current = new ItemInfo(sku, displayname, fullname, mq, oq, fav);
		_items.insert({ sku, current });	// Create {sku, itemInfo*} pair & add to map
	}

	inFile.close();
}

ItemInfo* Items::GetItemInfoBySku(string sku)
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

ItemInfo* Items::CreateNewItem(string sku)
{
	string shortName, fullName;
	int minQ, orderQ, favChar;

	std::cin.ignore(1024, '\n');	// Clear input buffer

	std::cout << std::endl << "Item with Sku # " << sku
		<< " not in database.  Please enter the item information:" << std::endl;
	shortName = getString("Display (short) name of item: ");
	fullName = getString("Full name of item: ");
	replace(shortName.begin(), shortName.end(), ',', '_');	// No commas in names b/c csv
	replace(fullName.begin(), fullName.end(), ',', '_');

	minQ = getInt(0, 999, "Minimum amount to keep on hand (0 for occasional only items): ");
	orderQ = getInt(0, 1000, "Order quantity (Amount of item in each box): ");
	char yesNo[2] = { 'y', 'n' };
	favChar = getChar("Mark as favorite item? (y for yes, n for no)", yesNo, 2);
	bool fav = (favChar == 'y') ? true : false;
	printString();

	ItemInfo* newItem = new ItemInfo(sku, shortName, fullName, minQ, orderQ, fav);
	_items[sku] = newItem;

	std::ofstream outfile(ITEMSFILE, std::ios::app);
	outfile << shortName << ',' << fullName << ',' << sku << ',' << minQ << ',' << orderQ 
		<< ',' << (fav ? "true" : "false") << std::endl;
	outfile.close();

	return newItem;
}

