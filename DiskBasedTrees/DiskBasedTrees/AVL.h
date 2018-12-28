/*	AVL.h
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 0/15/2018

This is a header for AVL.cpp. It creates a struct that represents a node in a AVL Tree.

*/

#pragma once

using namespace std;

class AVL
{
public:
	AVL();
	~AVL();
	void AVL_Insert(char word[]);
	int printAVLTreeHeight(); // This method will print the height of the tree (is accessible to the user)
	string AVL_Search(); // Method that traverses the tree

private:
	struct AVLNode
	{
		// This struct defines a node for an AVL tree. These nodes model that of normal BST nodes, other than that these nodes don't use a 
		// parent pointer, and utilize a Balance Factor
		char keyWord[32];
		int count = 1;
		int BF = 0; // Initialize the Balance Factor to 0
		int leftNode = -1; // Left node and right node saved as
		int rightNode = -1; // integers used to lookup the tree in a disk based file (-1 = NULL for node)
	};

	int root = -1; // The root node which is going to be position 1 
	               // root = -1 = No Root

	AVLNode Disk_Read(int nodeNumber); // Used to read the node number from disk file
	int Disk_Write(AVLNode wNode, int position); // Used to write node out to file and returns position in file

	int treeHeight = 0; // This variable keeps track of the height of the tree
	int totalNumberOfNodes = 0; // This will track the number of nodes in the tree
	int totalNumberOfWords = 0; // This will track the number of words in the tree

	void computeAVLTreeHeight(int startPos); // This method will figure out the height of the tree
	void AVL_Traversal(int startPos); // IN-ORDER traversal of the tree

};
