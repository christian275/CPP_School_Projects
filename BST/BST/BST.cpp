/* Author: Christian Ropchock
   Date: 02-07-2018
   Class: EECS 2510 -- Non-Linear Data Structures

   This CPP file is where the MAIN method resides. This method will be subjected to a loop that is continously 
   prompting for user input. The user will have the option to perform several operations on a Binary Search Tree
   including: Insert, Delete, Search, Min, Max, Next, Prev, List, Help, and Exit to terminate the loop.
*/


#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

#include "BST_Model.h"

using namespace std;

int main()
{
	// Create an infinite while loop that is constantly prompting for user input until the user requests to exit the program by terminating the 
	// loop by means of entering "Exit". During each iteration of the loop, match up the user input with the corresponding method in BST_Model.cpp. 
	// If the user input isn't an option, specify to the user "Input is Invalid. Enter "Help" for a list of possible commands.".
	
	BST_Model* BST = new BST_Model(); // Declare and initialize a new Binary Search Tree of the class BST_Model

	while (true)
	{

		string cmdInput;
		string command; // This string stores the input from the user
		string input; // This string stores a string input if applicable to the command

		cout << "Enter a command (Format <Command>_<Word if applicable>): ";
		getline(cin,cmdInput); // Get the entire line of input

		istringstream iss (cmdInput); // Create a stream that reads through the string, and delimits by space

		for (int i = 0; i < 2; i++) // Loop twice and get only the first two words, any other input after isn't valid
		{
			if (i == 0) iss >> command; // If the first word, then it's the command
			else iss >> input;          // If there is a second word, then it's an input to a command method
		}

		transform(command.begin(), command.end(), command.begin(), (int(*)(int))tolower); // This command is used out of the algorithm header file, it reads in the string in the first two parameters. The third parameter defines the starting position for the output iterator, and the 
                                                                                // fourth parameter defines the type to convert the output to, and in this instance it converts each char pointer in the string to lower case as the iterator progresses.
		
		if (input == "")
		{
			if (command == "min")
			{
				cout << BST->min() << endl;
			}
			else if (command == "max")
			{
				cout << BST->max() << endl;
			}
			else if (command == "list")
			{
				BST->list();
			}
			else if (command == "help")
			{
				BST->help();
			}
			else if (command == "exit")
			{
				BST->terminate();
			}
			else
			{
				// If there is not input, and one of the methods wans't used
				cout << "Input is Invalid. Enter \"Help\" for a list of possible commands." << endl;
			}
		}
		else
		{
			if (command == "insert")
			{
				BST->insert(input);
			}
			else if (command == "delete")
			{
				cout << BST->remove(input) << endl;
			}
			else if (command == "search")
			{
				if (input != "")
					cout << BST->search(input) << endl;
				else
					cout << "Invalid formatting for the search command." << endl;
			}
			else if (command == "next")
			{
				cout << BST->next(input) << endl;
			}
			else if (command == "prev")
			{
				cout << BST->prev(input) << endl;
			}
			else
			{
				// If there is input, and one of the methods wans't used
				cout << "Input is Invalid. Enter \"Help\" for a list of possible commands." << endl;
			}
		}

	}

    return 0;
}

