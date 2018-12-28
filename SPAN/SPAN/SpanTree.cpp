/*	SpanTree.cpp
Developer: Christian Ropchock
Class: EECS 2510, Spring 2018
Date: 04/28/2018

This is the cpp file that defines the class for a Spanning Tree. There are two separate types of algorithms implemented for a Spanning Tree that will be
used in this file. One type is Kruskal's Algorithm which will implement a linked list of sets, which in turn will each keep their own sets which will also
be implemented using linked lists. There will also be an array of edge structs that will each contain the two vertices that create an edge, as well as that
edge's associated weight. Next will be Prim's alorithms, which will implement a heap for it's priority queue which is used in set Y, which looks for the minimum
path to each of the vertices outlined in the set of vertices already visited.

*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include "SpanTree.h"

#define _CRT_SECURE_NO_WARNINGS

SpanTree::SpanTree()
{
	// Constructor for Spanning Tree
	
}

void SpanTree::MST_Kruskal(char** colNames, double** adjMatrix)
{
	// Performs the operations for creating a Spanning Tree using the Kruskal algorithm for finding the MST Spanning Tree. This 
	// algorithm is not in-place, in which is uses auxillary data structures to find the minimum path to visit every vertex in 
	// a graph. This algorithm creates a LL set A, and a LL set Vertices. A will track all of the sets which were already visited
	// in the graph, while Vertices will contain all the vertex information in increasing order according to weight.

	for (int i = 0; i < N; i++)
		Make_Set(colNames[i]);

	Set *ptr = kruskalRoot;

	while (ptr != NULL)
		ptr = ptr->nextSet;

	for (int i = 0; i < N; i++) // Row
	{
		for (int j = 0; j < N; j++) // Col
		{
			if (adjMatrix[i][j] != 0)
				Make_Vertices(adjMatrix[i][j], colNames[i], colNames[j]); // Update the edge array for all non-zero edges in adjacency matrix
		}
	}

	Edges *edgePtr = edgeRoot; // Ptr to traverse all the edges

	while (edgePtr != NULL)
	{
		// Loop through every edge in the edge matrix
		if (Find_Set(edgePtr->edge[0]) != Find_Set(edgePtr->edge[1]))
		{
			edgePtr->shortFlag = true;
			Union(Find_Set(edgePtr->edge[0]), Find_Set(edgePtr->edge[1]));
		}

		edgePtr = edgePtr->nextEdge;
	}

	PrintKruskal();

}

void SpanTree::MST_Prim(char** colNames, double** adjMatrix, char* arbVertex)
{
	// A method that implements Prim's algorithm to find the shortest path in a graph. This method has two separate 
	// sets. The first set Y is a priority queue that stores vertice information as they are visited, and finds all
	// of the shortest paths in the priority queue based on which vertex has a shortest path available. The other 
	// set is F which keeps track of all of the edges that were hit.

	// Parameter arbVertex: Is an arbitrarily chosen vertex to start the traversal at

	//ShortestPath *shortPathMap = new ShortestPath[N]; // Keeps a reference of all the shortest adjacent paths already visited for each vertex

	HeapSize = N; // Set the size of the Heap
	int rootIndex = 0;

	for (int i = 0; i < N; i++)
	{
		PQ[i] = new HeapVertex();
		strcpy(PQ[i]->vertexName, colNames[i]); // Set the Name values, as well as 
		if (strcmp(colNames[i], arbVertex) == 0) { PQ[i]->key = 0; rootIndex = i; } // Key to 0 if root      
		else                                     PQ[i]->key = INFINITY; // Key to infinity if any other node
	}

	Min_Heapify(rootIndex);


	while (PQ[0]->inHeap)
	{
		// Keep going until every node was visited in the Graph, in which it's minimum path was already extracted, 
		// and the minimum value at the top of the heap is NULL
		// All NULL values are considered the heaviest weights and sink to the bottom of the heap
		
		HeapVertex searchValue = Extract_Min(); // Extract MIN value in heap, and use as new search point in Graph
		int originalValueLoc = 0;

		for (int i = 0; i < N; i++)
		{
			if (strcmp(colNames[i], searchValue.vertexName) == 0)
			{
				originalValueLoc = i;
				break;
			}
		}

		for (int i = 0; i < N; i++)
		{
			// Traverse the row in the adjacency matrix for every possible non-zero path connected to the 
			// node with the value @ i

			if (adjMatrix[originalValueLoc][i] != 0) // Don't consider zero's, because they are non-existant paths
			{

				
				int vPos = -1; // Get position of v

				for (int j = 0; j < HeapSize; j++)
				{
					// Search for v in the priority queue based on it's name
					if (strcmp(PQ[j]->vertexName, colNames[i]) == 0) vPos = j;
				}

				if (vPos != -1 && PQ[vPos]->inHeap && adjMatrix[originalValueLoc][i] < PQ[vPos]->key)
				{
					strcpy(PQ[vPos]->PI, PQ[HeapSize]->vertexName);
					PQ[vPos]->key = adjMatrix[originalValueLoc][i];
					DecreaseValue(vPos);
				}
			}

		}

	}

	PrintPrim();

}

void SpanTree::Make_Set(char* vertices)
{
	// This method is responsible for creating and updating the set to contain nodes for the LL
	// that each point to the next corresponding nodes which are the vertices in the graph. Each
	// set element will initially contain a single vertex.

	Set *newNode = new Set(); // New node to be inserted into LL
	strcpy(newNode->setName, vertices);

	if (kruskalRoot == NULL) // Create new root for set A LL if it doesn't exist exist
	{
		kruskalRoot = newNode;
		return;
	}
	else if (strcmp(newNode->setName, kruskalRoot->setName) < 0)
	{
		newNode->nextSet = kruskalRoot;
		kruskalRoot = newNode;
		return;
	}

	Set *ptr; // Used to track position while navigating in LL
	ptr = kruskalRoot;
	Set *lagPtr = ptr;

	while (strcmp(newNode->setName, ptr->setName) > 0)
	{
		// Keep looking for a nextPtr that is NULL for insertion into LL. Insert so that all the nodes
		// in the LL are in alphabetical order.
		lagPtr = ptr;
		if (ptr->nextSet == NULL)
		{
			ptr->nextSet = newNode;
			return;
		}
		ptr = ptr->nextSet;
	}
	newNode->nextSet = ptr;
	lagPtr->nextSet = newNode;

}

void SpanTree::Make_Vertices(double weight, string u, string v)
{
	// Adds a new non-zero weighted edge node to the Vertices LL
	// u and v are both vertices for the edge

	// Adds to keep increasing order contraint intact, so
	// looks in LL until new node's weight doesn't exceed weight
	// of node in LL

	if (u > v)
	{
		// Conditional used to keep edges in alphabetical order
		string tempV = v;
		v = u;
		u = tempV;
	}

	Edges *newNode = new Edges(); // Create new set element that will be added to the LL
	strcpy(newNode->edge[0], u.c_str()); // Set the vertices for the edge of the root
	strcpy(newNode->edge[1], v.c_str());
	newNode->weight = weight;

	if (edgeRoot == NULL) // Insert at root
	{
		edgeRoot = newNode;
		return;
	}
	else if (newNode->weight < edgeRoot->weight)
	{
		// If the root needs to be replaced
		newNode->nextEdge = edgeRoot;
		edgeRoot = newNode;
		return;
	}

	Edges *ptr; // Used to track position while navigating in LL
	ptr = edgeRoot;

	while (true)
	{
		// Keep looking for a nextPtr that is NULL for insertion into LL, 
		// if weight is found, alphabetically check if u and v match up
		// with LL edges at the corresponding weight. If they do, don't insert,
		// and return.

		if (strcmp(ptr->edge[0], u.c_str()) == 0 && strcmp(ptr->edge[1], v.c_str()) == 0) return;

		if (ptr->nextEdge == NULL || ptr->nextEdge->weight > weight)
		{
			newNode->nextEdge = ptr->nextEdge;
			ptr->nextEdge = newNode;
			return;
		}
		ptr = ptr->nextEdge;
	}

}

SpanTree::Set * SpanTree::Find_Set(char word[2])
{
	// Looks through all the sets and determines which set the updated edge in Vertices belongs to.
	// Return a pointer to the set that contains the word.

	Set *ptr = kruskalRoot; // Ptr to start traversal of the set A

	while (ptr != NULL)
	{
		if (ptr->subRoot != NULL)
		{
			// Start another traversal at the subroot if it isn't NULL
			Set *subPtr = ptr->subRoot;
			while (subPtr != NULL)
			{
				if (strcmp(subPtr->setName, word) == 0) return ptr; // If the word is contained in this set
				subPtr = subPtr->nextSet;
			}
		}
		else
		{
			// Else just look at the single item in the set element
			if (strcmp(ptr->setName, word) == 0) return ptr;
		}
		ptr = ptr->nextSet;
	}

}

void SpanTree::Union(Set * u, Set * v)
{
	// Responsible for merging two sets together. It takes set v, and merges it into set u, and combines them
	// into one set u which is put into the old set u's position in the LL, and erases the reference to the old set v. 

	// Take the subroot of the set that contains v, and merge it into the subroot of the set that contains u. If the subroot of the 
	// set that contains v is NULL, then add the entire set node into the subset of u. Afterwards, delete the node that originally contained
	// the set that contained v.

	// Check whether u is less than v, so v is the set that is to be deleted
	if (strcmp(u->setName, v->setName) > 0)
	{
		Set *tempSet = u;
		u = v;
		v = tempSet;
	}

	Set *vParent = kruskalRoot; // Used as a placeholder to delete v from the set

	while (vParent->nextSet != v) vParent = vParent->nextSet;

	if (u->subRoot == NULL)
	{
		// If there isn't a LL created in this node of U, then add V to this linked list, setting V as the root
		if (v->subRoot == NULL)
		{
			Set *subU = new Set(); // Next set reference to add to subset
			Set *subV = new Set();
			strcpy(subU->setName, u->setName);
			strcpy(subV->setName, v->setName);
			u->subRoot = subU; // Set the new root of U to itself to establish a subset
			subU->nextSet = subV;
		}
		else
		{
			Set *subU = new Set(); // Going to be added to subset of V
			Set *subV = new Set();
			strcpy(subU->setName, u->setName);
			subV = v->subRoot;
			u->subRoot = subU;
			subU->nextSet = subV;
		}
	}
	else if (v->subRoot == NULL)
	{
		// If this part was reached, then V didn't have a subroot while U did
		Set *subU = u->subRoot;
		Set *subV = new Set();
		strcpy(subV->setName, v->setName);
		while (subU->nextSet != NULL) subU = subU->nextSet;
		subU->nextSet = subV;
	}
	else
	{
		// This part is reached if both u and v have subroots
		Set * subU = u->subRoot;
		while (subU->nextSet != NULL) subU = subU->nextSet;
		subU->nextSet = v->subRoot;
	}
	vParent->nextSet = v->nextSet;
	v = NULL;
}

SpanTree::HeapVertex SpanTree::Extract_Min()
{
	// Method that extracts the minimum node in the heap. This method will also return a node
	// that contains the information of the node that was deleted. This returned node will be
	// the next node to look through in the Graph.

	// Since this is a priority queue, the min will always reside at node 0.

	HeapVertex remVertex; // Vertex that is going to return the minimum vertex in the heap

	strcpy(remVertex.vertexName, PQ[0]->vertexName);
	strcpy(remVertex.PI, PQ[0]->PI);
	remVertex.key = PQ[0]->key;

	PQ[0]->inHeap = false; // Taken out of the heap

	// Swap these two pointers to get PQ[0] out of the heap
	HeapVertex *tempVertex = PQ[0];
	PQ[0] = PQ[HeapSize - 1];
	PQ[HeapSize - 1] = tempVertex;

	HeapSize--;
	Min_Heapify(0);
	return remVertex;
}

void SpanTree::Min_Heapify(int mapPos)
{
	// Used to readjust the heap
	
	int smallest = 0; // Value to find new position of mapPos node

	// Check whether the value to the left is smaller
	if (2 * mapPos + 1 < HeapSize && PQ[2 * mapPos + 1]->key < PQ[mapPos]->key) smallest = 2 * mapPos + 1;
	else																	     smallest = mapPos;

	// Check whether the value to the right is smaller
	if (2 * mapPos + 2 < HeapSize && PQ[2 * mapPos + 2]->key < PQ[smallest]->key)
		smallest = 2 * mapPos + 2;

	// If a smaller value was found, re-run Min-Heapify on this position
	if (smallest != mapPos)
	{
		HeapVertex *tempVertex = PQ[mapPos];
		PQ[mapPos] = PQ[smallest];
		PQ[smallest] = tempVertex;
		Min_Heapify(smallest);
	}

}

void SpanTree::DecreaseValue(int valPos)
{
	// Used when a value in the Graph is decreased, it puts it in the correct position

	if (PQ[valPos / 2]->key > PQ[valPos]->key)
	{
		HeapVertex *tempVertex = PQ[valPos];
		PQ[valPos] = PQ[valPos / 2];
		PQ[valPos / 2] = tempVertex;
		DecreaseValue(valPos / 2);
	}

}

void SpanTree::PrintKruskal()
{
	// Organizes the vertices into an alphabetical output, and prints them all

	Edges *tempEdge;
	Edges *sizeCheck = edgeRoot; // Used to traverse over the length of the list

	while (sizeCheck != NULL)
	{
		// Keep finding all the minimums in the list until the root is deleted
		Edges *minEdge = edgeRoot;
		tempEdge = edgeRoot;
		while (tempEdge != NULL)
		{
			// While there are still edges left to look at other then the root
			if (tempEdge->shortFlag && tempEdge->edgeChecked && strcmp(tempEdge->edge[0], minEdge->edge[0]) <= 0)
			{
				if (strcmp(tempEdge->edge[0], minEdge->edge[0]) == 0 && strcmp(tempEdge->edge[1], minEdge->edge[1]) < 0)
					minEdge = tempEdge;
				else if (strcmp(tempEdge->edge[0], minEdge->edge[0]) < 0)
					minEdge = tempEdge;
			}
			tempEdge = tempEdge->nextEdge;
		}
		cout << minEdge->edge[0] << "-" << minEdge->edge[1] << ": " << minEdge->weight << endl;
		minEdge->edgeChecked = false;
		while (!sizeCheck->nextEdge->shortFlag) 
		{
			sizeCheck = sizeCheck->nextEdge; 
			if (sizeCheck->nextEdge == NULL) break;
		}
		sizeCheck = sizeCheck->nextEdge;
		if (minEdge == edgeRoot) edgeRoot = minEdge->nextEdge;
	}

}

void SpanTree::PrintPrim()
{
	// Organizes all the vertices into an alphabetical output, and prints them all

	for (int i = 0; i < N - 1; i++)
	{
		if (strcmp(PQ[i]->vertexName, PQ[i]->PI) < 0)
		{
			char tempChar[2];
			strcpy(tempChar, PQ[i]->vertexName);
			strcpy(PQ[i]->vertexName, PQ[i]->PI);
			strcpy(PQ[i]->PI, tempChar);
		}
	}

	for (int i = 0; i < N - 1; i++)
	{
		HeapVertex *tempVertex = PQ[0]; // Used to traverse the array 
		HeapVertex *minVertex;
		for (int j = 0; j < N - 1; j++) // Set the min to a node that is still in the edges to be checked
		{
			if (PQ[j]->alreadyChecked)
				minVertex = PQ[j];
		}
		for (int j = 0; j < N - 1; j++)
		{
			tempVertex = PQ[j];
			if (strcmp(minVertex->PI, tempVertex->PI) >= 0 &&  tempVertex->alreadyChecked)
			{
				if (strcmp(minVertex->PI, tempVertex->PI) == 0 && strcmp(minVertex->vertexName, tempVertex->vertexName) > 0)
					minVertex = tempVertex;
				else if (strcmp(minVertex->PI, tempVertex->PI) > 0)
					minVertex = tempVertex;
			}
		}
		cout << minVertex->PI << "-" << minVertex->vertexName << ": " << minVertex->key << endl;
		minVertex->alreadyChecked = false;
	}

}
