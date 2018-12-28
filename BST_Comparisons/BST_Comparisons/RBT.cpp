/*	RBT.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This CPP file contains the class for the Red-Black Tree. The RB Tree is responsible for performing inserts for performance comparisons 
towards some other Binary Search Trees. This tree contains special data is it's nodes indicating whether the specific node is red or black. 
There are also 5 different rules that the tree is to follow...
1. Every node is either "Red" or "Black"
2. The root is always Black
3. Every leaf is always Black
4. If a nodes is red, then both of its children are black (no two consecutive red nodes on any path to leaf)
5. For EVERY node, the number of black nodes between that node and the leaves is the same

*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include "RBT.h"

#define _CRT_SECURE_NO_WARNINGS

int RBT_tempTreeHeight = 0; // Global vari able used to track the current height of the tree during the traversal
int RBT_comparisons = 0; // Global variable that tracks key comparisons with word inserted
int RBT_pointerChange = 0; // Tracks total number of pointer changes
int RBT_recoloring = 0; // Tracks the number of times a node is recolored

RBT::RBT()
{
	// Constructor for RB Tree that sets the root and nil node to point to itself and be black
	root = nil;
	nil->color = 'B'; // Set nil node to be black
	nil->leftNode = nil;
	nil->rightNode = nil;
	nil->parentNode = nil;
}

void RBT::RBT_Insert(char word[])
{
	// Method responsible for inserting a word into the RBT Tree. There are two different phases to this process, first a "dumb"
	// BST Tree insert is performed, then if needed, a fixup is performed on the tree if it violates any of the 5 rules.
	RBTNode *y = nil; // y lags behind x (parent of x)
	RBTNode *x = root; // x searches for insertion point

	while (x != nil)
	{
		// Find where the new node goes in the tree
		y = x;
		if (strcmp(word, x->keyWord) == 0) {x->count++; RBT_comparisons++; return;} // If the word is found during traversal,... 
		(strcmp(word, x->keyWord) < 0) ? (x = x->leftNode) : (x = x->rightNode);    // ...increment count and return
		RBT_comparisons++;
	}

	RBTNode *z = new RBTNode(); // Instantiate the new node to be added to the tree
	strcpy(z->keyWord, word);   // Add the keyword to the new node
	z->parentNode = y;          // Set the parent of the new word's node to the Y node (The node above it in the tree)
	RBT_pointerChange++;

	if (y == nil) // If tree was empty, new node z is the new root
	{
		RBTNode* nilP = nil; // Create new instance of nil node to keep reference when setting the root
		z->color = 'B'; // Root is black
		RBT_recoloring++;
		root = z;       // Set the root to node z
		root->leftNode = nilP; // Set the left, right, and parent nodes of z to the nil node.
		root->rightNode = nilP;
		root->parentNode = nilP;
		RBT_pointerChange += 3; // For ALL of the new node references for the root
		return;
	}
	else if (strcmp(word, y->keyWord) < 0) {y->leftNode = z; RBT_comparisons++; RBT_pointerChange++;} // Left node if less then it's parent
	else                                   {y->rightNode = z; RBT_comparisons++; RBT_pointerChange++;} // Or right if greater than

	z->leftNode = nil; // Left and right node point to nil node
	z->rightNode = nil;
	RBT_pointerChange+=2;
	z->color = 'R';    // Dumb insert a red node
	RBT_recoloring++;

	RBT_Insert_Fixup(z); // Call method to check for any fix-ups

}

void RBT::RBT_Insert_Fixup(RBTNode* z)
{
	// Method used to fix any potential problems with the tree. The problems that could occur with the tree
	// are explained in the file description above, there are five of them. Node z is the node being inserted
	// into the tree.
	//cout << z->parentNode->color << endl;
	while (z->parentNode->color == 'R') // While there are still consecutive red nodes
	{
		if (z->parentNode == z->parentNode->parentNode->leftNode) // Check if z's parent is a left child of it's grandparent
		{
			RBTNode* y = z->parentNode->parentNode->rightNode; // Y is z's right uncle
			if (y->color == 'R') // Case 1: Recolor Only
			{
				z->parentNode->color = 'B';              // Case 1
				y->color = 'B';                          // Case 1
				z->parentNode->parentNode->color = 'R';  // Case 1
				z = z->parentNode->parentNode;           // Case 1
				RBT_recoloring+=3;
			}
			else
			{
				if (z == z->parentNode->rightNode) // Is z the right child of it's parent?
				{
					z = z->parentNode; // Case 2
					Left_Rotate(z);    // Case 2
				}
				z->parentNode->color = 'B';              // Case 3
				z->parentNode->parentNode->color = 'R';  // Case 3
				Right_Rotate(z->parentNode->parentNode); // Case 3
				RBT_recoloring += 2;
			}
		}
		else
		{
			RBTNode* y = z->parentNode->parentNode->leftNode; // Y is z's left uncle
			if (y->color == 'R') // Case 1: Recolor Only
			{
				z->parentNode->color = 'B';              // Case 1
				y->color = 'B';                          // Case 1
				z->parentNode->parentNode->color = 'R';  // Case 1
				z = z->parentNode->parentNode;           // Case 1
				RBT_recoloring+=3;
			}
			else
			{
				if (z == z->parentNode->leftNode) // Is z the left child of it's parent?
				{
					z = z->parentNode; // Case 2
					Right_Rotate(z);    // Case 2
				}
				z->parentNode->color = 'B';              // Case 3
				z->parentNode->parentNode->color = 'R';  // Case 3
				Left_Rotate(z->parentNode->parentNode);  // Case 3
				RBT_recoloring+=2;
			}
		}
	}
	root->color = 'B'; // Set root to Black
	RBT_recoloring++;
	
}

void RBT::Left_Rotate(RBTNode* z)
{
	// Method used to perform a left rotation on a node z, that is being inserted into a RB Tree. Node z is the node being
	// inserted into the tree.
	RBTNode* y = z->rightNode; // y is z's right (non-NIL) child
	z->rightNode = y->leftNode; // move y's left subtree into z's right subtree
	if (y->leftNode != nil)
		{y->leftNode->parentNode = z; RBT_pointerChange++;}
	y->parentNode = z->parentNode; // Link z's parent to y

	if (z->parentNode == nil)      // If z has no parent, z was the root, so 
		root = y;                  // set y to the new root
	else if (z == z->parentNode->leftNode) // Otherwise, z has a parent, the 
		{z->parentNode->leftNode = y; RBT_pointerChange++;} // spot z used to occupy now gets taken by y
	else
		{z->parentNode->rightNode = y; RBT_pointerChange++;}

	y->leftNode = z;               // Put z on y's left, which
	z->parentNode = y;             // Makes z's parent be y
	RBT_pointerChange+=4;
}

void RBT::Right_Rotate(RBTNode* z)
{
	// Symmetrically identical to Left_Rotate
	RBTNode* y = z->leftNode; // y is z's left (non-NIL) child
	z->leftNode = y->rightNode; // move y's right subtree into z's left subtree
	if (y->rightNode != nil)
		{y->rightNode->parentNode = z; RBT_pointerChange++;}
	y->parentNode = z->parentNode; // Link z's parent to y
	if (z->parentNode == nil)      // If z has no parent, z was the root, so 
		root = y;                  // set y to the new root
	else if (z == z->parentNode->leftNode) // Otherwise, z has a parent, the 
		{z->parentNode->leftNode = y; RBT_pointerChange++;} // spot z used to occupy now gets taken by y
	else
		{z->parentNode->rightNode = y; RBT_pointerChange++;}
	y->rightNode = z;               // Put z on y's right, which
	z->parentNode = y;             // Makes z's parent be y
	RBT_pointerChange+=4;
}


int RBT::printRBTTreeHeight()
{
	// This method is responsible for printing out the height of the tree to the user. Returns the height of the tree.
	treeHeight = 0; // Reset the tree height

	computeRBTTreeHeight(root);

	return treeHeight;
}

void RBT::computeRBTTreeHeight(RBTNode *p)
{
	// This method is private and unaccessible to the user. It's used to traverse the tree and find the longest path to a 
	// null node, which will be the height of the tree. The input is a node to start the traversal from. This method returns 
	// the height of the tree. Pointer p is the root of the tree.
	if (p != nil) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		RBT_tempTreeHeight++; // Increment the temperary tree height
		if (RBT_tempTreeHeight > treeHeight) treeHeight = RBT_tempTreeHeight; // When going down a left node, if the temp gets larger then 
		                                                                      // the current height, then the new largest height is the temp

		computeRBTTreeHeight(p->leftNode);

		computeRBTTreeHeight(p->rightNode);

		RBT_tempTreeHeight--; // Decrement the temporary tree height if going back from a right node
	}
}

string RBT::RBT_Search()
{
	// Used to return a string that contains the total number of nodes, as well as the 
	// total number of words
	totalNumberOfNodes = 0; // Reset
	totalNumberOfWords = 0;

	RBT_Traversal(root);

	// Add one to total number of nodes to include the nil node
	return "	Total number of Distinct Words: " + to_string(totalNumberOfNodes) + "\n	Total number of Words: " 
		+ to_string(totalNumberOfWords) + "\n" + "	Total number of Color Changes: " + to_string(RBT_recoloring) +
		"\n	Total number of Key Comparisons: " + to_string(RBT_comparisons) + "\n	Total number of Pointer Changes: " + 
		to_string(RBT_pointerChange) + "\n	Total Amount of Work Done: " + to_string(RBT_recoloring + RBT_comparisons + RBT_pointerChange)
		+ "\n";
}

void RBT::RBT_Traversal(RBTNode *p)
{
	// This method traverses the tree to get the total number of nodes, as well as the total number of words in the 
	// tree through the node's count variables

	if (p != nil) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		RBT_Traversal(p->leftNode);
		// In the middle node, capture it's count occurence
		totalNumberOfNodes++;           // Increase node encounter by 1
		totalNumberOfWords += p->count; // Increase number of words encountered by 1

		RBT_Traversal(p->rightNode);
	}
}