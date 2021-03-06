/*	SPAN.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 04/28/2018

This is the main program file that is responsible for parsing a text file containing information for a Spanning Tree in terms of:
N (The number of vertices), the names for each vertex in Excel Column Header format, the adjacency matrix for the vertices. This
file will create an instance of a Spanning Tree and run both it's Kruskal and Prim algorithms on it, and output the total length of
the shortest path, as well as each individual path and their associated weights. 

*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

#include "SpanTree.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

int main(int argc, char **argv)
{
	// First read in the lines, starting with N, which is number of vertices, then through the 
	// next N lines for the vertex names. Afterwards read the adjacency matrix, which will be read as 
	// space delimeted characters that are then converted to INTs, and stored into a NxN array. 


	SpanTree SPT_T;
	char **vertices; // Pointer that points at beginning of array that is going to hold the names of the Vertices
	double **adjMatrix; // Pointer array that points to beginning pointers of individual pointer arrays of ints (2D Matrix)

	ifstream inFile;
	inFile.open(argv[1], ios::binary);
	if (inFile.fail())
	{
		cout << "Unable to open input file\n\n"
			 << "Program Exiting\n\nPress ENTER to exit\n";
		exit(1);
	}

	inFile >> SPT_T.N; // Read number of vertices in

	vertices = new char*[SPT_T.N];

	for (int i = 0; i < SPT_T.N; i++)
	{
		vertices[i] = new char[2]; // Initialize each pointer array to have length of 2 chars
		inFile >> vertices[i];
	}

	adjMatrix = new double*[SPT_T.N]; // Set number of rows

	for (int i = 0; i < SPT_T.N; i++) // For each row
	{
		adjMatrix[i] = new double[SPT_T.N]; // Initialize number of columns for each row

		for (int j = 0; j < SPT_T.N; j++) // Read in each value in each col for Adjaceny Matrix
			inFile >> adjMatrix[i][j];
	}

	cout << "=========================Kruskal Output=========================" << endl;
	SPT_T.MST_Kruskal(vertices, adjMatrix);
	cout << "=========================Prim Output==============================" << endl;
	SPT_T.MST_Prim(vertices, adjMatrix, vertices[0]);

    return 0;
}

