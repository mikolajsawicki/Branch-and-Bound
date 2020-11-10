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
	void checkAllRoads(std::vector<int> road, bool* node_visited, std::vector<int> best_road, int* best_road_weight);
	int getRoadWeight(std::vector<int> road);

private:
	int** adjacencyMatrix;
	int nodesCount;

	void deleteAdjacencyMatrix();
	void createAdjacencyMatrix();
};

