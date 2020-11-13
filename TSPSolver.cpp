#include "TSPSolver.h"
#include "algorithm"

TSPSolver::TSPSolver(Graph* graph, int node)
{
	this->graph = graph;
	this->startNode = node;
	bestPath = new int[graph->getNodesCount() + 1];
	bestPathWeight = -1;

	consideredPathInit();
}

TSPSolver::~TSPSolver()
{
	delete[] bestPath;
	delete[] consideredPath;
}

void TSPSolver::consideredPathInit()
{
	int nodes_count = this->graph->getNodesCount();

	int* consideredPath = new int[nodes_count + 1];

	consideredPath[0] = startNode;
	consideredPath[nodes_count] = startNode;

	for (int val = 0, i = 1; i < nodes_count; val++)
	{
		if (val != startNode)
		{
			consideredPath[i] = val;
			i++;
		}
	}
}

int TSPSolver::getBestPathWeight()
{
	return bestPathWeight;
}

void TSPSolver::solveBruteForce()
{
	int weight;
	int nodes_count = this->graph->getNodesCount();

	do
	{
		weight = 0;

		for (int i = 0; i < nodes_count; i++)
		{
			weight += graph->getWeight(i, i+1);
		}

		if (bestPathWeight == -1 || weight < bestPathWeight)
		{
			std::copy(consideredPath, consideredPath + nodes_count + 1, bestPath);
			bestPathWeight = weight;
		}
	} while (std::next_permutation(consideredPath + 1, consideredPath + nodes_count));
}