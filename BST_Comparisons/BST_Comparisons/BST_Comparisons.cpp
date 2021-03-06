/*	BST_Comparisons.cpp
	Developer: Christian Ropchock
	Class: EECS 2510, Spring 2018
	Date: 03/14/2018

	This is the main program file that is responsible for performing the analysis of the 4 different types of Binary Search Trees. These
	3 trees include BST, AVL Tree, RB Tree, and the Skip List. There will be an instance created of each of these trees, a text file will be 
	parsed and stored inside each of these trees, and their performances and overhead will be measured during the execution of each tree, and 
	compared to one and another. 

*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "BST.h"
#include "AVL.h"
#include "RBT.h"
#include "SkipList.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

int main()
{
	char c;
	RBT RBT_T;
	AVL AVL_T;
	BST BST_T;
	SkipList SL;

	clock_t overHeadTime; // Variable that captures the overhead time to read the text file
	clock_t BST_Time;
	clock_t AVL_Time;
	clock_t RBT_Time;
	clock_t SKPLST_Time;
	
	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	bool IsDelimiter = false, WasDelimiter = false;
	ifstream inFile;
	inFile.open("C:\\Users\\Christian\\Documents\\College Documents\\EECS 2510 Spring 2018\\DiskBasedTrees\\Debug\\Shakespeare.txt", ios::binary);
	if (inFile.fail())
	{
		cout << "Unable to open input file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";
		cin.get(c);
		exit(1);
	}
	iPtr = 0;
	inFile.get(c); // priming read

//=====================================================================================================================================================
// First System Buffer Run

	while (!inFile.eof())
	{
		// First run to buffer the system
		inFile.get(c);
	}

	inFile.clear();
	inFile.seekg(0, ios::beg);

//=====================================================================================================================================================
// Second Read Time Run

	overHeadTime = clock();
	while (!inFile.eof())
	{
		// Second run to capture the time of the read
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the								  
										  // last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			// Inserts go here, without inserts
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration

		inFile.get(c);
	}
	overHeadTime = clock() - overHeadTime;

	inFile.clear();
	inFile.seekg(0, ios::beg);

//=====================================================================================================================================================
// BST RUN

	BST_Time = clock();
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the								  
										  // last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			BST_T.BST_Insert(chari); // insert it in the BST
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration

		inFile.get(c);
	}
	BST_Time = clock() - BST_Time - overHeadTime;

	cout << "Binary Search Tree Statistics\n	Time to run: " << to_string(BST_Time / CLOCKS_PER_SEC) << "." <<
		to_string(BST_Time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) << " seconds.\n" << BST_T.BST_Search() <<
		"	Height: " << BST_T.printBSTTreeHeight();

	inFile.clear();
	inFile.seekg(0, ios::beg);

//=====================================================================================================================================================
// AVL RUN

	AVL_Time = clock();
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the								  
										  // last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			AVL_T.AVL_Insert(chari); // insert it in the AVL Tree
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration

		inFile.get(c);
	}
	AVL_Time = clock() - AVL_Time - overHeadTime;

	cout << "\nAVL Tree Statistics\n	Time to run: " << to_string(AVL_Time / CLOCKS_PER_SEC)
		<< "." << to_string(AVL_Time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) << " seconds.\n" << AVL_T.AVL_Search() <<
		"	Height: " << AVL_T.printAVLTreeHeight();

	inFile.clear();
	inFile.seekg(0, ios::beg);

//=====================================================================================================================================================
// RBT RUN

	RBT_Time = clock();
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the								  
										  // last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			RBT_T.RBT_Insert(chari); // insert this word in the RBT
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration

		inFile.get(c);
	}
	RBT_Time = clock() - RBT_Time - overHeadTime;

	cout << "\nRBT Tree Statistics\n	Time to run: " << to_string(RBT_Time / CLOCKS_PER_SEC) << "." <<
		to_string(RBT_Time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) << " seconds.\n" << RBT_T.RBT_Search() <<
		"	Height: " << RBT_T.printRBTTreeHeight();

	inFile.clear();
	inFile.seekg(0, ios::beg);

//=====================================================================================================================================================
// SKIP LIST RUN

	SKPLST_Time = clock();
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the								  
										  // last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			SL.Skip_Insert(chari); // insert it in the skip list
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration

		inFile.get(c);
	}
	SKPLST_Time = clock() - SKPLST_Time - overHeadTime;

	cout << "\nSkip List Statistics\n	Time to run: " << to_string(SKPLST_Time / CLOCKS_PER_SEC)
		<< "." << to_string(SKPLST_Time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) << " seconds.\n" << SL.searchSkipList() <<
		"	Height: " << SL.getHeight();

	inFile.close();

    return 0;
}

