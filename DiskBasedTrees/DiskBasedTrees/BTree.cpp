/*	BTree.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 04/15/2018

This is the cpp file that defines the class for a BTree. A Btree is different from other search trees
in which it focuses more on disk based operations as opposed to memory based operations. This tree will
read nodes to and from a file, and can maintain more than one key in each individual node. 

*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

#include "BTree.h"

#define _CRT_SECURE_NO_WARNINGS

fstream BFile;
int BTree_diskWriteCount = 0; // Tracks # of disk writes
int BTree_diskReadCount = 0; // Tracks # of disk reads
int BTree_totalNodeCount = 0; // Tracks # of nodes
int BTree_totalAvailableKeys = 0; // Tracks # of available keys

int BTree_tempTreeHeight = 0; // Global variable used to track the current height of the tree during the traversal

BTree::BTree()
{
	// Constructor for BTree
	BFile.open("C:\\BTreeInfo.txt", ios::in | ios::out | ios_base::trunc | ios::binary);

	if (BFile.fail())
	{
		cout << "Unable to open tree info file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";
		exit(1);
	}

	BNode nullNode;

	Disk_Write(nullNode, -1); // Set the position of the root in the file after writing to the disk file
	BTree_diskWriteCount++;

}

BTree::~BTree()
{
	// Destructor for BTree

	BFile.close();

}

void BTree::B_Tree_Insert(char key[])
{
	// Method that inserts a key into a node, or updates a counter if a node with the keyword already exists. 
	// This method will first attempt to locate the key amongst the array of keys in the node, then if the node 
	// is full, will pop a key out of the node, then will split it into 2 nodes.
	// Value 2 * T - 1 = Max number of Keys allowed in Node

	BNode r = Disk_Read(root); // Read in the root into a node that will traverse the tree for insertion
	BTree_diskReadCount++;

	if (root == -1)
	{
		// If the root is NULL
		BNode newRoot;
		newRoot.leafFlag = true;
		newRoot.n = 1;
		strcpy(newRoot.keys[0], key);
		newRoot.count[0] = 1;

		for (int i = 0; i < 2 * t; i++) // Set all pointers to NULL state of -1
			newRoot.childPointers[i] = -1;

		root = Disk_Write(newRoot, -1); // Set the position of the root in the file after writing to the disk file
		return;
	}

	if (r.n == 2 * t - 1)
	{
		// If the root node is full, then split it
		BNode s; // Create a new node that will be the new root
		r = s; // Set root to blank, and update s (New Root) with new root attributes
		s.leafFlag = false;
		s.n = 0;
		s.childPointers[0] = root; // The root will then be the child of s, after popping it up as a new root
		for (int i = 1; i < 2 * t; i++) // Set all pointers to NULL state of -1
			s.childPointers[i] = -1;
		int sPos = Disk_Write(s, -1); // Write s out to the file, then update after splitting
		BTree_diskWriteCount++;
		sPos = B_Tree_Split_Child(s, 0, sPos);
		s = Disk_Read(sPos);
		BTree_diskReadCount++;
		root = sPos; // Set the new root
		B_Tree_Insert_NonFull(s, key, sPos);
	}
	else
		// Otherwise there is room to insert, so insert the node
		B_Tree_Insert_NonFull(r, key, root);

}

int BTree::B_Tree_Split_Child(BNode X, int i, int xPos)
{
	// This method splits X's child in half, and X is the node that takes the key that is being pushed out of the old node. Y is X's
	// i'th child as designated by method paramter i. So, when this method is done, y has the lower half of it's children, node Z will
	// contain the upper half, and the middle key is pushed up until it can find an open node up in the tree.

	// Returns pointer to new root of SubTree

	BNode Z; // Allocate a new node Z
	BNode Y = Disk_Read(X.childPointers[i]); // Read in node Y, which is located at the i'th child of X
	BTree_diskReadCount++;
	Z.leafFlag = Y.leafFlag; // Set the leafflag of Z to that of Y, becaues they are going to reside at the same depth in the tree
	Z.n = t - 1; 
	for (int j = 0; j < t - 1; j++) // Populate Z's keys with the upper half of Y's keys
	{
		strcpy(Z.keys[j], Y.keys[j + t]);
		Z.count[j] = Y.count[j + t];
	}
	if (!Y.leafFlag) // If Y was not a leaf
	{
		for (int j = 0; j < t; j++) // Then, set all of the child pointers of Z to the same as the upper half of Y's child pointers
			Z.childPointers[j] = Y.childPointers[j + t];
	}
	Y.n = t - 1; // Update number of keys in Y

	for (int j = X.n; j > i; j--) // Shift all existing childpointers in X to the right one spot
		X.childPointers[j + 1] = X.childPointers[j];
	for (int j = 2 * t - 1; j >= t; j--)
		Z.childPointers[j] = -1;
	X.childPointers[i + 1] = Disk_Write(Z, -1); // Write new node z and capture it's position, then save it as new childpointer in X at position i+1 
	BTree_diskWriteCount++;
	for (int j = X.n - 1; j >= i; j--) // Update all the keys in X to reflect the shift in the child pointers
	{
		strcpy(X.keys[j + 1], X.keys[j]);
		X.count[j + 1] = X.count[j];
	}
	strcpy(X.keys[i], Y.keys[t - 1]); // Move the middle key from Y up to the opened position in X
	X.count[i] = Y.count[t - 1];	
	X.n = X.n + 1; // Increase number of keys in X by one

	for (int j = 2 * t - 1; j > Y.n; j--) // Eliminate all upper half keys in Y, including middle key since it moved up
	{
		strcpy(Y.keys[j - 1], "");
		Y.childPointers[j] = -1;
		Y.count[j - 1] = 0;
	}

	// Write new X, Y, and Z pointers to update in disk file
	Disk_Write(Y, X.childPointers[i]); 
	Disk_Write(Z, X.childPointers[i + 1]);
	Disk_Write(X, xPos);

	BTree_diskWriteCount += 3;

	return xPos;
}

void BTree::B_Tree_Insert_NonFull(BNode s, char key[], int sPos)
{
	// This method is used to insert a new node into an existing node. The parameter s is is node to be inserted, as well as key which is 
	// it's corresponding key. This method first attempts to find a node with a vacancy to place s, and if the node is full that is epected to
	// take s as a new child, then the node is split, and the next node up is checked.

	int i = s.n - 1; // Set i to be the number of keys in the node 
	for (int j = 0; j <= i; j++) // Check for duplicate
	{
		if (strcmp(key, s.keys[j]) == 0)
		{
			s.count[j]++;
			Disk_Write(s, sPos);
			BTree_diskWriteCount++;
			return;
		}
	}
	if (s.leafFlag) // Code to handle if s is a leaf node, and inserting key into s
	{
		while (i >= 0 && strcmp(key, s.keys[i]) < 0) // Keep shifting all the keys right until a spot opens up to insert key
		{ 
			strcpy(s.keys[i + 1], s.keys[i]);
			s.count[i + 1] = s.count[i];
			i = i - 1;
		}
		strcpy(s.keys[i + 1], key); // Insert the key into node s
		s.count[i + 1] = 1;
		s.n = s.n + 1; // Increase number of keys in s
		Disk_Write(s, sPos); // Update s in disk file
		BTree_diskWriteCount++;
	}
	else
	{
		while (i >= 0 && strcmp(key, s.keys[i]) < 0) // Determine the child of s that the recursion descends into
			i = i - 1;
		i = i + 1;
		BNode sChild = Disk_Read(s.childPointers[i]); // Read in the child of s that we are descended into
		BTree_diskReadCount++;
		if (sChild.n == 2 * t - 1) // If this child is full, takes care of splitting nodes while working down the tree
		{
			sPos = B_Tree_Split_Child(s, i, sPos); // Split the child
			s = Disk_Read(sPos);
			for (int j = 0; j < s.n; j++) // Check for duplicate
			{
				if (strcmp(key, s.keys[j]) == 0)
				{
					s.count[j]++;
					Disk_Write(s, sPos);
					BTree_diskWriteCount++;
					return;
				}
			}
			if (strcmp(key, s.keys[i]) > 0) // Figure out which child to descend into for inserting key
				i = i + 1;
			sChild = Disk_Read(s.childPointers[i]);
			BTree_diskReadCount += 2;
		}
		B_Tree_Insert_NonFull(sChild, key, s.childPointers[i]); // Recurse to insert key into appropriate subtree
	}

}

string BTree::B_Tree_Search()
{
	// Used to traverse the tree in order to capture the number of nodes in the tree, as well as the number of words in the file being read, 
	// and the height of the tree. This method will also add all the n values in each node it visits to get the total number of keys that 
	// occupy a key slot in the tree. 
	// Loading Factor = Total Number of Free Key Slots in Tree / Total Number of Key Slots in Tree
	totalNumberOfDistinctWords = 0; // Reset
	totalNumberOfWords = 0;

	BTree_Traversal(root);

	BFile.clear();
	BFile.seekg(0, ios::end);

	return "	Total number of Distinct Words: " + to_string(totalNumberOfDistinctWords) + "\n	Total number of Words: " +
		to_string(totalNumberOfWords) + "\n	Total Number of Nodes: " + to_string(BTree_totalNodeCount) + "\n	Total number of Disk Writes: "
		+ to_string(BTree_diskWriteCount) + "\n	Total number of Disk Reads: " + to_string(BTree_diskReadCount) + 
		"\n	Total Number of Disk Accesses: " + to_string(BTree_diskWriteCount + BTree_diskReadCount) + "\n	Total Tree File Size: " 
		+ to_string(BFile.tellg()) + " B" + "\n	Total Height of Tree: " + to_string(treeHeight) + "\n	Loading Factor: " + 
		to_string((double)(BTree_totalAvailableKeys) / (double)((2 * t - 1) * BTree_totalNodeCount) * 100).substr(0, 5) + "%\n";

}

void BTree::BTree_Traversal(int startPos)
{
	// This method traverses the tree to get the total number of nodes, as well as the total number of words in the 
	// tree through the node's count variables.

	if (startPos != -1)
	{
		BNode startNode = Disk_Read(startPos);
		int i = 0;

		BTree_tempTreeHeight++; // Increment the temperary tree height
		if (BTree_tempTreeHeight > treeHeight) treeHeight = BTree_tempTreeHeight; // When going down a child pointer, if the
		                                                                          // temp is larger then the current height, then 
		                                                                          // replace current height with temp
		if (!startNode.leafFlag)
		{
			while (i <= startNode.n) // To account for all child pointers of non-leaves
			{
				BTree_Traversal(startNode.childPointers[i]);
				if (i != startNode.n)
				{
					totalNumberOfDistinctWords++;             // Increase node encounter by 1
					totalNumberOfWords += startNode.count[i]; // Increase number of words encountered by 1
				}
				i++;
			}
			BTree_tempTreeHeight--; // Decrease tree height when coming out of a level
			BTree_totalNodeCount++;
			BTree_totalAvailableKeys += ((2 * t - 1) - startNode.n); // Get keys that are still available in node
		}
		else
		{
			while (i < startNode.n) // To get all children of leaves
			{
				BTree_Traversal(startNode.childPointers[i]);
				totalNumberOfDistinctWords++;           // Increase node encounter by 1
				totalNumberOfWords += startNode.count[i]; // Increase number of words encountered by 1
				i++;
			}
			BTree_tempTreeHeight--; // Decrease tree height when coming out of a level
			BTree_totalNodeCount++;
			BTree_totalAvailableKeys += ((2 * t - 1) - startNode.n); // Get keys that are still available in node
		}
	}

}

BTree::BNode BTree::Disk_Read(int nodeNumber)
{
	// Used to read a node from a disk file that resides in a binary file on the disk. Have to buffer to
	// the correct node in the file using nodeNumber as a multiplier for the location. IE, 
	// NodeNumber = 5 = 5th node, and if NodeSize = 1000B, then start at loc 5kB.

	BNode readNode; // Node to read in from file

	// Locate position in file where node to be read resides
	BFile.clear();
	BFile.seekg((sizeof(readNode.keys) + sizeof(readNode.childPointers) + sizeof(readNode.count) + sizeof(int) + sizeof(bool)) * nodeNumber);

	for (int i = 0; i < 2 * t - 1; i++)
		BFile.read((char *)&readNode.keys[i], sizeof(readNode.keys[i]));

	for (int i = 0; i < 2 * t; i++)
		BFile.read((char *)(&readNode.childPointers[i]), sizeof(readNode.childPointers[i]));

	for (int i = 0; i < 2 * t - 1; i++)
		BFile.read((char *)(&readNode.count[i]), sizeof(int));

	BFile.read((char *)(&readNode.leafFlag), sizeof(bool));
	BFile.read((char *)(&readNode.n), sizeof(int));

	return readNode; // Return the node at that location in the file

}

int BTree::Disk_Write(BNode wNode, int position)
{
	// Used to write a node to the disk file that resides on the disk or update node. It will start 
	// the write at the indicated position, and if position is -1, then will append to EOF. Update any time 
	// a node is changed in the code.

	// Returns position of new written node in disk file
	if (position == -1)
	{
		// If position is indicated at end of file, append new node to end of file
		BFile.clear();
		BFile.seekp(0, ios_base::end); // Write at end of file

		for (int i = 0; i < 2*t - 1; i++)
			BFile.write((char *)&wNode.keys[i], sizeof(wNode.keys[i]));

		for (int i = 0; i < 2*t; i++)
			BFile.write((char *)(&wNode.childPointers[i]), sizeof(wNode.childPointers[i]));

		for (int i = 0; i < 2 * t - 1; i++)
			BFile.write((char *)(&wNode.count[i]), sizeof(int));

		BFile.write((char *)(&wNode.leafFlag), sizeof(bool));
		BFile.write((char *)(&wNode.n), sizeof(int));
		
		// Divide the total file size by size of each file to get location 										   
		// of newly added node - 1 because of 0-index based
		return (BFile.tellp() / (sizeof(wNode.keys) + sizeof(wNode.childPointers) + sizeof(wNode.count) + sizeof(int) + sizeof(bool))) - 1;
	}
	else
	{
		// Else update the node already residing in a position
		BFile.clear();
		BFile.seekp((sizeof(wNode.keys) + sizeof(wNode.childPointers) + sizeof(wNode.count) + sizeof(int) + sizeof(bool)) * position);

		for (int i = 0; i < 2 * t - 1; i++)
			BFile.write((char *)&wNode.keys[i], sizeof(wNode.keys[i]));

		for (int i = 0; i < 2 * t; i++)
			BFile.write((char *)(&wNode.childPointers[i]), sizeof(wNode.childPointers[i]));

		for (int i = 0; i < 2 * t - 1; i++)
			BFile.write((char *)(&wNode.count[i]), sizeof(int));

		BFile.write((char *)(&wNode.leafFlag), sizeof(bool));
		BFile.write((char *)(&wNode.n), sizeof(int));

		return position; // Divide the total file size by size of each file to get location 										   
						 // of newly added node - 1 because of 0-index based
	}
}