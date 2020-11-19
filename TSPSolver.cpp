#include "TSPSolver.h"
#include "algorithm"
#include "SquareMatrix.h"

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

TSPSolver::TSPSolver(Graph* graph, int node)
{
	int nodes_count = graph->getNodesCount();

	this->graph = graph;
	startNode = node;
	bestPath = new int[nodes_count + 1];
	bestPathWeight = INT_MAX;

	visited = new bool[nodes_count + 1];
	memset(visited, false, (nodes_count + 1) * sizeof(bool));
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

	bestPathWeight = INT_MAX;

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

int TSPSolver::startBound(int* min_1, int* min_2)
{
	int bound = 0;
	
	for (int i = 0; i < graph->getNodesCount(); i++)
	{
		bound += min_1[i] + min_2[i];
	}

	if (bound % 2 == 1)
		bound++;
	bound /= 2;

	return bound;
}


void TSPSolver::branchAndBoundRecursive(int level, int* path, int weight, int bound, int* min_1, int* min_2)
{
	SquareMatrix* matrix = graph->getAdjacencyMatrix();

	if (level == graph->getNodesCount())
	{
		int path_weight = weight + (*matrix)[path[level - 1]][path[0]];

		if (path_weight < bestPathWeight)
		{
			bestPathWeight = path_weight;
			std::copy(path, path + graph->getNodesCount() + 1, bestPath);
		}
		return;
	}

	for (int i = 0; i < graph->getNodesCount(); i++)
	{
		if ((*matrix)[path[level - 1]][i] > 0 && visited[i] == false)
		{
			int old_bound = bound;

			weight += (*matrix)[path[level - 1]][i];

			if (level == 1)
				bound -= ((min_1[path[level - 1]] + min_1[i]) / 2);
			else
				bound -= ((min_2[path[level - 1]] + min_1[i]) / 2);

			if (bound + weight < bestPathWeight)
			{
				path[level] = i;
				visited[i] = true;

				branchAndBoundRecursive(level + 1, path, weight, bound, min_1, min_2);
			}

			weight -= (*matrix)[path[level - 1]][i];
			bound = old_bound;

			memset(visited, false, sizeof(visited));

			for (int i = 0; i < graph->getNodesCount(); i++)
				visited[i] = false;

			for (int j = 0; j <= level - 1; j++)
				visited[path[j]] = true;
		}
	}
}

void TSPSolver::fillMin(int* min_1, int* min_2)
{
	SquareMatrix* matrix = graph->getAdjacencyMatrix();

	for (int node = 0; node < graph->getNodesCount(); node++)
	{
		min_1[node] = INT_MAX, min_2[node] = INT_MAX;
		
		for (int j = 0; j < graph->getNodesCount(); j++)
		{
			if (node == j)
				continue;

			if ((*matrix)[node][j] <= min_1[node])
			{
				min_2[node] = min_1[node];
				min_1[node] = (*matrix)[node][j];
			}
			else if ((*matrix)[node][j] <= min_2[node] &&
				(*matrix)[node][j] != min_1[node])
				min_2[node] = (*matrix)[node][j];
		}
	}
}

void TSPSolver::solveBranchAndBound()
{
	SquareMatrix* matrix = graph->getAdjacencyMatrix();
	int nodes_count = matrix->getSize();

	int* path = new int[nodes_count + 1];
	memset(path, -1, sizeof(int) * (nodes_count + 1));

	bestPathWeight = INT_MAX;

	memset(visited, false, (matrix->getSize() + 1) * sizeof(bool));

	int* min_1 = new int[nodes_count];
	int* min_2 = new int[nodes_count];

	fillMin(min_1, min_2);

	int bound = startBound(min_1, min_2);

	path[0] = startNode;
	visited[startNode] = true;

	branchAndBoundRecursive(1, path, 0, bound, min_1, min_2);
}


void TSPSolver::solveBruteForceSearchTree()
{
	std::vector<int> path;
	path.push_back(startNode);

	bestPathWeight = INT_MAX;

	memset(visited, false, (graph->getNodesCount() + 1) * sizeof(bool));

	visited[startNode] = true;
	//

	solveBruteForceSearchTreeRecursive(path);
}

// Let's consider the tree, which contains all possible paths in our graph.
// This function is based on the recursive deep search of such a tree.
// The path vector must contain the start node number.
void TSPSolver::solveBruteForceSearchTreeRecursive(std::vector<int> path)
{
	SquareMatrix* matrix = graph->getAdjacencyMatrix();

	if (path.size() < graph->getNodesCount())
	{
		for (int i = 0; i < graph->getNodesCount(); i++)
		{
			if (!visited[i])
			{
				path.push_back(i);
				visited[i] = true;

				solveBruteForceSearchTreeRecursive(path);

				visited[i] = false;
				path.pop_back();
			}
		}
	}
	else if ((*matrix)[path.back()][path.front()] > 0) {
		path.push_back(path.front());

		int path_weight = 0;

		for (int i = 0; i < path.size() - 1; i++)
		{
			path_weight += (*matrix)[path[i]][path[i + 1]];
		}

		if (bestPathWeight == INT_MAX || path_weight < bestPathWeight)
		{
			for (int i = 0; i < matrix->getSize(); i++)
				bestPath[i] = path[i];

			bestPathWeight = path_weight;
		}

		path.pop_back();

		return;
	}
}

int* TSPSolver::getBestPath()
{
	return bestPath;
}