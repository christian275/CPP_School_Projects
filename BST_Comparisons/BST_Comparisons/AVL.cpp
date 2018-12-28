/*	AVL.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 03/14/2018

This is the cpp file that defines the class for an AVL Tree. The AVL Tree is responsible for inserting words into the tree, as well as returning
the number of comparisons made, reference swaps, and the height of the tree. This tree implements Balance Factors that determine whether the tree
is still in balance or not, and implements rotations to put the tree back in balance. 

*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include "AVL.h"

#define _CRT_SECURE_NO_WARNINGS

int AVL_tempTreeHeight = 0; // Global variable used to track the current height of the tree during the traversal
int AVL_comparisons = 0; // Tracks total number of node key comparisons
int AVL_pointerChange = 0; // Tracks total number of pointer changes
int BF_Changes = 0; // Tracks the number of times a Balance Factor was changed in a node

AVL::AVL()
{
	// Constructor for AVL Tree
	root = NULL;
}

void AVL::AVL_Insert(char word[])
{
	// Method used to insert a word into an AVL Tree. The process involves doing a "dumb" BST insert into the tree. Then after performing the insert, the Balance Factors in the 
	// Tree are updated all the way up towards the root. If one of the Balance factors were to go to +/-2, then a rotation is performed on the grandparent of the node that was 
	// inserted. There are 4 different types of rotations, RR, LL, RL, and LR. LR and RL both also contain 3 different subcases.

	AVLNode *Y; // The new node that is inserted into the AVL Tree
	AVLNode *A, *B, *F; // A is the last parent above Y with BF=+/-1, B is the child of A used to determine type of rotation, F is A's parent
	AVLNode *P, *Q; // P is used to scan the tree until a NULL node is found for doing a "dumb" insert, Q is P's parent
	AVLNode *C, *CL, *CR; // Node C is either B's left or right child, depending on the location of the imbalance
	int d; // Displacement used to adjust BFs

	if (root == NULL) // If the root is NULL, then insert and exit the method
	{
		Y = new AVLNode();
		strcpy(Y->keyWord, word); // Copy the word into the new node
		root = Y; // Set the new root
		return;
	}

	F = Q = NULL; // F and Q lag A and P respectively, so they start at NULL
	A = P = root; // A and P start at the root of the tree

	while (P != NULL) // Search the tree until P falls off
	{
		if (strcmp(word, P->keyWord) == 0) {P->count++; AVL_comparisons++; return;} // The word is already in the tree
		if (P->BF != 0)     // Track the most recent place where a non-zero BF was found (A)
			{A = P; F = Q;} // As well as it's parent (F)
		if (strcmp(word, P->keyWord) < 0) {Q = P; P = P->leftNode; AVL_comparisons++;} // BST Property to go left or right
		else                              {Q = P; P = P->rightNode; AVL_comparisons++;}
	}

	// P fell off the tree and is NULL, Q is pointing to P's parent, and the parent of the node where Y needs to be inserted.
	Y = new AVLNode();
	strcpy(Y->keyWord, word);

	// Check if Y will be Q's Left or Right child
	if (strcmp(word, Q->keyWord) < 0) {Q->leftNode = Y; AVL_comparisons++; AVL_pointerChange++;}
	else                              {Q->rightNode = Y; AVL_comparisons++; AVL_pointerChange++;}

	// Check whether the displacement from A is left or right
	if (strcmp(word, A->keyWord) > 0) {B = P = A->rightNode; d = -1; AVL_comparisons++;}
	else                              {B = P = A->leftNode; d = 1; AVL_comparisons++;}

	// P is now one node below A. Adjust all of the BFs up to the insertion point. Don't tamper with Node Y.
	while (P != Y)
	{
		if (strcmp(word, P->keyWord) > 0) {P->BF = -1; P = P->rightNode; AVL_comparisons++; BF_Changes++;}
		else                              {P->BF = 1; P = P->leftNode; AVL_comparisons++; BF_Changes++;}
	}

	// Check if the BF at node A pushed the tree into balance, if the balance is still "Acceptable" enough
	// or out of balance
	if (A->BF == 0) // Tree was set to a slight (acceptable) imbalance of d=+/-1, in which the 
	{               // tree was originally completely balanced
		A->BF = d;
		BF_Changes++;
		return;
	}

	if (A->BF == -d) // If a slight imbalance in the other direction, then check if the insertion threw the tree
	{                // into complete balance (BF=0), and return.
		A->BF = 0;
		BF_Changes++;
		return;
	}

	// If neither of the two conditions above returned, then the tree went from acceptably imbalanced to unacceptably
	// imbalanced, in which a rotation is needed to be applied.
	if (d == 1) // Left imbalance. LL or LR?
	{
		if (B->BF == 1) // LL rotation
		{
			// Change the child pointers at A and B to reflect the rotation
			// Adjust the BFs at A & B

			AVLNode *BR = B->rightNode; // Get a reference to the right node of B which will be moved to the left node of A

			// Perform rotation
			B->rightNode = A; // Set right node of B to node A, while still keeping reference to original right node in BR
			A->leftNode = BR; // Move the original right node of B to the left node of A
			AVL_pointerChange+=2;

			// Adjust the BF's of A and B to zero, since they are now balanced
			A->BF = 0;
			B->BF = 0;
			BF_Changes+=2;
		}
		else // LR Rotation (3 cases with switch statement)
		{
			// Adjust the child pointers of nodes A, B, & C to reflect the new post-rotation structure 

			C = B->rightNode; // C is B's right child
			CL = C->leftNode; // CL and CR are C's left and right children
			CR = C->rightNode; 

			switch (C->BF)
			{
				// Set the new BF's at A and B, based on the BF at C.
				// Note: There are 3 sub-cases
				case 0:
				{
					// The BF at C is zero
					AVLNode* AL = B;

					C->rightNode = A; // Right node of C set to A
					C->leftNode = AL; // Left node of C set to original B 
					A->leftNode = NULL;
					B->rightNode = NULL;
					AVL_pointerChange+=2;

					// Set Balance Factors for A and B to 0 for complete balance
					A->BF = 0;
					B->BF = 0;
					BF_Changes+=2;
					break;
				}
				case 1:
				{
					// The BF at C is one, so insert to CL
					AVLNode* AL = B; // Keep reference to node B
					AVLNode* AR = A->rightNode; // Keep reference to A's right node

					C->rightNode = A; // Set right node of node C to node A
					C->leftNode = AL; // Set left node of C to node B
					A->leftNode = CR; // Set left node of A to CR
					A->rightNode = AR; // Set right node of A back to it's right node
					B->rightNode = CL; // Set right node of B to CL
					AVL_pointerChange+=5;

					// Set Balance Factors
					A->BF = -1;
					B->BF = 0;
					BF_Changes+=2;
					break;
				}
				case -1:
				{
					// The BF at C is negative one, so insert to CR
					AVLNode* AL = B; // Keep reference to node B
					AVLNode* AR = A->rightNode; // Keep reference to A's right node

					C->rightNode = A; // Set right node of node C to node A
					C->leftNode = AL; // Set left node of C to node B
					A->leftNode = CR; // Set left node of A to CR
					A->rightNode = AR; // Set right node of A back to it's right node
					B->rightNode = CL; // Set right node of B to CL
					AVL_pointerChange+=5;

					// Set Balance Factors
					A->BF = 0;
					B->BF = 1;
					BF_Changes+=2;
				}

			}

			C->BF = 0; // Regardless, C is now balanced
			BF_Changes++;
			B = C; // B is the root of the now-rebalanced subtree (recycle)
		} // End of LR rotation
	} // End of d=+1 (L)
	else // d == -1
	{
		// RR or RL?
		if (B->BF == 1) // RL rotation (3 cases with switch statement)
		{
			// Adjust the child pointers of nodes A, B, & C to reflect the new post-rotation structure 

			C = B->leftNode; // C is B's left child
			CL = C->leftNode; // CL and CR are C's left and right children
			CR = C->rightNode;

			switch (C->BF)
			{
				// Set the new BF's at A and B, based on the BF at C.
				// Note: There are 3 sub-cases
				case 0:
				{
					// The BF at C is zero
					AVLNode* AR = B;
	
					C->leftNode = A; // Left node of C set to A
					C->rightNode = AR; // Right node of C set to original B 
					A->rightNode = NULL;
					B->leftNode = NULL;
					AVL_pointerChange+=2;

					// Set Balance Factors for A and B to 0 for complete balance
					A->BF = 0;
					B->BF = 0;
					BF_Changes+=2;
					break;
				}
				case 1:
				{
					// The BF at C is one, so insert to CL
					AVLNode* AL = A->leftNode; // Keep reference to A's left node
					AVLNode* AR = B; // Keep reference to node B

					C->leftNode = A; // Set right node of node C to node A
					C->rightNode = AR; // Set left node of C to node B
					A->leftNode = AL;
					A->rightNode = CL; // Set left node of A to CR
					B->leftNode = CR; // Set right node of A back to it's right node
					AVL_pointerChange+=5;

					// Set Balance Factors
					A->BF = 0;
					B->BF = -1;
					BF_Changes+=2;
					break;
				}
				case -1:
				{
					// The BF at C is negative one, so insert to CR
					AVLNode* AL = A->leftNode; // Keep reference to A's left node
					AVLNode* AR = B; // Keep reference to node B

					C->leftNode = A; // Set right node of node C to node A
					C->rightNode = AR; // Set left node of C to node B
					A->leftNode = AL;
					A->rightNode = CL; // Set left node of A to CR
					B->leftNode = CR; // Set right node of A back to it's right node
					AVL_pointerChange+=5;

					// Set Balance Factors
					A->BF = 1;
					B->BF = 0;
					BF_Changes+=2;
				}

			}

			C->BF = 0; // Regardless, C is now balanced
			BF_Changes++;
			B = C; // B is the root of the now-rebalanced subtree (recycle)

		}
		else // RR rotation
		{
			// Change the child pointers at A and B to reflect the rotation
			// Adjust the BFs at A & B

			AVLNode *BL = B->leftNode; // Get a reference to the left node of B which will be moved to the right node of A

			// Perform rotation
			B->leftNode = A; // Set right node of B to node A, while still keeping reference to original right node in BR
			A->rightNode = BL; // Move the original right node of B to the left node of A
			AVL_pointerChange+=2;

			// Adjust the BF's of A and B to zero, since they are now balanced
			A->BF = 0;
			B->BF = 0;
			BF_Changes+=2;
		} // End of RR rotation
	}// End of d=-1 (R)

	// Finish up:

	// Regardless, the subtree rooted at B has been rebalanced, and needs to be the new child 
	// of F. The original subtree of F had root A.

	// Was the whole tree's root rebalanced?
	if (F == NULL) {root = B; return;} // B is the tree's new root - done

	// The root is now B. If rebalanced subtree is left of F, then B needs to be left of F; otherwise
	// if A was right of F, then B needs to be right of F.
	if (A == F->leftNode) {F->leftNode = B; AVL_pointerChange++; return;}
	if (A == F->rightNode) {F->rightNode = B; AVL_pointerChange++; return;}
	cout << "We should never be here\n";

} // End of AVL_Insert

  
int AVL::printAVLTreeHeight()
{
	// This method is responsible for printing out the height of the tree to the user. Returns the height of the tree.
	treeHeight = 0; // Reset the tree height

	computeAVLTreeHeight(root);

	return treeHeight;
}

void AVL::computeAVLTreeHeight(AVLNode* p)
{
	// This method is private and unaccessible to the user. It's used to traverse the tree and find the longest path to a null node, which will be the height of the tree.
	// The input is a node to start the traversal from. This method returns the height of the tree. Pointer p is the root of the tree.
	if (p != NULL) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		AVL_tempTreeHeight++; // Increment the temperary tree height
		if (AVL_tempTreeHeight > treeHeight) treeHeight = AVL_tempTreeHeight; // When going down a left node, if the temp gets larger then the current height, then the new largest height is the temp

		computeAVLTreeHeight(p->leftNode);

		computeAVLTreeHeight(p->rightNode);

		AVL_tempTreeHeight--; // Decrement the temporary tree height if going back from a right node
	}
}

string AVL::AVL_Search()
{
	// Used to return a string that contains the total number of nodes, as well as the 
	// total number of words
	totalNumberOfNodes = 0; // Reset
	totalNumberOfWords = 0;

	AVL_Traversal(root);

	return "	Total number of Distinct Words: " + to_string(totalNumberOfNodes) + "\n	Total number of Words: " + 
		to_string(totalNumberOfWords) + "\n" + "	Total number of Balance Factor Changes: " + to_string(BF_Changes) +
		"\n	Total number of Key Comparisons: " + to_string(AVL_comparisons) + "\n	Total number of Pointer Changes: " + 
		to_string(AVL_pointerChange) + "\n	Total Amount of Work Done: "+ to_string(BF_Changes + AVL_comparisons + AVL_pointerChange)
		+ "\n";
}

void AVL::AVL_Traversal(AVLNode *p)
{
	// This method traverses the tree to get the total number of nodes, as well as the total number of words in the 
	// tree through the node's count variables

	if (p != NULL) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		AVL_Traversal(p->leftNode);
		// In the middle node, capture it's count occurence
		totalNumberOfNodes++;           // Increase node encounter by 1
		totalNumberOfWords += p->count; // Increase number of words encountered by 1

		AVL_Traversal(p->rightNode);
	}
}