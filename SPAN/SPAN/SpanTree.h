/*	SpanTree.h
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 04/28/2018

This is a header for SpanTree.cpp. 

*/

#pragma once

using namespace std;

class SpanTree
{
public:
	SpanTree();
	void MST_Kruskal(char** colNames, double** adjMatrix);
	void MST_Prim(char** colNames, double** adjMatrix, char* arbVertex);

	int N; // Used to store number of vertices

private:
// Kruskal Information ======================================================================================================
	struct Set
	{
		// The set that contains all the vertices in A implemented as a LL with a root pointer, as well as
		// pointers to the next set element in the set.

		Set *subRoot = NULL; // Defines the root for each set element that will in turn have its own 
		                     // LL, each subSet element's subRoot will be NULL 

		char setName[2]; // The element associated with the set

		Set *nextSet = NULL; // Defines the next set pointer of this set element

	};

	struct Edges
	{
		// This struct contains the edges and their associated weights that is derived from the 
		// adjacency matrix

		char edge[2][2]; // char[0] contains the starting vertex of the path, and char[1] is the ending vertex 

		double weight = 0; // Weight of the edge

		bool shortFlag = false; // Flag used to mark whether the path was used for shortest path traversal

		bool edgeChecked = true; // Used for printing the edges

		Edges *nextEdge = NULL; // The next element in the LL
	};

	Set *kruskalRoot; // The root of set A
	Edges *edgeRoot; // The root of the Edges LL

	void Make_Set(char* vertices);
	void Make_Vertices(double weight, string u, string v);
	Set * Find_Set(char word[2]);
	void Union(Set * u, Set * v);
	void PrintKruskal();
// =============================================================================================================================

// Prim Information ============================================================================================================

	struct HeapVertex
	{
		// This is a struct that will define all the elements of a vertex. This struct will contain the key as well as the PI
		// reference for the vertex. 
		double key; // Key that is updated for the current minimum path in the priority queue

		char vertexName[2];

		char PI[2];

		int inHeap = true; // Specifies whether the vertex is still in the heap

		bool alreadyChecked = true; // Used for printing out vertices in alphabetical order
	};

	HeapVertex **PQ = new HeapVertex*[N]; // Array that will act as the heap priority queue
	
	int HeapSize = 0; // Current size of the Priority Queue

	HeapVertex Extract_Min(); // Used to extract the vertex that is going to be removed from the Heap after it is deemed already searched
	void Min_Heapify(int mapPos); // Method to update the heap
	void DecreaseValue(int valPos); // Method to float the decrease value up to the top
	void PrintPrim();
// =============================================================================================================================
};
