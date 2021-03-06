/*	DiskBasedTrees.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 04/14/2018

This is the main program file that is responsible for performing the analuysis of two different trees using disk 
based operations. This program will first read a text file for overhead, and insert all of the words of any ASCII based file
into each tree accordingally, also making sure to moniter the performance of the tree in terms of runtime and memory size. 

*/
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "AVL.h"
#include "BTree.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

int main()
{
	char c;
	AVL AVL_T;
	BTree BTree_T;

	clock_t overHeadTime; // Variable that captures the overhead time to read the text file
	clock_t AVL_Time;
	clock_t BTree_Time;

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
		"	Height: " << AVL_T.printAVLTreeHeight() << endl;

	AVL_T.~AVL(); // Destroy the AVL Tree

	inFile.clear();
	inFile.seekg(0, ios::beg);

	//=====================================================================================================================================================
	// BTree RUN

	BTree_Time = clock();
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the								  
										  // last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			BTree_T.B_Tree_Insert(chari); // insert it in the AVL Tree
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
	BTree_Time = clock() - BTree_Time - overHeadTime;

	cout << "\nBTree Statistics\n	Time to run: " << to_string(AVL_Time / CLOCKS_PER_SEC)
		<< "." << to_string(BTree_Time / (CLOCKS_PER_SEC / 1000)).substr(0, 3) << " seconds.\n" << BTree_T.B_Tree_Search() << endl;

	BTree_T.~BTree(); // Destroy the AVL Tree

	inFile.close();

	return 0;
}

