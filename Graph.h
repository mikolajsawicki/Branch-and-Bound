#pragma once
#include <string>
#include <vector>
#include "SquareMatrix.h"

class Graph
{
private:
	SquareMatrix* adjacencyMatrix;
	int nodesCount;

public:
	Graph(int nodes_count);
	Graph();
	~Graph();
	SquareMatrix* getAdjacencyMatrix();
	int getWeight(int from, int to);
	bool reset(int new_nodes_count);
	int getNodesCount();
	std::string toString();
	bool empty();
};

