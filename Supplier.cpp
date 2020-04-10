#include "Supplier.hpp"
#include "Fridge.hpp"
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

Supplier::Supplier()
{
    _items = Items().GetAll();
}

string Supplier::GetFridgeJson(vector<FridgeItem*> items, string listName)
{
    string result = "{\"" + listName + "\": [";
    for (FridgeItem* i : items)
    {
        result += "{\"sku\": \"" + i->itemInfo->sku + "\", ";
        result += "\"name\": \"" + i->itemInfo->displayName + "\"}, ";
    }
    result.erase(result.end() - 2, result.end());   // erase last comma and space
    result += "]}\n";
    return result;
}

string Supplier::PrettyPrintJson(string json)
{
    string result = "";
    int tabs = 0;
    string insertStr;
    for (size_t i = 0; i < json.size(); ++i)
    {
        if (json[i] == '{' || json[i] == '[')
        {
            ++tabs;
            result += json[i];
            ppj_newline(result, tabs);
        }
        else if (json[i] == '}' || json[i] == ']')
        {
            --tabs;
            ppj_newline(result, tabs);
            result += json[i];
            if (json[i + 1] == ',')
            {
                result += ',';
                ++i;
            }
            ppj_newline(result, tabs);
        }
        else if (json[i] == ',')
        {
            result += ',';
            ppj_newline(result, tabs);
        }
        else
        {
            result += json[i];
        }
    }
    return result;
}

void Supplier::ppj_newline(string &result, int tabs)
{
    result += '\n';
    for (int j = 0; j < tabs; ++j)
    {
        result += "\t";
    }
}

void Supplier::addToOrderLog(string listJson)
{

}

string Supplier::makeFakeOrder(string orderListJson)
{
    string orderNumber = "112-8639332-37";
    for (int _ = 0; _ < 5; ++_) // need to do loop or leading zeros disappear
    {
        orderNumber += std::to_string(rand() % 10); // Fake order number, random last 5 digits
    }

    // Save the order JSON to file as if received from internet
    std::ofstream saveOrderFile("order" + orderNumber + ".json");
    saveOrderFile << "{\"orderNumber\": \"" + orderNumber + "\", \"shipmentDate\": \"";

    string dateStr = GetDateString(AddDaysToDate(GetCurrentDate(), 2)); // 2 day shipping
    //std::replace(dateStr.begin(), dateStr.end(), '/', '_');    // Replace /s with _s
    saveOrderFile << dateStr << "\", \"total\": \"$";

    std::ostringstream amount;  // To fixed double precision issue
    amount << std::fixed;
    amount << std::setprecision(2);
    amount << 99.99;
    saveOrderFile << amount.str() << "\", ";

    orderListJson.erase(0, 1);  // Erase begining '{' of orderlist, keep trailing for our own use
    saveOrderFile << orderListJson << std::endl;

    saveOrderFile.close();

    return orderNumber;
}

string Supplier::GetOrderListJson(map<string, int> list)
{
    string result = "{\"order\": [";
    for (std::map<string, int>::iterator it = list.begin(); it != list.end(); it++) {
        result += "{\"" + it->first + "\": \"" + std::to_string(it->second) + "\"}, ";
    }
    result.erase(result.end() - 2, result.end());
    result += "]}";
    return result;
}

void Supplier::CreateOrder(User* user, map<string, int> orderList)
{
    // NOTE: at some point an API token will need to replace sending clear text account info
    string orderJson = "{{\"account\": ";
    string userJson = "{\"Number\": \"" + std::to_string(user->GetAccount()) + "\", \"Email\": \"" +
        user->GetEmail() + "\", \"Password\": \"" + user->GetPassword() + "\"}";
    string listJson = GetOrderListJson(orderList);
    orderJson += userJson + ", " + listJson + "}";

    // TODO: network code to send orderJson to Amazon (There is no _public_ API for this.)
    string orderNumber = makeFakeOrder(listJson);   // Creates a fake order for now

    // Add order to order log file
    std::ofstream logFile(ORDERLOG, std::ofstream::app);
    logFile << orderNumber << "," << GetDateString(GetCurrentDate()) << "," << listJson << std::endl;
    logFile.close();
}

vector<FridgeItem*> Supplier::ProcessReceivedOrder(string orderJson)
{
    vector<FridgeItem*> items;
    string jsonIn, orderNumber, date, total, itemList, item, sku, quant;

    std::ifstream order(orderJson);
    std::getline(order, jsonIn);

    std::istringstream ss(jsonIn);	// Split and read data
    ss.ignore(256, ' '); ss.ignore();   // ignore `{"orderNumber": "`
    getline(ss, orderNumber, '\"');
    ss.ignore(256, ':'); ss.ignore(2);
    getline(ss, date, '\"');
    ss.ignore(256, ':'); ss.ignore(2);
    getline(ss, total, '\"');
    ss.ignore(256, ':'); ss.ignore(1);
    getline(ss, itemList, ']');
    itemList += ']';

    order.close();

    std::ofstream logfile(RECVLOG, std::ios::app);
    logfile << orderNumber << ',' << date << ',' << total << ',' << itemList << std::endl;
    logfile.close();

    std::istringstream list(itemList);
    list.ignore(1);
    while (getline(list, item, ','))
    {
        std::istringstream iss(item);
        std::cout << "CURRENT ITEM SCAN: " << item << std::endl;

        iss.ignore(5, '\"');
        getline(iss, sku, '\"');
        iss.ignore(5, '\"');
        getline(iss, quant, '\"');

        ItemInfo* itemInfo = Items().GetItemInfoBySku(sku);
        if (itemInfo == nullptr)
        {
            itemInfo = Items().CreateNewItem(sku);  // Creates new item from prompts & saves it
        }
        items.push_back(new FridgeItem(itemInfo, std::stoi(quant)*itemInfo->orderQuantity, 0, GetDateFromString(date)));
    }

    return items;
}