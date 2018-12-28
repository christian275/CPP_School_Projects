/* Author: Christian Ropchock
Date: 02-07-2018
Class: EECS 2510 -- Non-Linear Data Structures

*/

#pragma once
#include <string>

using namespace std;

class Huffman
{
public:
	Huffman();  // Constructor for the Huffman Tree
	void MakeTreeBuilder(string inputFile, string outputFile);
	void EncodeFile(string inputFile, string outputFile);
	void DecodeFile(string inputFile, string outputFile);
	void EncodeFileWithTree(string inputFile, string treeFile, string outputFile);
	void DisplayHelp();
	
private:
	struct HuffmanNode
	{
	// This struct defines a node to the Huffman Tree. Each leaf will contain a symbol and weight of that symbol
	// which corresponds to the symbols number of occurences in any type of ASCII text file, and the non-leaves contain 
    // only the weights. The left child and right child pointers point to the left and right children of the node that 
	// is designated as the parent node. 

		char symbol;
		int weight;
		HuffmanNode* leftChild;
		HuffmanNode* rightChild;
	};
};
