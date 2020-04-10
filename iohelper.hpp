/******************************************************************************
** Program name: iohelper.h
** Author: Jason Moule
** Assignment: Project 1 -  CS162-400-W19
** Date: 1/13/19
** Description: Collection of I/O related helper functions
******************************************************************************/

#ifndef IOHELPER_JM
#define IOHELPER_JM

#include <string>
using std::string;
#include <limits>
#include <vector>
using std::vector;

/// Description: Displays a string to the console
void printString(string display = "", bool endLine = true);

/// Description: Display a int variable and optional label to the console
void printInt(int var, string label = "");

/// Description: Validates that console input from user is an integer
///				with optional string parameter for prompt
int getInt(string prompt = "");

/// Description: Overloaded version of above with minimum and maximum values
int getInt(int min, int max, string prompt = "");

/// Description: Gets a character from cin and validates it against array
/// Note: if useCase is false (default), all chars converted to LOWERCASE
char getChar(string header, char options[], int numOptions, bool useCase = false);

/// Description: Gets a character from cin and validates it against array
/// Note: if useCase is false (default), all chars converted to LOWERCASE
char getChar(string header, vector<char> options, bool useCase = false);

/// Description: gets a char of input from keyboard with optional prompt
char getChar(string prompt = "");

/// Description: Displays 2D array to console
void display2dCharArray(char** arrayIn, int rows, int cols);

/// Description: Displays 2D array to console
/// 				overloaded to include starting and ending values for rows and cols
///				NOTE: this is xth row/col for first and last, not index in array 
void display2dCharArray(char** arrayIn, int firstR, int lastR, int firstC, int lastC);

/// Description: Takes an string for a heading, an array of strings for options and int for 
/// 				length of array and creates a menu, returning the (validated) user choice
int displayMenu(string header, string option[], int numOptions);

/// Description: Takes an string for a heading, an vector of strings for options 
///     and creates a menu, returning the (validated) user choice (index + 1 of vector)
int displayMenu(string header, vector<string> option);

/// Description: gets a string from the keyboard with optional prompt
string getString(string prompt = "");

/// Description: gets a double from the keyboard with specified min and max and optional prompt
double getDouble(double min = std::numeric_limits<double>::min(), double max = std::numeric_limits<double>::max(), string prompt = "");

/// Description: returns right-justifed string of int + leading chars if necessary
string rightJustInt(int value, unsigned int width, char fill = ' ');

/// Description: returns a string of length len containing str in center w/ 
/// leading/trailing fill characters (default = space)
string centerStr(string str, unsigned int len, char fill = ' ');

/// Description: returns string like "+---+" of total length len
string boxTop(unsigned int len, char corner = '+', char line = '-');

/// Description: adds trailing fill characters (default = space) to string up to length len
string leftJustStr(string str, unsigned int len, char fill = ' ');

/// Description: validates that user inputted string is not empty
string getNonEmptyString(string prompt, string error);

/// Description: validates that user inputted string is not empty and has no spaces
string getNonEmptyNoSpacesString(string prompt, string error);

/// Description: validates that string has no spaces
bool doesStringHaveSpaces(string input);

/// Description: Prints a number (default: 40) of empty lines
void printEmptyLines(int lines = 40);
#endif
