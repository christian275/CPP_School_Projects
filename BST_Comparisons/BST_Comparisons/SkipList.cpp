/*	SkipList.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This CPP file contains the code that defines the class for the Skip List data structure. 

*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include "SkipList.h"

#define _CRT_SECURE_NO_WARNINGS

int SKP_comparisons = 0; // Tracks total number of node key comparisons
int SKP_pointerChange = 0; // Tracks total number of pointer changes
int numberOfHeads = 0; // Tracks the number of times heads was flipped

SkipList::SkipList()
{
	// Constructor for Skip List
	head = new SkipListNode();
	strcpy(head->keyWord, NEG_INF); // Set head pointer to negative infinity
	tail = new SkipListNode();
	strcpy(tail->keyWord, POS_INF); // Set head pointer to negative infinity
	head->rightNode = tail; // Link the head and tail nodes together
	tail->leftNode = head;

	srand(clock());  // Seed the random number generator
	random = rand(); // Generate a new random number with seed
	numItems = 0; // Original number of entries is zero
	height = 1; // Height starts at one
}

void SkipList::Skip_Insert(char word[])
{
	// First search the Skip List and find where the node containing the word would go, or update 
	// the count at that node if it already exists. Afterwards if an insert is needed, keep using the 
	// random number generator to produce a random number, and mod it with 2 to get a 50/50 probability
	// which is used to determine if the node needs to be added to the next express lane up.

	SkipListNode *p = head; // Start traversal at the head of the skip list
	
	while (true)
	{ 
		// While the sentinel node isn't hit, or the word to insert is still less than the next word, keep 
		// searching right in that lane
		while (strcmp(p->rightNode->keyWord, POS_INF) != 0 && strcmp(p->rightNode->keyWord, word) <= 0)
		{
			// If in the slow lane, and it's a repeated word, then increase it's count and return
			SKP_comparisons++;
			if (p->downNode == NULL && strcmp(p->keyWord, word) == 0) { p->count++;  SKP_comparisons++; return; }
			SKP_comparisons++;
			p = p->rightNode;
		}

		if (p->downNode == NULL)
		{
			if (strcmp(p->keyWord, word) == 0) {SKP_comparisons++; return;}
			break; // Break and capture the pointer if the slow lane is finally reached
		}
		p = p->downNode;                // Go down a lane
	}

	// Insert the node between p and p->rightNode. Flip the coin, and for tails, stop inserting new node above. 
	// If heads, go all the way to the left until a non-null up node is reached, then insert in the right node of that node. 
	// If the left search goes all the way to NEG_INF, then create a new express lane, and insert to the new express lane, making 
	// sure that the up node of the node to insert points to the new node, and the new node's down pointer points to the node 
	// searched from. Keep repeating this process until a tails is flipped, making sure to create a new random number each time.

	SkipListNode *pR = p->rightNode; // Keep reference to the right node of p
	SkipListNode *newNode = new SkipListNode(); // Create node for new word to insert
	strcpy(newNode->keyWord, word);

	p->rightNode = newNode;  // Insert the new node and...
	newNode->leftNode = p;   // ...set it's left node to the orignal node of p
	newNode->rightNode = pR; // and it's right node to the orignal right node of p
	pR->leftNode = newNode; // Set the left node of p's original right to point to the new node
	p = p->rightNode; // Set p to now point at it's right node
	numItems++; // Increase number of items in Skip List
	SKP_pointerChange+=4;

	srand(clock()); // Reset the random number

	while (random % 2 == 1) // 1 = heads, 0 = tails
	{
		numberOfHeads++; // Increase the number of heads encountered

		random = rand();

		SkipListNode *q = p; // Q is used to traverse through the skip list

		while (strcmp(q->keyWord, NEG_INF) != 0 && q->upNode == NULL)	q = q->leftNode;

		if (strcmp(q->keyWord, NEG_INF) == 0 && q->upNode == NULL)
		{
			// Perform new express lane operation here
			SkipListNode *leftEnd = new SkipListNode();
			SkipListNode *rightEnd = new SkipListNode();

			strcpy(leftEnd->keyWord, NEG_INF);
			strcpy(rightEnd->keyWord, POS_INF);

			// Create new express lane with new setinel nodes that point to eachother
			leftEnd->downNode = head;
			head->upNode = leftEnd;
			rightEnd->downNode = tail;
			tail->upNode = rightEnd;
			leftEnd->rightNode = rightEnd;
			rightEnd->leftNode = leftEnd;
			head = leftEnd;
			tail = rightEnd;
			height++; // Increase overall height of tree
			SKP_pointerChange+=6;
		}

		// Insert new node in the next express lane up 
		q = q->upNode; // Shift attention to the top node of the node we're looking at
		SkipListNode *qR = q->rightNode; // Save reference to right node of this top node
		SkipListNode *newP = new SkipListNode(); // New node to insert
		strcpy(newP->keyWord, word); 

		q->rightNode = newP; // The right node of q is this new node
		newP->leftNode = q; // The left node of the new node is q
		newP->rightNode = qR; // The right node of this new node is the original right node of q
		qR->leftNode = newP; // Set left node of old q to the new p
		newP->downNode = p; // Set p to be the bottom node reference of this new node
		p->upNode = newP;   // And top of p to point to new p node
		SKP_pointerChange+=6;

		p = p->upNode; // Move p to point to this new node
	}

	random = rand();

}

int SkipList::getHeight()
{
	// Getter that sends the height of the Skip List
	return height;
}

string SkipList::searchSkipList()
{
	// Traverses the skip list starting at the head. There are two pointers, one that works down the lanes, and another that works 
	// going right in the lanes. Keep going down and counting nodes until the downnode is NULL, then count the number of words and 
	// number of nodes going across.
	totalNumberOfNodes = 0; // Reset
	totalNumberOfWords = 0;

	SkipListNode *downP = head;  // This pointer goes down a lane
	SkipListNode *rightP = head; // This pointer goes right across a lane

	while (downP->downNode != NULL) // Keep going down until there is no more down node
	{
		while (rightP != NULL) // Keep going right until rightP is NULL
		{
			totalNumberOfNodes++; // Increase for every node hit
			rightP = rightP->rightNode;
		}
		downP = downP->downNode; // Go down a node
		rightP = downP; // Reset rightP to point at beginning of new row
	}

	rightP = rightP->rightNode; // Don't include the negative inf node
	totalNumberOfNodes++;

	while (strcmp(rightP->keyWord, POS_INF) != 0)
	{
		// Count every node on bottom lane's count plus the node occurence
		totalNumberOfNodes++;
		totalNumberOfWords += rightP->count;
		rightP = rightP->rightNode;
	}

	// Add one to total number of nodes to account for the last negative infinity not accounted for in bottom 
	// of skip list loop
	return "	Total number of Nodes: " + to_string(totalNumberOfNodes + 1) + "\n	Total number of Nodes in Slow Lane: "
		+ to_string(totalNumberOfWords) + "\n" + "	Total number of Heads Flipped: " + to_string(numberOfHeads) +
		"\n	Total number of Key Comparisons: " + to_string(SKP_comparisons) + "\n	Total number of Pointer Changes: " +
		to_string(SKP_pointerChange) + "\n	Total Amount of Work Done: " + to_string(totalNumberOfNodes + 1 + SKP_comparisons + SKP_pointerChange)
		+ "\n";
}
