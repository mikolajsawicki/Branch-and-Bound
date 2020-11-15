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
	int** orders;

	void setFirstPermutation(int* considered_path);
	static int partition(int* arr, int* order, int p, int q);
	static void quickSort(int* arr, int* order, int p, int q);
	void getAscendingOrders();
	int startBound(int** orders);
	void branchAndBoundRecursive(int level, int* path, int weight, int bound);
	int firstMin(int i);
	int secondMin(int i);

public:
	TSPSolver(Graph* graph, int node);
	~TSPSolver();
	void solveBruteForce();
	void solveBranchAndBound();
	int getBestPathWeight();
};

