/* Author: Christian Ropchock
Date: 02/18/18
Class: EECS 2510 - Non-Linear Data Structures

Description: This class defines a Huffman Tree, and all of the public/private methods that are avaiable for the Huffman 
Tree. The five basic operations a Huffman Tree should be able to perform are to print out it's use, encode any type of file
of text, decode the encoded text file, create a file that represents the process to build a Huffman Tree, and encode a file 
with any type of tree building file. Each method that the user can access outputs either a help output that specifies to the 
user how they can use each option on the command line, or the metrics involved with each process.

*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

#include "Huffman.h"

using namespace std;

Huffman::Huffman()
{
// Constructor for the Huffman Tree

}

string Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
// Method used to create the tree builder file (.htree). This file is used to decode any type of encoded (.huf) or user specifed extension file
// that was encoded via a Huffman Tree.

	clock_t time; // Create the variable to track the time
	time = clock(); // Capture start time 

	ofstream fout(outputFile, ios::binary); // Output file stream to output the new tree builder array to the new tree file

	if (populateTable(inputFile) == -1) 
		return "The input file specified wasn't found.";

	createHuffmanTree(); // Create the Huffman Tree

	root = huffTable[0]; // Set the root to point to the Huffman Tree's root in the array

	for (int i = 0; i < 256; i++) // Go through all 256 nodes in huffTable
	{
		bitSequence = ""; // Set bitSequence to zero, which is used to set the bit string representation of each char from the Huffman Table
		exitRecursion = true;

		traverseTree((unsigned char)i, root); // Traverse the tree to look for the char

		bitTable[i] = bitSequence; // Set the ASCII character, ASCII i, to it's binary string representation

	}

	fout.write((char *)saveTable, 510); // Write the 510 bytes for the tree building array

	fileWriteSize = fout.tellp(); // Read size of output file

	fout.close(); // Close the file reader

	time = clock() - time; // Capture elapsed time

	return to_string(fileReadSize) + " bytes read; " + to_string(fileWriteSize) + " bytes written in " + to_string(time/CLOCKS_PER_SEC) + "." +
		   to_string(time / (CLOCKS_PER_SEC / 1000)).substr(0,3) + " seconds";
}

string Huffman::EncodeFile(string inputFile, string outputFile)
{
	// Method that encodes a text file, and writes the results into a second file with extension .huf if no other extension is provided. 
	// This method will traverse the 8-bit ASCII text file and create a Huffman Tree based on the number of occurences of a character within the text file. The tree is to have up to 256 roots to define each ASCII character that is avaialable within 
	// the text file. The table that tracks the occurence of each character, is to be an array of ints that will map it's indecies to the correlating ASCII character. After the tree is built, go through
	// the table and create the tree building data by traversing through each of the lowest occurences of a character, and combining in a second table, until all of the characters are combined in the 0th 
	// index, resulting in the root. Save the tree building numbers in the first 510-bytes of the encoded file, then the bit stream representing the text file afterwards.

	clock_t time; // Create the variable to track the time
	time = clock(); // Capture start time 

	if (populateTable(inputFile) == -1) // Populate the frequency count table from the input file
		return "The input file specified wasn't found.";

	createHuffmanTree(); // Create the Huffman Tree

	// Use the Huffman Tree to write to a text file that is designated by the user, or default to fileName1.huf. To do so, traverse the text again, and while doing so, look up each individual char in the Huffman Tree and 
	// capture the bit string representation of that character. Keep doing this process until 8 bits are captured, then store into a byte, and output to the text file. When an EOF is encountered, pad the last bits to the 
	// current bit string until 8 bits is captured by getting the first n-bits needed to fill out of the largest bit sequence in the list. 

	root = huffTable[0]; // Set the root to point to the Huffman Tree's root in the array

	for (int i = 0; i < 256; i++) // Go through all 256 nodes in huffTable
	{
		bitSequence = ""; // Set bitSequence to zero, which is used to set the bit string representation of each char from the Huffman Table
		exitRecursion = true;

		traverseTree((unsigned char)i, root); // Traverse the tree to look for the char

		bitTable[i] = bitSequence; // Set the ASCII character, ASCII i, to it's binary string representation
	}

	// Write the saveTable sequence out to text file outputfile. 
	// Traverse the inputFile character by character, and use the table to lookup the bit representation of the ASCII value and write out to the outputFile byte by byte (8-bits). Make sure to pull 
	// the padding amount off a non-ASCII character in the bitTable. 

	outputToFile(inputFile, outputFile);

	time = clock() - time; // Capture elapsed time

	return to_string(fileReadSize) + " bytes read; " + to_string(fileWriteSize) + " bytes written (" + to_string(((float)(fileReadSize - fileWriteSize)/(float)fileReadSize) * 100).substr(0,5) +
		   "% compression) in " + to_string(time / CLOCKS_PER_SEC) + "." + to_string(time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) + " seconds";
}

string Huffman::DecodeFile(string inputFile, string outputFile)
{
// Method that will decode any type of (.huf) file, or any file of the form, 510 chars (Huffman Tree Builder) + 
// File data formatted to be ASCII characters. Starts by building the Huffman Tree from the first 510 chars, and
// uses the tree to write the byte data back out to an output file that will contain the original text.

	clock_t time; // Create the variable to track the time
	time = clock(); // Capture start time 

	// Create all 256 nodes with their appropriate symbols
	for (int i = 0; i < 256; i++)
	{
		huffTable[i] = new HuffmanNode();
		huffTable[i]->symbol = (unsigned char)i;
	}
	
	// Rebuild the Huffman Tree with the tree builder information and blank tree nodes
	ifstream fin(inputFile, ios::binary);
	ofstream fout(outputFile, ios::binary);

	for (int i = 0; i < 510; i += 2) // Traverse through all 510 chars used to define the process for rebuilding the tree
	{

		saveTable[i] = (unsigned char)fin.get();

		saveTable[i + 1] = (unsigned char)fin.get();

		HuffmanNode* newRoot = new HuffmanNode();

		newRoot->leftChild = huffTable[(int)saveTable[i]]; // Set the left child to the current node
		newRoot->rightChild = huffTable[(int)saveTable[i + 1]]; // Set the right child to the node that comes after the current node in the saveTable ordering

		huffTable[(int)saveTable[i]] = newRoot; // The current node was the one that was replaced by the new subtree
		huffTable[(int)saveTable[i + 1]] = NULL;  // The node that followed it was the one that was set to NULL

	}

	root = huffTable[0];

	// Get each one of the bytes from the encoded file, and traverse the tree after converting the byte back to it's binary represented string. 
	// During the traversal, each time a leaf is reached, write out it's character to an output file, and everytime there are no more bits to use, 
	// fetch more bits from another character in the encoded file. Make sure that when the end of file is reached, keep going through the traversal
	// until there are no more bits.

	HuffmanNode* currentNode = new HuffmanNode();
	currentNode = root; // Start traversal at the root

	int nextChar = (unsigned char)fin.get(); // Gets the next char in the file
	//int binaryMult = 128;

	while (!fin.eof())
	{

		int powersOfTwo[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
		for (int i = 0; i < 8; i++) // Go through every character in the bit string
		{
			if (currentNode->leftChild == NULL && currentNode->rightChild == NULL) // If a leaf is found
			{
				fout.put(currentNode->symbol); // Write it's contents to the output file
				currentNode = root; // Set the current pointer back to the root
				continue;
			}
			currentNode = (nextChar & powersOfTwo[i]) ? currentNode->rightChild : currentNode->leftChild;			
		}
		nextChar = (unsigned char)fin.get(); // Gets the next char in the file		
	}

	fin.seekg(0, ios::end); // Read through rest of extra white characters
	fileReadSize = fin.tellg(); // Set the read size
	fileWriteSize = fout.tellp(); // Set the write size

	fin.close();
	fout.close();

	time = clock() - time; // Capture elapsed time
	return to_string(fileReadSize) + " bytes read; " + to_string(fileWriteSize) + " bytes written (" + to_string(((float)(fileWriteSize - fileReadSize) / (float)fileWriteSize) * 100).substr(0, 5) +
		"% compression) in " + to_string(time / CLOCKS_PER_SEC) + "." + to_string(time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) + " seconds";
}

string Huffman::EncodeFileWithTree(string inputFile, string treeFile, string outputFile)
{
// Method that takes the file that defines a Huffman Tree creation process, builds another identical instance of that Huffman Tree,
// and uses the Huffman Tree to encode a text file, dropping the result into a .huf encoded file, or any other extension as defined
// by the user.

	clock_t time; // Create the variable to track the time
	time = clock(); // Capture start time 

	// Create all 256 nodes with their appropriate symbols
	for (int i = 0; i < 256; i++)
	{
		huffTable[i] = new HuffmanNode();
		huffTable[i]->symbol = (unsigned char)i;
	}

	// Rebuild the Huffman Tree with the tree builder information from the .htree file, or extension defined by the user
	ifstream fin(treeFile, ios::binary);

	for (int i = 0; i < 510; i += 2) // Traverse through all 510 chars used to define the process for rebuilding the tree
	{
		saveTable[i] = (unsigned char)fin.get();

		saveTable[i + 1] = (unsigned char)fin.get();

		HuffmanNode* newRoot = new HuffmanNode();

		newRoot->leftChild = huffTable[(int)saveTable[i]]; // Set the left child to the current node
		newRoot->rightChild = huffTable[(int)saveTable[i + 1]]; // Set the right child to the node that comes after the current node in the saveTable ordering

		huffTable[(int)saveTable[i]] = newRoot; // The current node was the one that was replaced by the new subtree
		huffTable[(int)saveTable[i + 1]] = NULL;  // The node that followed it was the one that was set to NULL
	}

	root = huffTable[0];

	fin.close();

	for (int i = 0; i < 256; i++) // Go through all 256 nodes in huffTable
	{
		bitSequence = ""; // Set bitSequence to zero, which is used to set the bit string representation of each char from the Huffman Table
		exitRecursion = true;

		traverseTree((unsigned char)i, root); // Traverse the tree to look for the char

		bitTable[i] = bitSequence; // Set the ASCII character, ASCII i, to it's binary string representation
	}

	// Write the saveTable sequence out to text file outputfile. 
	// Traverse the inputFile character by character, and use the table to lookup the bit representation of the ASCII value and write out to the outputFile byte by byte (8-bits). Make sure to pull 
	// the padding amount off a non-ASCII character in the bitTable. 

	outputToFile(inputFile, outputFile);

	time = clock() - time; // Capture elapsed time

	return to_string(fileReadSize) + " bytes read; " + to_string(fileWriteSize) + " bytes written (" + to_string(((float)(fileReadSize - fileWriteSize) / (float)fileReadSize) * 100).substr(0, 5) +
		"% compression) in " + to_string(time / CLOCKS_PER_SEC) + "." + to_string(time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) + " seconds";

}

string Huffman::DisplayHelp()
{
// Method that returns the help menu
	return "\n***The proper uses of HUFF.exe are outlined as shown below***\n\nOption 1: HUFF <-h><-?><-help> : Shows the proper uses of HUFF.exe and exits.\n\n" 
		"Option 2: HUFF <-e> <fileName1> [fileName2]: (fileName2 is optional) Encodes fileName1 and puts the output into fileName2. The default extention of fileName2 "
		"is fileName1.huf, unless another extension is designated on the end of fileName2.\n\nOption 3: HUFF <-d> <fileName1> <fileName2>: Decode fileName1, and put the"
		" results into fileName2.txt as specified. The file fileName2 is to be bit identical to it's original input file.\n\nOption 4: HUFF <-t> <fileName1> [fileName2]: "
		"This option will create a 510-byte file (fileName2) that is used to decode the original input file (fileName1). Default -- fileName1.htree, unless otherwise specified."
		"(fileName2 is optional).\n\nOption 5: HUFF <-et> <fileName1> <fileName2> [fileName3]: File named fileName1 will be decoded using the decoding algorithm from fileName2 "
		"and will be written into fileName3. The file fileName3 is optional, and the output will default to dumping the results into a file named fileName1.huf.\n";
}

int Huffman::populateTable(string inputFile)
{
// Method is used to modularize the code by doing the process of traversing the text file char by char, and populating the ASCII table based on number of occurences of the char in the inputFile
	ifstream fin(inputFile, ios::binary);

	fin.seekg(0, ios::end); // Put the get position at the last char in the file, to get size
	fileReadSize = fin.tellg(); // Read the size of the input file
	fin.clear(); // Restart get position
	fin.seekg(0, ios::beg);

	if (fin.fail()) // Return and error code -1 if the file failed to open
		return  -1;

	while (!fin.eof())
	{
		table[(int)fin.get()]++; // Grab the character and convert to ASCII representation, then use the ASCII numerical value to update that index in the table
	}

	for (int i = 0; i < 256; i++)
	{
		huffTable[i] = new HuffmanNode(); // Create new instance of a node pointer in the array
		huffTable[i]->symbol = (unsigned char)i; // Associate the associated index with it's ASCII representation, and add to node pointer attribute
		huffTable[i]->weight = table[i]; // Add the frequency of the correlating ASCII character to the node
	}

	fin.close();

	return 0;
}

void Huffman::createHuffmanTree()
{
// Method used to create a Huffman Tree from an input file
// Create the Huffman Tree by making two passes through the array of nodes, create two nodes that will track the two lowest min in the list, then will combine into a subtree. Iteritavely follow this process until the root is at index 1 of the array.
	for (int i = 0; i < 255; i++)
	{
		int minIndex1 = 0; // The index of the first lowest subtree root node in the array
		int minIndex2 = 0; // The index of the second lowest subtree root node in the array
		HuffmanNode* newRoot = new HuffmanNode(); // Tracks the new root of the subtree to be formed

		minIndex1 = findMin1();     // Get first min
		minIndex2 = findMin2(minIndex1); // Get second min, sending first min so it doesn't get searched for again

		if (minIndex2 < minIndex1) // If there is a case that minIndex1 is pointing at a node containing a smaller value than minIndex2 in a part of the array that is in a greater index
		{
			newRoot->leftChild = huffTable[minIndex2];   // Set it's left child pointer
			newRoot->rightChild = huffTable[minIndex1];  // as well as it's right
			newRoot->weight = huffTable[minIndex1]->weight + huffTable[minIndex2]->weight; // Set the weight to the combined weights of min1 and min2, and its default symbol is ""

			huffTable[minIndex2] = newRoot;  // minIndex2 acts as the new root, since it's pointing at index 0
			huffTable[minIndex1] = NULL;     // minIndex1 goes to NULL

			saveTable[saveTableCount - 2] = minIndex2;
			saveTable[saveTableCount - 1] = minIndex1;

		}
		else
		{
			newRoot->leftChild = huffTable[minIndex1];   // Set it's left child pointer
			newRoot->rightChild = huffTable[minIndex2];  // as well as it's right
			newRoot->weight = huffTable[minIndex1]->weight + huffTable[minIndex2]->weight; // Set the weight to the combined weights of min1 and min2, and its default symbol is ""

			huffTable[minIndex1] = newRoot;  // minIndex1 is the newRoot since it's location is less than that of minIndex2
			huffTable[minIndex2] = NULL;     // set minIndex2 to NULL

		}

	}
}

int Huffman::outputToFile(string inputFile, string outputFile)
{
// Method used to write the tree building data to the output file. Then traverse the inputFile and correlate the ASCII representation from the bitTable 
// to each ASCII character, and write it to the output file. There is a return status where, a 0 return value is a successful run, and anything else is an error. 

	ifstream fin(inputFile, ios::binary);
	ofstream fout(outputFile, ios::binary);
	string bitByte = ""; // The string used to store the bits that are read in, and converted to a byte

	fin.seekg(0, ios::end); // Put the get position at the last char in the file, to get size
	fileReadSize = fin.tellg(); // Read the size of the input file
	fin.clear(); // Restart get position
	fin.seekg(0, ios::beg);

	if (fout.fail()) // Return error status if fileOutput2 can't be opened
		return -1;

	fout.write((char *)saveTable, 510);

	while (!fin.eof()) // While there are still characters left to write
	{
		string getBits = ""; // Used to fetch the bits from the table
		string overflowBitByte = ""; // Used to store any overflow bits when trying to read out a byte
		unsigned char nextChar = (unsigned char)fin.get(); // Get the next char

		while (!fin.eof())
		{				
			bitByte += getBits = bitTable[(int)nextChar]; // Append the bits for this character to bitByte
			
			while (bitByte.length() >= 8)		// As long as bitByte has at LEAST 8
			{									// characters in it, process the
				fout.put(bitsToChar(bitByte));	// FIRST 8 into a byte and output them=,
				bitByte.erase(0, 8);			// And then chop off those first 8.
			}
			nextChar = (unsigned char)fin.get(); // Get the next char
		}
	}

	if (bitByte.length() != 0) fout.put(addPadding(bitByte));

	fileWriteSize = fout.tellp(); // Set the write size

	fin.close();
	fout.close();

	return 0;
}

int Huffman::findMin1()
{
// Used to find the first minimum node in the array, returns the index of the minimum node

	HuffmanNode* startNode = NULL; // Start a searching pointer at NULL, then pick up the first avaible node in the node array 
	int currMinIndex = 0; // Starting pointer at index 0 of the array

	for (int i = 0; i < 256; i++) 
	{
		if (huffTable[i] != NULL && (startNode == NULL || huffTable[i]->weight < startNode->weight)) // Make sure a NULL pointer isn't accessed, then if it isn't, check if the searching node is NULL so it can be defaulted, if not, then look for a node with a lesser than value
		{
			startNode = huffTable[i];      // Set the pointer to access for conditional
			currMinIndex = i;              // Set index of min node
			saveTable[saveTableCount] = i; // Set the node associated with the minimum instance
		}
	}

	saveTableCount++; // Go to the next location in the saveTableCount

	return currMinIndex;
}


int Huffman::findMin2(int min1)
{
// Used to find the second minimum node in the array, making sure to not return min1 

	HuffmanNode* startNode = NULL; // Used to track min node that is not at location min1
	int currMinIndex = 0; // Tracks the index of the current min

	for (int i = 0; i < 256; i++) 
	{
		if (huffTable[i] != NULL && ((startNode == NULL && i != min1) || (i != min1 && huffTable[i]->weight < startNode->weight))) 
		{
			startNode = huffTable[i];
			currMinIndex = i;
			saveTable[saveTableCount] = i;
		}
	}

	saveTableCount++;

	return currMinIndex;
}

void Huffman::traverseTree(char character, HuffmanNode* p)
{
// Method used to traverse the Huffman Tree and record the paths taken in binary in the traversal
// Left = 0; Right = 1

	if (p != NULL)
	{
		if (exitRecursion) bitSequence += "0"; // Tack on zero for going down a left child

		traverseTree(character, p->leftChild); // Traverse the left side of the tree

		if (exitRecursion)	bitSequence = bitSequence.substr(0, bitSequence.length() - 1); // Went back up a level, take off of the bit string

		if (p->symbol == character) exitRecursion = false; // If this current node is the character, then capture the bit sequence string

		if (exitRecursion) bitSequence += "1"; // If about to go down a right node, then tack a one onto the string

		traverseTree(character, p->rightChild); // Traverse the right subtree

		if (exitRecursion)	bitSequence = bitSequence.substr(0, bitSequence.length() - 1); // If going back up from a right node, then take off of the bit string
	}

}

unsigned char Huffman::bitsToChar(string bitByte)
{
// Method that takes a string which resembles 8-bits, and converts to it's char equivalent, and returns the char

	int intEquiv = 0; // Used to add the places in the bit strings which will add up to the ASCII char integer representation
	int binaryMultiplier = 1; // The multiplier at each level of the binary string, (Powers of 2)
	/*
	for (int i = 7; i >= 0; i--)
	{
		intEquiv += (int)(bitByte.at(i) - '0') * binaryMultiplier; // Add the power of 2 at position i in the binary string
		binaryMultiplier <<= 1;
	}
	*/
	if (bitByte.at(7) == '1') intEquiv |= 1;
	if (bitByte.at(6) == '1') intEquiv |= 2;
	if (bitByte.at(5) == '1') intEquiv |= 4;
	if (bitByte.at(4) == '1') intEquiv |= 8;
	if (bitByte.at(3) == '1') intEquiv |= 16;
	if (bitByte.at(2) == '1') intEquiv |= 32;
	if (bitByte.at(1) == '1') intEquiv |= 64;
	if (bitByte.at(0) == '1') intEquiv |= 128;

	return (unsigned char)intEquiv;
}

unsigned char Huffman::addPadding(string bitByte)
{
// Method will take a sequence of bits, and will add bits to the sequence, until they add to a byte, then convert with bitsToChar() method 
// and return the padded char.
	int longString;
	for (longString = 0; longString <= 255; longString++) if (bitTable[longString].length() >= 8) break;

	bitByte += bitTable[longString]; // Since this section of the bitTable is never used, get it's bit string for paddding
	
	return bitsToChar(bitByte);
}

string Huffman::verifyCommandLineInput(string command, string inputFile1, string inputFile2, string inputFile3)
{
// Method used to verify the command line input that comes in from the user. It first checks the encoded command input structure, 
// then the decoded method input structure, then the tree file creation method, and lastly the decoding with tree file method input
// structure.

	if (command == "-e" || command == "-E")
	{
		if (inputFile1 == "")	return "-1";

		if (inputFile2 == "")
		{
			for (int i = 0; i < inputFile1.length(); i++)
			{
				if (inputFile1.at(i) == '.')
				{
					inputFile2 = inputFile1.substr(0, i) + ".huf"; // Replace the extension of inputFile1 with .huff and use as the encoded file
					return inputFile2;
				}
			}

			inputFile2 = inputFile1.substr(0, inputFile1.length()) + ".huf"; // If there is no extension, add it

		}

		if (inputFile1 == inputFile2)	return "-2";

		return inputFile2;

	}
	else if (command == "-d" || command == "-D")
	{
		if (inputFile2 == "")	      return "Error: File 2 not Specified. For usage of -d command, use -h for help.";
		if (inputFile1 == inputFile2) return "Error: File1 and File2 can't be the same files.";
	}
	else if (command == "-t" || command == "-T")
	{

		if (inputFile1 == inputFile2)	return "-1";

		if (inputFile2 == "")
		{
			for (int i = 0; i < inputFile1.length(); i++)
			{
				if (inputFile1.at(i) == '.')
				{
					inputFile2 = inputFile1.substr(0, i) + ".htree";
					return inputFile2;
				}
			}

			inputFile2 = inputFile1.substr(0, inputFile1.length()) + ".htree";

		}

		return inputFile2;
	}
	else if (command == "-et" || command == "-ET")
	{
		if (inputFile2 == "")	return "-1";

		if (inputFile3 == "")
		{
			for (int i = 0; i < inputFile1.length(); i++)
			{
				if (inputFile1.at(i) == '.')
				{
					inputFile3 = inputFile1.substr(0, i) + ".huf";
					return inputFile3;
				}
			}

			inputFile3 = inputFile1.substr(0, inputFile1.length()) + ".huf"; 

		}

		return inputFile3;
	}

	return "";
}


