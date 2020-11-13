#pragma once
#include <string>
#include <vector>

class Graph
{
private:
	int** adjacencyMatrix;
	int nodesCount;
	GraphNode* nodes;
	
	void createAdjacencyMatrix();
	void deleteAdjacencyMatrix();

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
	void setLightestEdges(int* lightest_edges);
	void updateLightestAvailableEdges(int* lightest_available_edges, int i, int* path, bool* node_available_as_destination);
	int getLowBound(int* lightest_edges, int node);
	int branchAndBoundTSP(int start_node);
};

