/*	AVL.h
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This is a header for AVL.cpp. It creates a struct that represents a node in a AVL Tree.

*/

#pragma once

using namespace std;

class AVL
{
public:
	AVL();
	void AVL_Insert(char word[]);
	int printAVLTreeHeight(); // This method will print the height of the tree (is accessible to the user)
	string AVL_Search(); // Method that traverses the tree

private:
	struct AVLNode
	{
		// This struct defines a node for an AVL tree. These nodes model that of normal BST nodes, other than that these nodes don't use a 
		// parent pointer, and utilize a Balance Factor
		char keyWord[50];
		int count = 1;
		int BF = 0; // Initialize the Balance Factor to 0
		AVLNode* leftNode = NULL;
		AVLNode* rightNode = NULL;
	};

	AVLNode* root = NULL; // The root for the AVL Tree
	int treeHeight = 0; // This variable keeps track of the height of the tree
	int totalNumberOfNodes = 0; // This will track the number of nodes in the tree
	int totalNumberOfWords = 0; // This will track the number of words in the tree

	void computeAVLTreeHeight(AVLNode* p); // This method will figure out the height of the tree
	void AVL_Traversal(AVLNode *p); // IN-ORDER traversal of the tree

};
