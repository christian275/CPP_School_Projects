/*	BST.h
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This is a header for BST.cpp. It creates a struct that represents a node in a Binary Search Tree.

*/

#pragma once

using namespace std;

class BST
{
public:		
	BST();
	void BST_Insert(char word[]); // This method will insert a node into the tree, or increment the count
	int printBSTTreeHeight(); // This method will print the height of the tree (is accessible to the user)
	string BST_Search(); // Method that traverses the tree

private:
	struct BSTNode
	{
		// Each node in the Binary Search Tree contains a word that acts as a key for the node, an
		// integer that represents the count which tracks the amount of times the node's key appears, 
		// a left and right node that contain links to the next nodes, also a parent node that is used
		// to traverse to the previous node.
		char keyWord[50];
		int count = 1;
		BSTNode* leftNode = NULL;
		BSTNode* rightNode = NULL;
	};

	BSTNode* root; // This node keeps a reference to the root of the tree
	int treeHeight = 0; // This variable keeps track of the height of the tree
	int totalNumberOfNodes = 0; // This will track the number of nodes in the tree
	int totalNumberOfWords = 0; // This will track the number of words in the tree

	void computeBSTTreeHeight(BSTNode* p); // This method will figure out the height of the tree
	void BST_Traversal(BSTNode *p); // IN-ORDER traversal of the tree
};