/*	BTree.h
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 04/15/2018

This is a header for BTree.cpp. It creates a struct that represents a node in a BTree.

*/

#pragma once

using namespace std;

const int t = 3; // Branching factor for the tree

class BTree
{
public:
	BTree();
	~BTree();
	void B_Tree_Insert(char key[]); // Insert new key into B_Tree
	string B_Tree_Search(); // Return string that shows metrics of B_Tree

private:

	struct BNode
	{
		// This struct defines a node for an AVL tree. These nodes model that of normal BST nodes, other than that these nodes don't use a 
		// parent pointer, and utilize a Balance Factor
		char keys[2*t-1][32]; // Keys that store individual strings in 32 byte interval
		int childPointers[2*t]; // n+1 child pointers for node
		int count[2*t-1];
		bool leafFlag = true;
		int n = 0; // Number of keys currently stored in node
	};

	int root = -1; // Set root to -1 null to initialize

	int treeHeight = 0; // This variable keeps track of the height of the tree
	int totalNumberOfDistinctWords = 0; // This will track the number of nodes in the tree
	int totalNumberOfWords = 0; // This will track the number of words in the tree

	int B_Tree_Split_Child(BNode X, int indexOfNode, int xPos); 
	void B_Tree_Insert_NonFull(BNode s, char key[], int sPos);
	void BTree_Traversal(int startPos);

	BNode Disk_Read(int nodeNumber); // Used to read the node number from disk file
	int Disk_Write(BNode wNode, int position); // Used to write node out to file and returns position in file

};