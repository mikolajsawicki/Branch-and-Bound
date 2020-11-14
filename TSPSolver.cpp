#include "TSPSolver.h"
#include "algorithm"

TSPSolver::TSPSolver(Graph* graph, int node)
{
	this->graph = graph;
	this->startNode = node;
	bestPath = new int[graph->getNodesCount() + 1];
	bestPathWeight = -1;
}

TSPSolver::~TSPSolver()
{
	delete[] bestPath;
}

int TSPSolver::getBestPathWeight()
{
	return bestPathWeight;
}

void TSPSolver::setFirstPermutation(int* path)
{
	int nodes_count = this->graph->getNodesCount();

	path[0] = startNode;
	path[nodes_count] = startNode;

	for (int val = 0, i = 1; i < nodes_count; val++)
	{
		if (val != startNode)
		{
			path[i] = val;
			i++;
		}
	}
}

void TSPSolver::solveBruteForce()
{
	int weight;
	int nodes_count = this->graph->getNodesCount();

	int* path = new int[nodes_count + 1];
	setFirstPermutation(path);

	do
	{
		weight = 0;

		for (int i = 0; i < nodes_count; i++)
		{
			weight += graph->getWeight(path[i], path[i+1]);
		}

		if (bestPathWeight == -1 || weight < bestPathWeight)
		{
			std::copy(path, path + nodes_count + 1, bestPath);
			bestPathWeight = weight;
		}
	} while (std::next_permutation(path + 1, path + nodes_count));

	delete[] path;
}

void TSPSolver::solveBranchAndBound()
{
	std::vector<PathNode> path;

	PathNode first;

}