#include "TSPSolver.h"
#include "algorithm"
#include "SquareMatrix.h"
#include "PathNode.h"

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

	orders = new int* [nodes_count];
	for (int i = 0; i < nodes_count; i++)
		orders[i] = new int[nodes_count];

	getAscendingOrders();
}

TSPSolver::~TSPSolver()
{
	delete[] bestPath;
	
	for (int i = 0; i < graph->getNodesCount(); i++)
		delete[] orders[i];

	delete[] orders;
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


int TSPSolver::partition(int* arr, int* order, int p, int q)
{
	int i = (p - 1);
	int pivot = arr[q];

	for (int j = p; j <= q - 1; j++)
	{
		if (arr[j] < pivot)
		{
			i++;
			std::swap(arr[i], arr[j]);
			std::swap(order[i], order[j]);
		}
	}
	std::swap(arr[i + 1], arr[q]);
	std::swap(order[i + 1], order[q]);

	return (i + 1);
}


void TSPSolver::quickSort(int* arr, int* order, int p, int q)
{
	if (p < q)
	{
		int index = partition(arr, order, p, q);

		quickSort(arr, order, 0, index - 1);
		quickSort(arr, order, index + 1, q);
	}
}

// Sorts each row in matrix and creates an array including ordered indexes of elements sorted by value.
// So if after execution we have order[i] = [3, 0, 2, 1], then matrix[i][3] is the smallest, matrix[i][0] is the second smallest, etc.
void TSPSolver::getAscendingOrders()
{
	SquareMatrix tmp_matrix = SquareMatrix(*graph->getAdjacencyMatrix());

	for (int i = 0; i < tmp_matrix.getSize(); i++)
		for (int j = 0; j < tmp_matrix.getSize(); j++)
			orders[i][j] = j;

	for (int i = 0; i < tmp_matrix.getSize(); i++)
	{
		quickSort(tmp_matrix[i], orders[i], 0, tmp_matrix.getSize() - 1);
	}
}

int TSPSolver::startBound(int** orders)
{
	SquareMatrix matrix = *graph->getAdjacencyMatrix();

	int bound = 0;
	
	for (int i = 0; i < matrix.getSize(); i++)
	{
		int* row = matrix[i];

		bound += row[orders[i][1]] + row[orders[i][2]];
	}

	if (bound % 2 == 1)
		bound++;
	bound /= 2;

	return bound;
}

int TSPSolver::firstMin(int i)
{
	SquareMatrix* matrix = graph->getAdjacencyMatrix();

	int min = INT_MAX;
	for (int k = 0; k < graph->getNodesCount(); k++)
		if ((*matrix)[i][k] < min && i != k)
			min = (*matrix)[i][k];
	return min;
}

int TSPSolver::secondMin(int i)
{
	int first = INT_MAX, second = INT_MAX;
	SquareMatrix* matrix = graph->getAdjacencyMatrix();
	for (int j = 0; j < graph->getNodesCount(); j++)
	{
		if (i == j)
			continue;

		if ((*matrix)[i][j] <= first)
		{
			second = first;
			first = (*matrix)[i][j];
		}
		else if ((*matrix)[i][j] <= second &&
			(*matrix)[i][j] != first)
			second = (*matrix)[i][j];
	}
	return second;
}

void TSPSolver::branchAndBoundRecursive(int level, int* path, int weight, int bound)
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


	// for any other level iterate for all vertices to 
	// build the search space tree recursively 
	for (int i = 0; i < graph->getNodesCount(); i++)
	{
		// Consider next vertex if it is not same (diagonal 
		// entry in adjacency matrix and not visited 
		// already) 
		if ((*matrix)[path[level - 1]][i] > 0 && visited[i] == false)
		{
			int old_bound = bound;

			weight += (*matrix)[path[level - 1]][i];

			if (level == 1)
				bound -= ((firstMin(path[level - 1]) + firstMin(i)) / 2);
			else
				bound -= ((secondMin(path[level - 1]) + firstMin(i)) / 2);

			if (bound + weight < bestPathWeight)
			{
				path[level] = i;
				visited[i] = true;

				branchAndBoundRecursive(level + 1, path, weight, bound);
			}

			weight -= (*matrix)[path[level - 1]][i];
			bound = old_bound;

			memset(visited, false, sizeof(visited));

			for (int j = 0; j <= level - 1; j++)
				visited[path[j]] = true;
		}
	}
}

void TSPSolver::solveBranchAndBound()
{
	SquareMatrix* matrix = graph->getAdjacencyMatrix();
	int nodes_count = matrix->getSize();

	int* path = new int[nodes_count + 1];
	memset(path, -1, sizeof(int) * (nodes_count + 1));

	int bound = startBound(orders);

	path[0] = 0;
	visited[0] = true;

	branchAndBoundRecursive(1, path, 0, bound);
}