/* Author: Christian Ropchock
Date: 02-07-2018
Class: EECS 2510 -- Non-Linear Data Structures

Description: Header file that defines all of the public/private members of the Huffman.cpp file.

*/

#pragma once
#include <string>

using namespace std;

class Huffman
{
public:
	Huffman();  // Constructor for the Huffman Tree
	string MakeTreeBuilder(string inputFile, string outputFile);
	string EncodeFile(string inputFile, string outputFile);
	string DecodeFile(string inputFile, string outputFile);
	string EncodeFileWithTree(string inputFile, string treeFile, string outputFile);
	string verifyCommandLineInput(string command, string inputFile1, string inputFile2, string inputFile3);
	string DisplayHelp();

private:
	struct HuffmanNode
	{
		// This struct defines a node to the Huffman Tree. Each leaf will contain a symbol and weight of that symbol
		// which corresponds to the symbols number of occurences in any type of ASCII text file, and the non-leaves contain 
		// only the weights. The left child and right child pointers point to the left and right children of the node that 
		// is designated as the parent node. 

		char symbol = NULL;
		int weight = 0;
		HuffmanNode* leftChild;
		HuffmanNode* rightChild;
	};

	HuffmanNode* root; // Used to track the root of the Huffman Table

	streampos fileReadSize, fileWriteSize;

	int populateTable(string inputFile); // Method to populate the table with the ASCII integer representations

	int findMin1();                 // Find first min in the array
	int findMin2(int min1);    // Find second min in the array, making sure not to pick up min1 again
	int saveTableCount = 0;                  // Used to track location in the saveTable

	void createHuffmanTree(); // Method used to create the Huffman Tree
	string bitSequence = "";   // Used to track the bit string in each traversal of the BST for each character
	void traverseTree(char character, HuffmanNode* p);
	bool exitRecursion = true; // Used to exit the recursion when the character is found
	int outputToFile(string inputFile, string outputFile); // Used to output to the textfile as designated by outputFile

	unsigned char bitsToChar(string bitByte); // Method takes an 8-bit binary string, and converts to a char for writing to a file
	unsigned char addPadding(string bitByte); // Method will add padding to the last byte stream from a file read

	int table[256] = { 0 }; // Array that stores the occurence of ASCII characters associated with building the Huffman Tree
	HuffmanNode* huffTable[256]; // Array that stores all 256 leaves to the Huffman Tree
	string bitTable[256]; // Array used later in the method to store the bit representation of each character as represented by the Huffman Tree
	unsigned char saveTable[510]; // Array that stores the order of building the Huffman Tree
};
