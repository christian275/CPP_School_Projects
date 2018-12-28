/* Author: Christian Ropchock
   Date: 02-07-2018
   Class: EECS 2510 -- Non-Linear Data Structures

  This CPP file defines all the attributes of the Binary Search Tree ADT. This file will define the methods needed for
  the BST including: Insert, Delete, Search, Min, Max, Next, Prev, List, Help, and Exit. A BST contains nodes that are 
  all linked to each other through pointer references via a right link, left link, and a parent pointer to traverse 
  backwards through the tree.
*/

#include "stdafx.h"
#include <iostream>
#include "BST_Model.h"

// Constructor for the BST_Model class
BST_Model::BST_Model()
{
	root = NULL;
}

// Performs the action of inserting a specified word into the Binary Search Tree. If the word exists in the tree, 
// increment the count of it's corresponding node. If the word doesn't exist in the tree, find the correct location
// where the word is to be inserted, and insert it as a node with a count of 1. If the root is null, then point the root's
// address equal to the node of the word.
void BST_Model::insert(string word)
{
	BSTNode* curr = root; // The node that traverses the tree
	BSTNode* currPrev = NULL; // The node that lags one node behind curr

	while (curr != NULL) // While the the current pointer didn't fall off the tree yet
	{
		currPrev = curr; // Set the previous pointer to the current to hold reference to the pointer before the left or right node traversal
		if (word == curr->keyWord) // If the word to be found is found in the list
		{
			curr->count++; // Increment the count of the word in it's appropriate node
			cout << word << " " << curr->count << endl;
			return;
		}
		else if (word < curr->keyWord)  // If the word to be inserted is less than the current node, search the left node
			curr = curr->leftNode;
		else                            // If greater than, the right node 
			curr = curr->rightNode;
	}

	BSTNode* newNode = new BSTNode(); // Create new node with the new word to insert
	newNode->keyWord = word;
	newNode->parentNode = currPrev;   // Set the parent node to the previous node

	if (currPrev == NULL) // If the previous node is still null, then there isn't a root created yet
		root = newNode;
	else if (newNode->keyWord < currPrev->keyWord) // Insert in the left node of the previous node, if the new node's key is less than the previous node's key
		currPrev->leftNode = newNode;
	else                                           // Else insert in the right node if greater than
		currPrev->rightNode = newNode;

	cout << word << " " << newNode->count << endl;

}

// Deletes a single node from the Binary Search Tree. A string is passed to the method that in turn is used to find the key of the node
// that is to be deleted. There are 3 cases to consider when deleting a node, whether it has zero, one, or two children.
string BST_Model::remove(string word)
{
	
	BSTNode* curr = locate(word); // Locate where the node is to be delted.

	if (curr == NULL)             // If the string isn't in the BST, return -1
		return word + " -1";
	else if (curr->count > 1)     // If the count in the string found is greater than 1, then decrement the count attribute of the node associated with the string
	{
		curr->count--;            // then decrement the count attribute of the node associated with the string
		return curr->keyWord + " " + to_string(curr->count);
	}
	else
	{

		BSTNode* backUp = curr; // Used to keep a reference to curr for printing out results

		// Case 1, No Children
		if (curr->leftNode == NULL && curr->rightNode == NULL)       // Both are NULL
		{
			if (curr == root) // If the special case that it's the root, then delete the root
			{
				root = NULL;                             // Set the pointer to NULL
				delete curr;                             // Free up the memory that the pointer originally referenced
			}
			else if (curr->parentNode->leftNode == curr) // If curr's parent's left node is curr, then set the left node to NULL
			{
				curr->parentNode->leftNode = NULL;
				delete curr;
			}
			else                                         // Else if curr's parent's right node is curr, then set the right node to NULL
			{
				curr->parentNode->rightNode = NULL;
				delete curr;
			}

		}


		// Case 2, 1 Child
		else if (curr->leftNode == NULL) // Left node is NULL, and right node has content
		{
			transplant(curr, curr->rightNode);
		}

		else if (curr->rightNode == NULL) // Right node is NULL, and left node has content
		{
			transplant(curr, curr->leftNode);
		}


		// Case 3, 2 Children
		else                                                            // Then if the top two conditions fail, both children have to be occupied
		{
			BSTNode* successorNode = min(curr->rightNode);              // Get the successor of the current node to be deleted

			if (successorNode->parentNode != curr)                      // Check if the current node's successor is it's immediate right child          
			{
				transplant(successorNode, successorNode->rightNode);    // If it's not the case that the node's direct successor is it's right child, start by switching the current node's successor with its right child
				successorNode->rightNode = curr->rightNode;             // Replace the current node's right child 
				successorNode->rightNode->parentNode = successorNode;   // by the successor node's right child
			}

			transplant(curr, successorNode);                            // Replace the node to be deleted by the successor node to replace it
			successorNode->leftNode = curr->leftNode;                   // Replace the successor node's left child with
			successorNode->leftNode->parentNode = successorNode;        // the left child of the current node
		}
	}

	return word + " 0";
}

// Function used for replacing a subtree with another subtree. Used in the remove method for deleting a node, preserving it's subtree, 
// and adding the subtree back to the node's original location.
void BST_Model::transplant(BSTNode* subTree1, BSTNode* subTree2)
{
	if (subTree1->parentNode == NULL) // If subTree1 is the root
		root = subTree2;              // then subTree2 becomes the new root

	else if (subTree1 == subTree1->parentNode->leftNode) // If subTree1 is the left child of it's parent, then replace that left child with subTree2
		subTree1->parentNode->leftNode = subTree2;
	else                                                 // Else if it's the right child, then replace the right child with subTree2
		subTree1->parentNode->rightNode = subTree2;

	if (subTree2 != NULL)                                // If subtree2 isn't NULL, then the parent of subTree2 becomes the parent of subTree1
		subTree2->parentNode = subTree1->parentNode;

}



// This method searches the Binary Search Tree for the word specified. If the string is found, return a string with the 
// word and it's associated count. If the string isn't found, return the string name searched for followed by a zero.
string BST_Model::search(string word)
{

	BSTNode* curr = locate(word); // This pointer is used to traverse the tree, progressing down the tree while searching for the key
	
	if (curr != NULL) // If the current pointer is returned with an address of the node containing the word
		return word + " " + std::to_string(curr->count);
	else 
		return word + " 0"; // Return the word with a zero count, indicating it isn't in the list

}

// The public min function that is accessible only to main()
string BST_Model::min()
{

	if (root == NULL) // If there is no root in the tree yet
		return "";

	BSTNode* curr = root; // Current pointer used to traverse the tree
	
	while (curr->leftNode != NULL) // Search the left node of every node starting from the root until a null left node is found
		curr = curr->leftNode;

	return curr->keyWord;
}

// The private min function that is accessible only from within the class BST_Model
BST_Model::BSTNode* BST_Model::min(BSTNode* currentPtr)
{
	while (currentPtr->leftNode != NULL) 
		currentPtr = currentPtr->leftNode;

	return currentPtr;
}

// Finds the maximum key in the Binary Search Tree
string BST_Model::max()
{

	if (root == NULL) // If there is no root in the tree yet
		return "";

	BSTNode* curr = root; // Current pointer used to traverse the tree

	while (curr->rightNode != NULL) // Search the right node of every node starting from the root until a null right node is found
		curr = curr->rightNode;

	return curr->keyWord;
}

// The private max function that is accessible only from within the class BST_Model
BST_Model::BSTNode* BST_Model::max(BSTNode* currentPtr)
{
	while (currentPtr->rightNode != NULL)
		currentPtr = currentPtr->rightNode;

	return currentPtr;
}

// Finds the next node in the tree that Succeeds the node that contains the user input word
string BST_Model::next(string word)
{
	if (word != max()) // If it isn't the maximum node in the BST
	{
		BSTNode* curr = locate(word); // The current node to traverse the BST, and the locate method is used to locate the node in the tree

		if (curr != NULL) // If the node was found in the BST via the locate function
		{
			if (curr->rightNode != NULL) // If there is another subtree that contains a node with key greater than that of word to be found
			{
				return min(curr->rightNode)->keyWord; // Return the min of that subtree
			}

			else
			{
				while (curr->parentNode != NULL && curr == curr->parentNode->rightNode) // While there is a parent node to traverse (not at the root), and the link back is that of a right node of a node
				{
					curr = curr->parentNode; // Traverse one node back up the tree
				}

				return curr->parentNode->keyWord;
			}
		}
	}

	return ""; // Return a blank string if either it's the max, or the string isn't found in the BST
}

// Finds the previous node in the tree that Precceeds the node that contains the user input
string BST_Model::prev(string word)
{
	if (word != min()) // If it isn't the minimum node in the BST
	{
		BSTNode* curr = locate(word); // The current node to traverse the BST, and the locate method is used to locate the node in the tree

		if (curr != NULL) // If the node was found in the BST via the locate function
		{
			if (curr->leftNode != NULL) // If there is another subtree that contains a node with key less than that of word to be found
			{
				return max(curr->leftNode)->keyWord; // Return the max of that subtree
			}

			else
			{
				while (curr->parentNode != NULL && curr == curr->parentNode->leftNode) // While there is a parent node to traverse (not at the root), and the link back is that of a left node of a node
				{
					curr = curr->parentNode; // Traverse one node back up the tree
				}

				return curr->parentNode->keyWord;
			}
		}
	}

	return ""; // Return a blank string if either it's the max, or the string isn't found in the BST
}

// This function lists all of the contents of the Binary Search Tree in an ordered, ascending traversal
void BST_Model::list()
{
	if (root != NULL)
		treeWalk(root);
	else
		cout << endl; // Print out newline to resemble an empty list
}

// Method that is recursively called to traverse the tree
void BST_Model::treeWalk(BSTNode* p)
{
	if (p != NULL) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		treeWalk(p->leftNode); // Print out the contents of the left subtree of the root
		cout << p->keyWord << " " << p->count << endl;
		treeWalk(p->rightNode); // Print out the contents of the right subtree of the root
	}
}

// This function finds the location of a node in the tree with the same key as the word that is sent in as the parameter.
BST_Model::BSTNode* BST_Model::locate(string word)
{
	BSTNode* curr = root; // The current node to traverse the BST

	while (curr != NULL) // First locate the node in the BST by traversing 
	{
		if (word == curr->keyWord) // If the word to be found is found in the list, return the pointer to that node
			return curr;
		else if (word < curr->keyWord)  // If the word to be found is less than the current node, search the left node
			curr = curr->leftNode;
		else                            // If greater than, the right node 
			curr = curr->rightNode;
	}

	return NULL; // Return null if the word doesn't exist in the BST

}

// This method displays the different commands avaiable to the user in their correct formatting.
void BST_Model::help()
{
	cout << "\n**Available commands with correct formatting**\n================================================================================" << "insert <string to insert>: Inserts a new word to the Binary Search Tree.\n\n" <<
		"delete <string to delete>: Delete a node from the Binary Search Tree containing the word as it's key.\n\n" <<
		"search <string to search for>: Search the Binary Search Tree for a word, returns the word and it's number of occurences if found, and the word with a zero if not found.\n\n" <<
		"min: Returns a string that represents the MINIMUM value in the Binary Search Tree, otherwise blank line if tree is empty.\n\n" <<
		"max: Returns a string that represents the MAXIMUM value in the Binary Search Tree, otherwise blank line if tree is empty.\n\n" <<
		"next <string to find the Successor for>: Returns the SUCCESSOR of the word in the Binary Search Tree.\n\n" <<
		"prev <string to find the Predecessor for>: Returns the PREDECESSOR of the word in the Binary Search Tree.\n\n" <<
		"list: Outputs the contents of the Binary Search Tree in order, along with each node's corresponding count in a neatly formatted display.\n\n" <<
		"help: Outputs the commands avaiable to the user.\n\n" <<
		"exit: Terminates the program.\n================================================================================" << endl;

}

// This method terminates the program.
void BST_Model::terminate()
{
	exit(0);
}