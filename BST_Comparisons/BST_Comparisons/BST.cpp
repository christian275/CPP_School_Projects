/*	BST.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This program is where the Binary Search Tree class resides. This class should be able to do Insert
as well as an In-Order traversal to be able to count the number of nodes and occurences of each letter inside each node. 

*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include "BST.h"

int BST_tempTreeHeight = 0; // Global used to track the temp tree height when trying to find the longest path
int BST_comparisons = 0; // Tracks total number of node key comparisons
int BST_pointerChange = 0; // Tracks total number of pointer changes

BST::BST()
{
	// Constructor for the BST_Model class
	root = NULL;
}

void BST::BST_Insert(char word[])
{
	// Performs the action of inserting a specified word into the Binary Search Tree. If the word exists in the tree, 
	// increment the count of it's corresponding node. If the word doesn't exist in the tree, find the correct location
	// where the word is to be inserted, and insert it as a node with a count of 1. If the root is null, then point the root's
	// address equal to the node of the word.

	BSTNode* curr = root; // The node that traverses the tree
	BSTNode* currPrev = NULL; // The node that lags one node behind curr

	while (curr != NULL) // While the the current pointer didn't fall off the tree yet
	{
		currPrev = curr; // Set the previous pointer to the current to hold reference to the pointer before the left or right node traversal

		if (strcmp(word, curr->keyWord) == 0) // If the word to be found is found in the list
		{
			BST_comparisons++;
			curr->count++; // Increment the count of the word in it's appropriate node
			return;
		}
		else if (strcmp(word, curr->keyWord) < 0)  // If the word to be inserted is less than the current node, search the left node
		{
			BST_comparisons += 2; // Add 2 for the if comparison, as well as the else/if
			curr = curr->leftNode;
		}
		else   // If greater than, the right node 
		{
			BST_comparisons += 2;
			curr = curr->rightNode;
		}
	}

	BSTNode* newNode = new BSTNode(); // Create new node with the new word to insert
	strcpy(newNode->keyWord, word);

	if (currPrev == NULL) {root = newNode;} // If the previous node is still null, then there isn't a root created yet
	else if (strcmp(newNode->keyWord, currPrev->keyWord) < 0) // Insert in the left node of the previous node, if the new 
	{														  // node's key is less than the previous node's key
		currPrev->leftNode = newNode;
		BST_pointerChange++;
	}
	else    // Else insert in the right node if greater than
	{
		currPrev->rightNode = newNode;
		BST_pointerChange++;
	}
}

int BST::printBSTTreeHeight()
{
	// This method is responsible for printing out the height of the tree to the user. Returns the height of the tree.

	treeHeight = 0; // Reset the tree height

	computeBSTTreeHeight(root);

	return treeHeight;
}

void BST::computeBSTTreeHeight(BSTNode* p)
{
	// This method is private and unaccessible to the user. It's used to traverse the tree and find the longest path to a 
	// null node, which will be the height of the tree. The input is a node to start the traversal from. This method returns 
	// the height of the tree.
	if (p != NULL) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		BST_tempTreeHeight++; // Increment the temperary tree height
		if (BST_tempTreeHeight > treeHeight) treeHeight = BST_tempTreeHeight; // When going down a left node, if the temp gets larger then 
																			  // the current height, then the new largest height is the temp
		computeBSTTreeHeight(p->leftNode);

		computeBSTTreeHeight(p->rightNode);

		BST_tempTreeHeight--; // Decrement the temporary tree height if going back from a right node
	}
}

string BST::BST_Search()
{
	// Used to return a string that contains the total number of nodes, as well as the 
	// total number of words
	totalNumberOfNodes = 0; // Reset
	totalNumberOfWords = 0;

	BST_Traversal(root);

	return "	Total number of Distinct Words: " + to_string(totalNumberOfNodes) + "\n	Total number of Words: " + 
		to_string(totalNumberOfWords) + "\n	Total number of Key Comparisons: " + to_string(BST_comparisons) + 
		"\n	Total number of Pointer Changes: " + to_string(BST_pointerChange) + "\n	Total Amount of Work Done: "
		+ to_string(BST_comparisons + BST_pointerChange) + "\n";
}

void BST::BST_Traversal(BSTNode *p)
{
	// This method traverses the tree to get the total number of nodes, as well as the total number of words in the 
	// tree through the node's count variables

	if (p != NULL) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		BST_Traversal(p->leftNode);
		// In the middle node, capture it's count occurence
		totalNumberOfNodes++;           // Increase node encounter by 1
		totalNumberOfWords += p->count; // Increase number of words encountered by 1

		BST_Traversal(p->rightNode);
	}
}