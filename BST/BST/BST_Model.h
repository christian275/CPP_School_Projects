/* Author: Christian Ropchock
   Date: 02-07-2018
   Class: EECS 2510 -- Non-Linear Data Structures

   Header file used to create definitions for the attributes of the BST_Model class, as
   well as create method definitions. This header file uses information hiding to keep
   the internal details of the BST_Model hidden from the user while also making it pubically
   available

*/

#pragma once
#include <string>

using namespace std;

class BST_Model
{
public:
	BST_Model();                // Constructor to initialize an empty BST               
	void insert(string word);
	string remove(string word);
	string search(string word);
	string min();
	string max();
	string next(string word);
	string prev(string word);
	void list();
	void help();
	void terminate();

private:
	struct BSTNode
	{
		// Each node in the Binary Search Tree contains a word that acts as a key for the node, an
		// integer that represents the count which tracks the amount of times the node's key appears, 
		// a left and right node that contain links to the next nodes, also a parent node that is used
	    // to traverse to the previous node.
		string keyWord;
		int count = 1;
		BSTNode* leftNode = NULL;
		BSTNode* rightNode = NULL;
		BSTNode* parentNode = NULL;
	};
	BSTNode* root; // This node keeps a reference to the root of the tree

	BSTNode* min(BSTNode* currentPtr); // Private min method for internal access from BST_Model
	BSTNode* max(BSTNode* currentPtr); // Same as min, but with max
	void treeWalk(BSTNode* p); // Private method used for recursion for traversing the BST
	void transplant(BSTNode* subTree1, BSTNode* subTree2); // Helper function for deleteing nodes, it replaces one subtree with another
	BSTNode* locate(string word); // Used to locate a node based on a string input correlating to the string to look up
};


