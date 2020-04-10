/******************************************************************************
** Program name: SmileFridge.cpp
** Authors: Jason Moule & Tommy Armstrong & Ryan Jones
** CS361-400-W20
** Date: 02/09/20
** Description: Main file for executing SmileFridge application
******************************************************************************/

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "iohelper.hpp"
#include "Fridge.hpp"
#include "climateEntry.h"
#include "climateControl.h"

// Prototypes
long int findAccountNumber(string userName, string userEmail, string userPassword);

// Main
int main()
{
    srand((unsigned int)time(NULL));
    //Creating menu strings


    int menuChoice = -1;
    int accountChoice;
    string userName, orderJson;
    string userPassword;
    string userEmail;
    int updateAmount;
    string updateSku;
    string viewSku;
    string welcomeMessage = "Welcome to the SmileFridge app!!!\n\n";
    string accountString = "You must log in or create a new account to proceed\n"
                            "1. Log into existing account\n"
                            "2. Create new account\n"
                            "Please enter your choice:";
    string menuString = "\n\nWhat would you like to do?\n"
                        "1. Show SmileFridge contents\n"
                        "2. Show My Favorites\n"
                        "3. View Quantity of Single Item\n"
                        "4. Add New Item to my SmileFridge\n"
                        "5. Remove Item from my SmileFridge\n"
                        "6. Update Quantity of an existing item\n"
                        "7. View SmileFridge Climate control\n"
                        "8. Update Order List and view\n"
                        "9. Receive a shipped order\n"
                        "10. Edit item information\n"
                        "0. Quit the SmileFridge app\n\n"
                        "Please enter your choice:";
    string enterSkuString = "Please enter the sku for which you would like to update the quantity of:";
    string viewSkuString = "Please enter the sku for which you would like to see the quantity of:";
    string skuError = "You cannot enter a SKU that is blank or has spaces. Please try again";
    string updateAmountString = "Please enter the new amount of your item in the fridge:";
    string newSKU;
    int quantity;

    //Initialize climate object to access climate objects.
    climateControl* cC;

    cC = new climateControl;

    //Introducing the program and asking the user to log in or create account
    cout << welcomeMessage;
    accountChoice = getInt(1, 2, accountString);

    //Creating new User object
    User* newUser = new User();
    newUser->setUpAccount(accountChoice);

    //Creating Fridge objects and populating with user's inventory
    Fridge* fridge = new Fridge(newUser);
    map<string, ItemInfo*> items = fridge->GetAllItems();
    vector<FridgeItem*> contents = fridge->GetContents();
    vector<ItemInfo*> favorites = fridge->GetFavorites();



    //Menu loop for user to navigate the app
    while (menuChoice != 0)
    {
        //Displaying menu and asking user what they would like to do
        menuChoice = getInt(0, 10, menuString);


        //Case statement based on user's menu choice
        switch (menuChoice)
        {
        case 1: //Show user's SmileFridge Contents
            cout << "Printing fridge contents:" << endl << endl;
			cout << fridge->stringifyContents();
            break;
        case 2: //Show user's favorites
            cout << endl << "Printing fridge favorites:" << endl << endl;
			cout << fridge->stringifyFavorites();
            break;
        case 3: //View Quantity of item in SmileFridge
            viewSku = getNonEmptyNoSpacesString(viewSkuString, skuError);
            fridge->ViewQuantity(viewSku);
            break;
        case 4: //Add New Item to user's SmileFridge
            cout << endl << "You've chosen to enter a new item. Please enter the required information" << endl << endl;

            cout << "Item SKU: " << endl;
            cin >> newSKU;

            cout << "Quantity: " << endl;
            cin >> quantity;

            fridge->AddItem(newSKU, quantity);
            break;
        case 5: //Remove Item from user's SmileFridge
            updateSku = getNonEmptyNoSpacesString(enterSkuString, skuError);
            fridge->Update(updateSku, 0);
            break;
        case 6: //Update Quantity of an existing item
            updateSku = getNonEmptyNoSpacesString(enterSkuString, skuError);
            updateAmount = getInt(0, 100, updateAmountString);
            fridge->Update(updateSku, updateAmount);
            break;
        case 7: //get Climate control info.

            cC->climateMenu();

            break;
        case 8: //Update orderList and print contents
            fridge->orderLowItems();
            fridge->printOrderList();
            fridge->SubmitOrder();
			contents = fridge->GetContents(); //update vector associated with fridge contents
            break;
        case 9:
            printString();
            orderJson = getString("Enter order filename: ");
            fridge->ReceiveOrder(orderJson);
			contents = fridge->GetContents(); //update vector associated with fridge contents
            break;
        case 10:
            fridge->EditItemMenu();
            break;
        case 0: //Quit the SmileFridge app
            cout << "\nThanks for using the SmileFridge app! Goodbye." << endl;
            break;
        default: 
            cout << "Invalid choice!  Try again." << endl << endl;
            menuChoice = getInt(0, 10, menuString);
        }
    }

    //Deleting user dynamic memory
    delete newUser;


    return 0;
} //End Main