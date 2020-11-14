#pragma once
#include "Graph.h"

struct PathNode
{
	int index;
	int indexInMatrix;
	bool visited;
	int** reducedMatrix;
};

class TSPSolver
{
private:
	Graph* graph;
	int startNode;
	
	int* bestPath;
	int bestPathWeight;

	void setFirstPermutation(int* considered_path);

public:
	TSPSolver(Graph* graph, int node);
	~TSPSolver();
	void solveBruteForce();
	void solveBranchAndBound();
	int getBestPathWeight();
};

