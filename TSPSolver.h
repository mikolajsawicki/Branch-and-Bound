#pragma once
#include "Graph.h"

class TSPSolver
{
private:
	Graph* graph;
	int startNode;
	int* consideredPath;
	int* bestPath;
	int bestPathWeight;

	void consideredPathInit();

public:
	TSPSolver(Graph* graph, int node);
	~TSPSolver();
	void solveBruteForce();
	int getBestPathWeight();
};

