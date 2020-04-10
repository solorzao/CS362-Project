/******************************************************************************
** Program name: SmileFridge.cpp
** Authors: Ryan Jones
** CS361-400-W20
** Date: 02/09/20
** Description: User class file for creating new users, logging in existing
**				users, finding/creating the correct csv file for that user
**				and returning user information if needed.
******************************************************************************/

#include "User.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "iohelper.hpp"
#include <ostream>

using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::istringstream;
using std::to_string;
using std::vector;

string enterNameString = "Please enter a name:";
string nameError = "Your name cannot be blank. Please try again:";
string enterEmailString = "Please enter an email address:";
string emailError = "Your email cannot be blank or contain spaces and must be in the form email@something.com. Please try again:";
string enterPasswordString = "Please enter a password:";
string passwordError = "Your password cannot be blank or contain spaces. Please try again:";
string existingAccountError = "An account with that username already exists, please choose another";
string noAccountError = "No account matches those credentials, please try again";
string duplicateAccountError = "That account already exists, please choose new credentials";
string duplicateEmailError = "That email is already linked to an account, please try again";
string loginSuccess = "\nYou have been logged in successfully!";
string accountCreationSuccess = "\nYou account has been successfully created!";


//*****************************************************************************************
//***createAccount method***															***
//***This method creates a new user account by either creating the accounts file		***
//***that stores all user account information or opening the existing accounts file and ***
//***adding the new user's information. It also creates a new csv file named after the  ***
//***newly created account number for storing inventory information in later.			***
//*****************************************************************************************
void User::createAccount()
{
	//Opening accounts file that contains all user information
	ifstream existingAccountFile("accounts.csv");

	if (!existingAccountFile.is_open())	// Check if file exists
	{
		//accounts.csv file doesn't exist, create it by 'opening it'
		fstream accountFile;
		accountFile.open("accounts.csv", std::fstream::out);

		//Set first account number
		SetAccount(1000000);
		accountFile << GetAccount() << "," << GetEmail() << "," << GetName() << "," << GetPassword();

		//Close accounts file
		accountFile.close();

	}
	else
	{
		//Extracting last line of accounts file
		string accountFileLastLine;
		int lastAccountNumber;
		while (!existingAccountFile.eof())
		{
			getline(existingAccountFile, accountFileLastLine);
		}

		//Extracting last created account number
		istringstream ss(accountFileLastLine);
		ss >> lastAccountNumber;
		
		//Open accounts.csv for appending new user information
		fstream accountFile;
		accountFile.open("accounts.csv", std::fstream::app);

		//Set new account number
		SetAccount(lastAccountNumber + 1);
		accountFile << "\n" << GetAccount() << "," << GetEmail() << "," << GetName() << "," << GetPassword();
		accountFile.close();
	}

	//Creating account specific file for inventory tracking
	ofstream accountSpecificFile;
	accountSpecificFile.open(to_string(GetAccount()) + ".csv");
	accountSpecificFile.close();

	//Close existing accounts file
	existingAccountFile.close();

}

//*****************************************************************************************
//***findAccountNumber method***														***
//***This method attempts to find a user account by searching each line of the accounts ***
//***csv file for the user entered credentials. If the credentials match a line in the  ***
//***csv file, then the account number is returned. If the email entered matches an     ***
//***email in one of the rows but not everything else matches, a 0 is returned. Fianlly,***
//***if no account is matched by the user entered credentials, a -1 is returned.        ***
//*****************************************************************************************
long int findAccountNumber(string userName, string userEmail, string userPassword)
{
    //Opening accounts file that contains all user information
    ifstream existingAccountFile("accounts.csv");

    if (!existingAccountFile.is_open())	// Check if file exists
    {
        //if accounts.csv file doesn't exist, then user account can't exist
        return -1;
    }
    else
    {
        //Creating variables for searching file
        string nextAccountLine;
        long int accountNumber;
        string newUserName;
        string newUserEmail;
        string newUserPassword;
        string nextString;
        vector<string> row;

        //Searching account.csv file for a matching account
        while (!existingAccountFile.eof())
        {
            row.clear();

            //Getting each line of csv and adding separated values to vector
            getline(existingAccountFile, nextAccountLine);
            istringstream ss(nextAccountLine);

            while (getline(ss, nextString, ','))
            {
                row.push_back(nextString);
            }

            //Storing account information to variables
            accountNumber = stoi(row[0]);
            newUserEmail = row[1];
            newUserName = row[2];
            newUserPassword = row[3];

            //Account was found
            if (newUserName == userName && newUserEmail == userEmail && newUserPassword == userPassword)
            {
                return accountNumber;
            }
            else if (newUserEmail == userEmail) //If email has been used before but entire account doesn't match
            {
                return 0;
            }
        }

        //Account was not found
        return -1;
    }

}


//*****************************************************************************************
//***setUpAccount method***														        ***
//***This method asks the user for their login credentials (name, email and password)   ***
//***and depending on the accountChoice input, either tries to find their account or    ***
//***creates them a new account.
//*****************************************************************************************
void User::setUpAccount(int accountChoice)
{
    bool foundAccount = false;
    long int accountNumber = -1;

    //Asking user to log in or create account
    if (accountChoice == 1) //Login
    {
        while (!foundAccount)
        {
            //Asking user for log in information
            _name = getNonEmptyString(enterNameString, nameError);
            _email = getNonEmptyNoSpacesString(enterEmailString, emailError);
            _password = getNonEmptyNoSpacesString(enterPasswordString, passwordError);
            _account = findAccountNumber(_name, _email, _password);

            //Checking to see if account was found. If not, re-prompting user for credentials
            if (_account > 0)
            {
                foundAccount = true;
                system("CLS");
                cout << loginSuccess << std::flush;
            }
            else
            {
                cout << noAccountError << endl;
            }
        }
    }
    else //Create account
    {
        foundAccount = true;
        while (foundAccount)
        {
            //Asking user for account creation information
            _name = getNonEmptyString(enterNameString, nameError);
            _email = getNonEmptyNoSpacesString(enterEmailString, emailError);
            _password = getNonEmptyNoSpacesString(enterPasswordString, passwordError);
            _account = findAccountNumber(_name, _email, _password);

            //Checking to see if account was found. If so, re-prompting user for credentials
            if (_account > 0)
            {
                cout << duplicateAccountError << endl;
            }
            else if (_account == 0)
            {
                cout << duplicateEmailError << endl;
            }
            else
            {
                foundAccount = false;
                system("CLS");
                cout << accountCreationSuccess << std::flush;
            }
        }

    }


    //Creating new account
    if (accountChoice != 1)
    {
        createAccount();
    }
}

