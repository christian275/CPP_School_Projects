/* Author: Christian Ropchock
Date: 02/18/18
Class: EECS 2510 - Non-Linear Data Structures

Description: The Huffman Tree is a type of high prority Binary Search Tree that is built based on the highest
number of occurences of a word in a text file. The table is built in a backwards fashion starting from the leaves,
and is built upwards until the root is established. The branches of the Huffman Tree are each associated with a binary
value, left zero and right one. When a text file is encoded, each ASCII character gets its own binary representation, that
is built into a large binary string that is then converted to 8-bit ASCII characters and stored in a text file. The Huffman
Tree serves as the decoder, because the binary digits are then read back through the branches of the tree to decode a file.

This tree serves the purpose of encoding and decoding any standared 8-bit ASCII text file, which is compatible with user end
decoding.
*/

#include "stdafx.h"
#include <iostream>

#include "Huffman.h"

using namespace std;

int main(int argc, char*argv[])
{
	
	Huffman* huffTree = new Huffman(); // New instance of a Huffman Tree

	string commands[5]; // Array of strings that holds the command line command and parameters, Index 0 = HUFF, Index 1 = Commands Switch Statement, Index 2-4 = Files 1-3 respectively

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] == NULL) commands[i] = ""; // Since NULL is #define as 0, store empty string for NULL
		else                 commands[i] = argv[i]; // Else store the command
	}

	if (commands[1] == "-h" || commands[1] == "-?" || commands[1] == "-help") cout << huffTree->DisplayHelp() << endl; // Method call to display help

	else if (commands[1] == "-e" || commands[1] == "-E")
	{
		string file1 = commands[2];
		string file2 = commands[3];

		file2 = huffTree->verifyCommandLineInput(commands[1], commands[2], commands[3], commands[4]); // Check that the file input is correct

		if (file2 == "-1") // If error code one is returned instead of file2 name, then there are not enough files specified in the command line
		{
			cout << "Error: Not enough files specified." << endl;
			return -1;
		}
		if (file2 == "-2") // If file2 is -2, then file1 and file2 were the same file name
		{
			cout << "Error: File1 and File2 can't be the same files." << endl;
			return -1;
		}

		cout << huffTree->EncodeFile(file1, file2) << endl;
	}
	else if (commands[1] == "-d" || commands[1] == "-D") // Interprets the decoding method
	{
		string file1 = commands[2];
		string file2 = commands[3];

		string message = huffTree->verifyCommandLineInput(commands[1], commands[2], commands[3], commands[4]); // Check that the file input is correct

		if (message == "") cout << huffTree->DecodeFile(commands[2], commands[3]) << endl;
		else               cout << message << endl;
	}
	else if (commands[1] == "-t" || commands[1] == "-T") // Interprets the making of the tree file
	{
		string file1 = commands[2];
		string file2 = commands[3];

		file2 = huffTree->verifyCommandLineInput(commands[1], commands[2], commands[3], commands[4]); // Check that the file input is correct

		if (file2 == "-1")
		{
			cout << "Error: File1 and File2 can't be the same files." << endl;
			return -1;
		}

		cout << huffTree->MakeTreeBuilder(file1, file2) << endl;
	}
	else if (commands[1] == "-et" || commands[1] == "-ET") // Interprets the decoding with a specified tree file
	{
		string file1 = commands[2];
		string file2 = commands[3];
		string file3 = commands[4];

		file3 = huffTree->verifyCommandLineInput(commands[1], commands[2], commands[3], commands[4]); // Check that the file input is correct

		if (file3 == "-1")
		{
			cout << "Error: Not enough files specified." << endl;
			return -1;
		}

		cout << huffTree->EncodeFileWithTree(file1, file2, file3) << endl;
	}
	else
	{
		cout << "Invalid command input."; // Default output if any mistyped commands
		huffTree->DisplayHelp();
	}

	return 0;
}

