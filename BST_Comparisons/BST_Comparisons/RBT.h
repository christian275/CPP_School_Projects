/*	RBT.h
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This is a header for RBT.cpp. It creates a struct that represents a node in a RBT Tree.
There is also an extra function defined called RBT-Insert-Fixup that fixes the tree.

*/

#pragma once

using namespace std;

class RBT
{
public: 
	RBT();
	void RBT_Insert(char word[]);
	int printRBTTreeHeight(); // This method will print the height of the tree (is accessible to the user)
	string RBT_Search(); // Method that traverses the tree

private:
	struct RBTNode
	{
		// This struct defines a node for an RBT tree. These nodes model that of normal BST nodes, other than that these nodes don't use a 
		// parent pointer, and utilize a reb black color schema.
		char keyWord[50];
		int count = 1;
		char color; // Either 'B'=Black, or 'R'=Red
		RBTNode* leftNode = NULL;
		RBTNode* rightNode = NULL;
		RBTNode* parentNode = NULL; // Parent node that resides above the node in the tree
	};

	RBTNode* root = NULL; // The root for the RBT Tree
	RBTNode* nil = new RBTNode(); // The leaf of the tree that is used at the bottom of the tree

	void RBT_Insert_Fixup(RBTNode* z); // Method to fix anything broken in the RB Tree
	void Left_Rotate(RBTNode* z); // Left rotation 
	void Right_Rotate(RBTNode* z); // Right rotation

	int treeHeight = 0; // This variable keeps track of the height of the tree
	int totalNumberOfNodes = 0; // This will track the number of nodes in the tree
	int totalNumberOfWords = 0; // This will track the number of words in the tree

	void computeRBTTreeHeight(RBTNode* p); // This method will figure out the height of the tree
	void RBT_Traversal(RBTNode *p); // IN-ORDER traversal of the tree
};