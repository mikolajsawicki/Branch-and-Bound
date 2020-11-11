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
	void checkAllPaths(std::vector<int> path, bool* node_visited, std::vector<int> best_path, int* best_path_weight);
	int getPathWeight(std::vector<int> path);

private:
	int** adjacencyMatrix;
	int nodesCount;

	void deleteAdjacencyMatrix();
	void createAdjacencyMatrix();
};

