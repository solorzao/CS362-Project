/******************************************************************************
** Program name: iohelper.cpp
** Author: Jason Moule	CS162-400-W19
** Last updated: 1/19/19
** Description: Collection of I/O related helper functions
******************************************************************************/

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

#include <limits>

#include "iohelper.hpp"

/// Description: Displays a string to the console
void printString(string display, bool endLine)
{
	cout << display;
	if (endLine)
	{
		cout << endl;
	}
}

/// Description: Display a int variable and optional label to the console
void printInt(int var, string label)
{
	cout << label << " " << var;
}

/// Description: Validates that console input from user is an integer
///				with optional string parameter for prompt (defaults to "")
int getInt(string prompt)
{
	int value;
	cout << prompt << " ";
	while ( true )
	{
		cin >> value;	// Get keyboard input

		if (cin.fail())	// Non numeric input
		{
			cin.clear();			// Clear error state
			cout << "Please enter valid integer: ";
		}
		// Catch input that starts numeric but contains other stuff 
		else if (cin.peek() != '\n')
		{
			cout << "Please enter valid integer: ";
		}
		else	// input is valid int only
		{
			cin.ignore();	// Ignore newline char in cin buffer
			return value;
		}
		// Ignore rest of input buffer
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

/// Description: Overloaded version of above with minimum and maximum values
int getInt(int min, int max, string prompt)
{
	int value = getInt(prompt);
	while (value < min || value > max)
	{
		cout << "Please enter an integer between " << min << " and "
			 << max << " (inclusive): ";
		value = getInt();
	}
	return value;
}

/// Description: Displays 2D array to console
void display2dCharArray(char** arrayIn, int rows, int cols)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			cout << arrayIn[row][col];
		}
		cout << endl;
	}
}

/// Description: Displays 2D array to console
/// 				overloaded to include starting and ending values for rows and cols
///				NOTE: this is xth row/col for first and last, not index in array 
void display2dCharArray(char** arrayIn, int firstR, int lastR, int firstC, int lastC)
{
	for (int row = firstR - 1; row < lastR; row++)		
	{												
		for (int col = firstC - 1; col < lastC; col++)
		{
			cout << arrayIn[row][col];
		}
		cout << endl;
	}
}

/// Description: Takes an string for a heading, an array of strings and int for
///   length of array and creates a menu, returning the (validated) user choice
int displayMenu(string header, string option[], int numOptions)
{
	int choice = 0;
	string spacer = "########################################################";
	cout << endl << spacer << endl << "#" << endl;
	cout << "# " << header << endl << "#" << endl;
	for (int i = 0; i < numOptions; i++)
	{
		cout << "# " << i + 1 << ") " << option[i] << endl;
	}
	cout << "#" << endl << spacer << endl << endl;
	choice = getInt(1, numOptions, "Please Enter your choice: ");
	cout << endl;
	return choice;
}

/// Description: Gets a character from cin and validates it against array
/// Note: if useCase is false (default), all chars converted to LOWERCASE
char getChar(string header, char options[], int numOptions, bool useCase)
{
	while (true)	// Loop until value matches something in options
	{
		char input = getChar(header);	// Get keyboard input

		if (useCase)	// Lower case and upper case treated differently
		{
			for (int i = 0; i < numOptions; i++)
			{
				if (input == options[i])		// return value if it matches an option
				{
					return input;
				}
			}
		}
		else
		{
			input = tolower(input);
			for (int i = 0; i < numOptions; i++)
			{
				if (input == tolower(options[i]))	// return value (as lowercase) if
				{									// it matches a lowercase option
					return input;
				}
			}
		}
		cout << "Please enter one of the following: ";
		for (int i = 0; i < numOptions; i++)
		{
			cout << options[i] << " ";
		}
		cout << endl;
	}
}

char getChar(string header, vector<char> options, bool useCase)
{
    while (true)	// Loop until value matches something in options
    {
        char input = getChar(header);	// Get keyboard input

        if (useCase)	// Lower case and upper case treated differently
        {
            for (unsigned int i = 0; i < options.size(); i++)
            {
                if (input == options[i])		// return value if it matches an option
                {
                    return input;
                }
            }
        }
        else
        {
            input = tolower(input);
            for (unsigned int i = 0; i < options.size(); i++)
            {
                if (input == tolower(options[i]))	// return value (as lowercase) if
                {									// it matches a lowercase option
                    return input;
                }
            }
        }
        cout << "Please enter one of the following: ";
        for (unsigned int i = 0; i < options.size(); i++)
        {
            cout << options[i] << " ";
        }
        cout << endl;
    }
}

/// Description: gets a single character from keyboard with optional prompt
char getChar(string prompt)
{
	char value;
	cout << prompt << " ";
	while (true)
	{
		cin >> value;
		if (cin.fail())	// Non numeric input
		{
			cin.clear();			// Clear error state
			cout << "Please enter valid character: ";
		}
		// Catch input that is longer than one char
		else if (cin.peek() != '\n')
		{
			cout << "Please enter a single character: ";
		}
		else	// input is single valid char only
		{
			cin.ignore();	// Ignore newline char in cin buffer
			return value;
		}
		// Ignore rest of input buffer
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

int displayMenu(string header, vector<string> option)
{
    cout << endl << boxTop(79) << endl << "|" << endl;
    cout << "| " << header << endl << "|" << endl;
    for (unsigned int i = 0; i < option.size(); i++)
    {
        cout << "| " << i + 1 << ") " << option[i] << endl;
    }
    cout << "|" << endl << boxTop(79) << endl << endl;
    int choice = getInt(1, option.size(), "Please Enter your choice: ");
    cout << endl;
    return choice;
}

/// Description: gets a string from the keyboard with optional prompt
string getString(string prompt)
{
	string value;
	cout << prompt << " ";
	getline(cin, value);
	return value;
}

/// Description: gets a double from the keyboard with specified min and max and optional prompt
double getDouble(double min, double max, string prompt)
{
	double value;
	cout << prompt << " ";
	bool validated = false;

	while (!validated)
	{
		cin >> value;
		if (cin.fail())	// Non numeric input
		{
			cin.clear();			// Clear error state
			cout << "Please enter valid number: ";
		}

		// Catch input that starts numeric but contains other stuff 
		else if (cin.peek() != '\n')
		{
			cout << "Please enter valid number: ";
		}

		else	// input is valid number
		{
			if (value < min || value > max)	// Test if in limits
			{
				cout << "Please enter an number between " << min << " and "
					<< max << " (inclusive): ";
			}
			else	// input valid and in range!
			{
				cin.ignore();	// Ignore newline char in cin buffer
				return value;
			}
		}
		// Ignore rest of input buffer
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return -999.99;	// Debug return (should never get here)
}

// Description: returns right-justifed string of int + leading chars if necessary
string rightJustInt(int value, unsigned int width, char fill)
{
	string result = std::to_string(value);
	while (result.size() < width)
	{
		result = fill + result;
	}
	return result;
}

string centerStr(string str, unsigned int len, char fill)
{
    string result = str;
    int lead = (len - result.length()) / 2;
    for (int i = 0; i < lead; ++i)
    {
        result = fill + result;   // Add leading fill chars
    }
    while (result.length() < len)
    {
        result += fill;        // Add trailing fill chars
    }
    return result;
}

string boxTop(unsigned int len, char corner, char line)
{
    string result = "";
    result += corner;
    if (len == 0)
    {
        return "";
    }
    else if (len == 1)
    {
        return result;
    }
    for (unsigned int i = 0; i < len - 2; ++i)
    {
        result += line;
    }
    result += corner;
    return result;
}

string leftJustStr(string str, unsigned int len, char fill)
{
    string result = str;
    while (result.length() < len)
    {
        result += fill;
    }
    return result;
}

bool doesStringHaveSpaces(string input)
{
	bool stringHasSpaces = false;

	for (unsigned i = 0; i < input.length(); i++)
	{
		if (input[i] == ' ')
		{
			stringHasSpaces = true;
		}
	}

	return stringHasSpaces;
}

// Prints a number (default: 40) of empty lines to console
void printEmptyLines(int lines)
{
	for (int i = 0; i < lines; ++i) printString();
}


string getNonEmptyString(string prompt, string error)
{
	string value;

	cout << prompt << " ";
	getline(cin, value);

	while (value.length() == 0)
	{
		cout << error << " ";
		getline(cin, value);
	}

	return value;
}

string getNonEmptyNoSpacesString(string prompt, string error)
{
	string value;
	bool stringHasSpaces = false;

	cout << prompt << " ";
	getline(cin, value);
	stringHasSpaces = doesStringHaveSpaces(value);

	while (value.length() == 0 || stringHasSpaces == true)
	{
		cout << error << " ";
		getline(cin, value);
		stringHasSpaces = doesStringHaveSpaces(value);
	}

	return value;
}