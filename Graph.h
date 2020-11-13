#pragma once
#include <string>
#include <vector>
class Graph
{
public:
	Graph(int nodes_count);
	Graph();
	~Graph();
	int** getAdjacencyMatrix();
	bool reset(int new_nodes_count);
	int getNodesCount();
	std::string toString();
	bool empty();
	int bruteForceTSP(int start_node);
	int branchAndBoundTSP(int start_node);

private:
	int** adjacencyMatrix;
	int nodesCount;

	void deleteAdjacencyMatrix();
	void createAdjacencyMatrix();
};

