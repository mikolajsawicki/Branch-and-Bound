#pragma once
#include "Graph.h"

class TSPSolver
{
private:
	Graph* graph;
	int startNode;
	
	int* bestPath;
	int bestPathWeight;

	bool* visited;

	void setFirstPermutation(int* considered_path);
	int startBound(int* min_1, int* min_2);
	void branchAndBoundRecursive(int level, int* path, int weight, int bound, int* min_1, int* min_2);
	void fillMin(int* min_1, int* min_2);

public:
	TSPSolver(Graph* graph, int node);
	~TSPSolver();
	void solveBruteForce();
	void solveBruteForceSearchTree();
	void solveBruteForceSearchTreeRecursive(std::vector<int> path);
	void solveBranchAndBound();
	int getBestPathWeight();
	int* getBestPath();
};

