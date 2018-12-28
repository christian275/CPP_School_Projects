/*	SkipList.h
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This is a header for SkipList.cpp. It creates a struct that represents a node in a SkipList.

*/

#pragma once

using namespace std;

class SkipList
{
public: 
	SkipList();
	void Skip_Insert(char word[]);
	int getHeight(); // This method will print the height of the tree (is accessible to the user)
	string searchSkipList(); // This method traverses the skip list

private:
	struct SkipListNode
	{
		// This struct defines a node for a Skip List data structure. This node is unique in which
		// it contains an up and down pointer, as well as an infinity keyword.
		char keyWord[50];
		int count = 1;
		SkipListNode* leftNode = NULL;
		SkipListNode* rightNode = NULL;
		SkipListNode* upNode = NULL;   // Up and down used to navigate "Express Lanes"
		SkipListNode* downNode = NULL;
	};
	const char NEG_INF[7] = "negInf";
	const char POS_INF[7] = "posInf";

	SkipListNode *head, *tail; // The head and tail pointers
	int height; // Height (# lanes) in the skip list
	int numItems; // # items in list (slow lane)
	int random; // Random Number for the skip list, used to build how high lane is for node
	int totalNumberOfNodes; // Tracks number of nodes in skip list
	int totalNumberOfWords; // Tracks total number of words in skip list
};