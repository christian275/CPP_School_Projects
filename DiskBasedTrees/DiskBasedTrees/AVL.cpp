/*	AVL.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 04/15/2018

This is the cpp file that defines the class for an AVL Tree. The AVL Tree is responsible for inserting words into the tree, as well as returning
the number of comparisons made, reference swaps, and the height of the tree. This tree implements Balance Factors that determine whether the tree
is still in balance or not, and implements rotations to put the tree back in balance. 

Tree Saved in disk file named: "AVLTreeInfo.txt"

Each node = 50 bytes

*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

#include "AVL.h"

#define _CRT_SECURE_NO_WARNINGS

fstream AVLFile;

int AVL_tempTreeHeight = 0; // Global variable used to track the current height of the tree during the traversal
int AVL_diskWriteCount = 0; // Tracks # of disk writes
int AVL_diskReadCount = 0; // Tracks # of disk reads


AVL::AVL()
{
	// Constructor for AVL Tree
	root = -1; // Set root to -1 for NULL

	AVLFile.open("C:\\AVLTreeInfo.txt", ios::in | ios::out | ios_base::trunc | ios::binary);

	if (AVLFile.fail())
	{
		cout << "Unable to open tree info file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";
		exit(1);
	}
}

AVL::~AVL()
{
	// Destructor for AVL Tree
	AVLFile.close();
}

void AVL::AVL_Insert(char word[])
{
	// Method used to insert a word into an AVL Tree. The process involves doing a "dumb" BST insert into the tree. Then after performing the insert, the Balance Factors in the 
	// Tree are updated all the way up towards the root. If one of the Balance factors were to go to +/-2, then a rotation is performed on the grandparent of the node that was 
	// inserted. There are 4 different types of rotations, RR, LL, RL, and LR. LR and RL both also contain 3 different subcases.

	// Initialize all to -1 for NULL
	int Y = -1; // The new node that is inserted into the AVL Tree
	int A = -1, B = -1, F = -1; // A is the last parent above Y with BF=+/-1, B is the child of A used to determine type of rotation, F is A's parent
	int P = -1, Q = -1; // P is used to scan the tree until a NULL node is found for doing a "dumb" insert, Q is P's parent
	int C = -1, CL = -1, CR = -1; // Node C is either B's left or right child, depending on the location of the imbalance*/

	AVLNode ANode, BNode, CNode; // The 3 nodes that are needed in memory

	int d; // Displacement used to adjust BFs

	if (root == -1) // If the root is NULL, then insert and exit the method
	{
		AVLNode YNode;
		AVLNode nullNode; // Used to fill up position 0 in disk file
		strcpy(YNode.keyWord, word); // Copy the word into the new node
		Disk_Write(nullNode, -1);
		root = Disk_Write(YNode, -1); //Set the new root  
		AVL_diskWriteCount += 2;
		return;
	}

	ANode = Disk_Read(root); // Read the root in from the disk file, and populate it into node A to begin traversal for insertion 
	                         // (Acts as node P)
	AVL_diskReadCount++;

	A = root;
	P = root; // Set P to contain the position of the root, and save the traversal position of A

	while (P != -1) // Search the tree until P falls off
	{
		if (strcmp(word, ANode.keyWord) == 0) { ANode.count++; Disk_Write(ANode, P); AVL_diskWriteCount++; return; } // The word is already in the tree  
		if (ANode.BF != 0)     // Track the most recent place where a non-zero BF was found (ANode (P))
			{A = P; F = Q;}    // As well as it's parent (F)
		// Change both the pointer number, as well as the temporary node that holds the info for the pointers
		if (strcmp(word, ANode.keyWord) < 0) 
		{ 
			Q = P;  
			BNode = Disk_Read(Q); // Populate node Q with P from Disk File
			P = ANode.leftNode;  // Set pointer number P to left Node of A
			ANode = Disk_Read(P);  // Read leftNode of P into A for child traversal
		} 
		else
		{ 
			Q = P;  
			BNode = Disk_Read(Q); 
			P = ANode.rightNode; 
			ANode = Disk_Read(P);
		}

		AVL_diskReadCount += 2;

	}

	// P fell off the tree and is NULL, Q is pointing to P's parent, and the parent of the node where Y needs to be inserted.
	strcpy(CNode.keyWord, word);

	// At this moment ANode = P, BNode = Q, and CNode = newNode

	Y = Disk_Write(CNode, -1); // Write it to append to end of disk file, and save in integer value Y  
	AVL_diskWriteCount++;

	// Check if Y will be Q's Left or Right child, left and right childs are -1 and empty
	if (strcmp(word, BNode.keyWord) < 0) 
	{ 
		BNode.leftNode = Y;  // Set left Node to position of newNode Y
		Q = Disk_Write(BNode, Q); // Update Q in Disk File
	}
	else 
	{ 
		BNode.rightNode = Y; // Set right Node to postion of newNode Y
		Q = Disk_Write(BNode, Q); // Update Q in Disk File
	}

	AVL_diskWriteCount++;

	BNode = Disk_Read(A); // Populate BNode with A for displacement 

	// Check whether the displacement from A is left or right
	if (strcmp(word, BNode.keyWord) > 0) { B = P = BNode.rightNode; d = -1; }
	else { B = P = BNode.leftNode; d = 1; }

	ANode = Disk_Read(P); // Since pointer P changed, update that in ANode

	AVL_diskReadCount += 2;

	// P is now one node below A. Adjust all of the BFs up to the insertion point. Don't tamper with Node Y.
	while (P != Y)
	{
		if (strcmp(word, ANode.keyWord) > 0) 
		{ 
			ANode.BF = -1; 
			P = Disk_Write(ANode, P); // Update P in disk file with new BF 
			P = ANode.rightNode; // Set P to point where rightNode of P was
			ANode = Disk_Read(P); // ANode now points at new P   
		}
		else 
		{
			ANode.BF = 1;
			P = Disk_Write(ANode, P); 
			P = ANode.leftNode; 
			ANode = Disk_Read(P); 
		}

		AVL_diskWriteCount++;
		AVL_diskReadCount++;

	}

	// Check if the BF at node A pushed the tree into balance, if the balance is still "Acceptable" enough
	// or out of balance
	if (BNode.BF == 0) // Tree was set to a slight (acceptable) imbalance of d=+/-1, in which the 
	{               // tree was originally completely balanced
		BNode.BF = d;
		A = Disk_Write(BNode, A); // Update A in disk file with new BF 
		AVL_diskWriteCount++;
		return;
	}

	if (BNode.BF == -d) // If a slight imbalance in the other direction, then check if the insertion threw the tree
	{                   // into complete balance (BF=0), and return.
		BNode.BF = 0;
		A = Disk_Write(BNode, A);
		AVL_diskWriteCount++;
		return;
	}

	// Set the A and B nodes for rebalancing and rotations
	ANode = Disk_Read(A);
	BNode = Disk_Read(B);

	AVL_diskReadCount += 2;

	// If neither of the two conditions above returned, then the tree went from acceptably imbalanced to unacceptably
	// imbalanced, in which a rotation is needed to be applied.
	if (d == 1) // Left imbalance. LL or LR?
	{
		if (BNode.BF == 1) // LL rotation
		{
			// Change the child pointers at A and B to reflect the rotation
			// Adjust the BFs at A & B

			int BR = BNode.rightNode; // Get a reference to the right node of B which will be moved to the left node of A
			                          // Store BR in it's own variable

			// Perform rotation
			BNode.rightNode = A; // Set right node of B to node A, while still keeping reference to original right node in BR
			ANode.leftNode = BR; // Move the original right node of B to the left node of A

			// Adjust the BF's of A and B to zero, since they are now balanced
			ANode.BF = 0;
			BNode.BF = 0;

			//Overwrite new contents of nodes A and B in disk file 
			A = Disk_Write(ANode, A); 
			B = Disk_Write(BNode, B); 
			AVL_diskWriteCount += 2;
		}
		else // LR Rotation (3 cases with switch statement)
		{
			// Adjust the child pointers of nodes A, B, & C to reflect the new post-rotation structure 

			C = BNode.rightNode; // C is B's right child
			CNode = Disk_Read(C); // Read in info for node C
			AVL_diskReadCount++;

			CL = CNode.leftNode; // CL and CR are C's left and right children
			CR = CNode.rightNode;

			switch (CNode.BF)
			{
				// Set the new BF's at A and B, based on the BF at C.
				// Note: There are 3 sub-cases
				case 0:
				{
					// The BF at C is zero
					int AL = B;

					CNode.rightNode = A; // Right node of C set to A
					CNode.leftNode = AL; // Left node of C set to original B 
					ANode.leftNode = -1; // Set to -1 to indicate NULL
					BNode.rightNode = -1;

					// Set Balance Factors for A and B to 0 for complete balance
					ANode.BF = 0;
					BNode.BF = 0;

					break;
				}
				case 1:
				{
					// The BF at C is one, so insert to CL
					int AL = B; // Keep reference to node B
					int AR = ANode.rightNode; // Keep reference to A's right node

					CNode.rightNode = A; // Set right node of node C to node A
					CNode.leftNode = AL; // Set left node of C to node B
					ANode.leftNode = CR; // Set left node of A to CR
					ANode.rightNode = AR; // Set right node of A back to it's right node
					BNode.rightNode = CL; // Set right node of B to CL

					// Set Balance Factors
					ANode.BF = -1;
					BNode.BF = 0;

					break;
				}
				case -1:
				{
					// The BF at C is negative one, so insert to CR
					int AL = B; // Keep reference to node B
					int AR = ANode.rightNode; // Keep reference to A's right node

					CNode.rightNode = A; // Set right node of node C to node A
					CNode.leftNode = AL; // Set left node of C to node B
					ANode.leftNode = CR; // Set left node of A to CR
					ANode.rightNode = AR; // Set right node of A back to it's right node
					BNode.rightNode = CL; // Set right node of B to CL

					// Set Balance Factors
					ANode.BF = 0;
					BNode.BF = 1;

				}

			}

			// Write out updated contents of nodes A, B, and C to disk file
			A = Disk_Write(ANode, A); 
			B = Disk_Write(BNode, B); 
			C = Disk_Write(CNode, C); 

			CNode.BF = 0; // Regardless, C is now balanced
			BNode = CNode; // B is the root of the now-rebalanced subtree (recycle)
			B = C; // Point B to where C was

			Disk_Write(BNode, B); // Overwrite contents of C with B with updated BF  //Replace B
			AVL_diskWriteCount += 4;

		} // End of LR rotation
	} // End of d=+1 (L)
	else // d == -1
	{
		// RR or RL?
		if (BNode.BF == 1) // RL rotation (3 cases with switch statement)
		{
			// Adjust the child pointers of nodes A, B, & C to reflect the new post-rotation structure 

			C = BNode.leftNode; // C is B's right child
			CNode = Disk_Read(C); // Read in info for node C
			AVL_diskReadCount++;

			CL = CNode.leftNode; // CL and CR are C's left and right children
			CR = CNode.rightNode;

			switch (CNode.BF)
			{
				// Set the new BF's at A and B, based on the BF at C.
				// Note: There are 3 sub-cases
				case 0:
				{
					// The BF at C is zero
					int AR = B;

					CNode.leftNode = A; // Left node of C set to A
					CNode.rightNode = AR; // Right node of C set to original B 
					ANode.rightNode = -1;
					BNode.leftNode = -1;

					// Set Balance Factors for A and B to 0 for complete balance
					ANode.BF = 0;
					BNode.BF = 0;

					break;
				}
				case 1:
				{
					// The BF at C is one, so insert to CL
					int AL = ANode.leftNode; // Keep reference to A's left node
					int AR = B; // Keep reference to node B

					CNode.leftNode = A; // Set right node of node C to node A
					CNode.rightNode = AR; // Set left node of C to node B
					ANode.leftNode = AL;
					ANode.rightNode = CL; // Set left node of A to CR
					BNode.leftNode = CR; // Set right node of A back to it's right node

					// Set Balance Factors
					ANode.BF = 0;
					BNode.BF = -1;
					break;
				}
				case -1:
				{
					// The BF at C is negative one, so insert to CR
					int AL = ANode.leftNode; // Keep reference to A's left node
					int AR = B; // Keep reference to node B

					CNode.leftNode = A; // Set right node of node C to node A
					CNode.rightNode = AR; // Set left node of C to node B
					ANode.leftNode = AL;
					ANode.rightNode = CL; // Set left node of A to CR
					BNode.leftNode = CR; // Set right node of A back to it's right node

					// Set Balance Factors
					ANode.BF = 1;
					BNode.BF = 0;
				}

			}

			A = Disk_Write(ANode, A); 
			B = Disk_Write(BNode, B); 
			C = Disk_Write(CNode, C); 

			CNode.BF = 0; // Regardless, C is now balanced
			BNode = CNode; // B is the root of the now-rebalanced subtree (recycle)
			B = C; // Point B to where C was

			Disk_Write(BNode, B); // Overwrite contents of C with B with updated BF  

			AVL_diskWriteCount += 4;

		}
		else // RR rotation
		{
			// Change the child pointers at A and B to reflect the rotation
			// Adjust the BFs at A & B

			int BL = BNode.leftNode; // Get a reference to the left node of B which will be moved to the right node of A

            // Perform rotation
			BNode.leftNode = A; // Set right node of B to node A, while still keeping reference to original right node in BR
			ANode.rightNode = BL; // Move the original right node of B to the left node of A

			// Adjust the BF's of A and B to zero, since they are now balanced
			ANode.BF = 0;
			BNode.BF = 0;

			//Overwrite new contents of nodes A and B in disk file 
			A = Disk_Write(ANode, A); //Replace A
			B = Disk_Write(BNode, B); //Replace B

			AVL_diskWriteCount += 2;

		} // End of RR rotation
	}// End of d=-1 (R)

	 // Finish up:

	 // Regardless, the subtree rooted at B has been rebalanced, and needs to be the new child 
	 // of F. The original subtree of F had root A.

	CNode = Disk_Read(F); // Read node F into a node struct
	AVL_diskReadCount++;

	 // Was the whole tree's root rebalanced?
	if (F == -1) { root = B; return; } // B is the tree's new root - done

	// The root is now B. If rebalanced subtree is left of F, then B needs to be left of F; otherwise
	// if A was right of F, then B needs to be right of F.
	// CNode is acting as Node F.
	if (A == CNode.leftNode) { CNode.leftNode = B; Disk_Write(CNode, F); AVL_diskWriteCount++; return; }
	if (A == CNode.rightNode) { CNode.rightNode = B; Disk_Write(CNode, F); AVL_diskWriteCount++; return; }
	cout << "We should never be here\n";

} // End of AVL_Insert


int AVL::printAVLTreeHeight()
{
	// This method is responsible for printing out the height of the tree to the user. Returns the height of the tree.
	treeHeight = 0; // Reset the tree height

	computeAVLTreeHeight(root);

	return treeHeight;
}

void AVL::computeAVLTreeHeight(int startPos)
{
	// This method is private and unaccessible to the user. It's used to traverse the tree and find the longest path to a null node, 
	// which will be the height of the tree. The input is a node to start the traversal from. This method returns the height of the tree.
	// startPos is an integer that tracks position of the tree traversal, and startNode is it's Node holder.

	AVLNode startNode = Disk_Read(startPos);

	if (startPos != -1) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		AVL_tempTreeHeight++; // Increment the temperary tree height
		if (AVL_tempTreeHeight > treeHeight) treeHeight = AVL_tempTreeHeight; // When going down a left node, if the temp gets larger then the current height, then the new largest height is the temp

		computeAVLTreeHeight(startNode.leftNode);

		computeAVLTreeHeight(startNode.rightNode);

		AVL_tempTreeHeight--; // Decrement the temporary tree height if going back from a right node
	}
}

string AVL::AVL_Search()
{
	// Used to return a string that contains the total number of nodes, as well as the 
	// total number of words
	totalNumberOfNodes = -1; // Reset
	totalNumberOfWords = -1;

	AVL_Traversal(root);

	AVLFile.clear();
	AVLFile.seekg(0, ios::end);

	return "	Total number of Distinct Words: " + to_string(totalNumberOfNodes + 1) + "\n	Total number of Words: " +
		to_string(totalNumberOfWords + 1) + "\n	Total number of Disk Writes: " + to_string(AVL_diskWriteCount) +
		"\n	Total number of Disk Reads: " + to_string(AVL_diskReadCount) + "\n	Total Number of Disk Accesses: " + 
		to_string(AVL_diskWriteCount + AVL_diskReadCount) + "\n	Total Tree File Size: " + to_string(AVLFile.tellg()) + " B\n";
}

void AVL::AVL_Traversal(int startPos)
{
	// This method traverses the tree to get the total number of nodes, as well as the total number of words in the 
	// tree through the node's count variables

	if (startPos > 0) // Conditional used to stop traversing the tree down a branch if a NULL node is reached
	{
		AVLNode startNode = Disk_Read(startPos);
		AVL_Traversal(startNode.leftNode);
		// In the middle node, capture it's count occurence
		totalNumberOfNodes++;           // Increase node encounter by 1
		totalNumberOfWords += startNode.count; // Increase number of words encountered by 1

		AVL_Traversal(startNode.rightNode);
	}
}

AVL::AVLNode AVL::Disk_Read(int nodeNumber)
{
	// Used to read a node from a disk file that resides in a binary file on the disk. Have to buffer to
	// the correct node in the file using nodeNumber as a multiplier for the location. IE, 
	// NodeNumber = 5 = 5th node, and if NodeSize = 1000B, then start at loc 5kB.

	AVLNode readNode; // Node to read in from file

	AVLFile.clear();
	AVLFile.seekg(sizeof(readNode) * nodeNumber); // Locate position in file where node to be read resides

	AVLFile.read(readNode.keyWord, sizeof(readNode.keyWord));
	AVLFile.read((char *)(&readNode.count), sizeof(int));
	AVLFile.read((char *)(&readNode.BF), sizeof(int));
	AVLFile.read((char *)(&readNode.leftNode), sizeof(int));
	AVLFile.read((char *)(&readNode.rightNode), sizeof(int));

	return readNode; // Return the node at that location in the file

}

int AVL::Disk_Write(AVLNode wNode, int position)
{
	// Used to write a node to the disk file that resides on the disk or update node. It will start 
	// the write at the indicated position, and if position is -1, then will append to EOF. Update any time 
	// a node is changed in the code.

	// Returns position of new written node in disk file
	if (position == -1)
	{
		// If position is indicated at end of file, append new node to end of file
		AVLFile.clear();
		AVLFile.seekp(0, ios_base::end); // Write at end of file

		AVLFile.write(wNode.keyWord, sizeof(wNode.keyWord));
		AVLFile.write((char *)(&wNode.count), sizeof(int));
		AVLFile.write((char *)(&wNode.BF), sizeof(int));
		AVLFile.write((char *)(&wNode.leftNode), sizeof(int));
		AVLFile.write((char *)(&wNode.rightNode), sizeof(int));

		return (AVLFile.tellp() / sizeof(wNode)) - 1; // Divide the total file size by size of each file to get location 										   
												   // of newly added node - 1 because of 0-index based
	}
	else
	{
		// Else update the node already residing in a position
		AVLFile.clear();
		AVLFile.seekp(position * sizeof(wNode));

		AVLFile.write(wNode.keyWord, sizeof(wNode.keyWord));
		AVLFile.write((char *)(&wNode.count), sizeof(int));
		AVLFile.write((char *)(&wNode.BF), sizeof(int));
		AVLFile.write((char *)(&wNode.leftNode), sizeof(int));
		AVLFile.write((char *)(&wNode.rightNode), sizeof(int));

		return position; // Divide the total file size by size of each file to get location 										   
	                     // of newly added node - 1 because of 0-index based
	}
}
