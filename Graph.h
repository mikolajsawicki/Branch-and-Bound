#pragma once
#include <string>
#include <vector>

class Graph
{
private:
	int** adjacencyMatrix;
	int nodesCount;
	
	void createAdjacencyMatrix();
	void deleteAdjacencyMatrix();

public:
	Graph(int nodes_count);
	Graph();
	~Graph();
	int** getAdjacencyMatrix();
	int getWeight(int from, int to);
	bool reset(int new_nodes_count);
	int getNodesCount();
	std::string toString();
	bool empty();
	void setLightestEdges(int* lightest_edges);
	void updateLightestAvailableEdges(int* lightest_available_edges, int i, int* path, bool* node_available_as_destination);
	int getLowBound(int* lightest_edges, int node);
	int branchAndBoundTSP(int start_node);
};

