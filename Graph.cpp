#include "Graph.h"
#include <vector>
#include <algorithm>

Graph::Graph(int nodes_count)
{
	if (nodes_count < 0)
		throw "Error: Nodes count cannot be less than 0";

	nodesCount = nodes_count;

	createAdjacencyMatrix();
}

Graph::Graph()
{
	nodesCount = 0;
	createAdjacencyMatrix();
}

Graph::~Graph()
{
	deleteAdjacencyMatrix();
}

bool Graph::reset(int new_nodes_count)
{
	deleteAdjacencyMatrix();

	nodesCount = new_nodes_count;

	createAdjacencyMatrix();

	return true;
}

int Graph::getNodesCount()
{
	return nodesCount;
}

int** Graph::getAdjacencyMatrix()
{
	return adjacencyMatrix;
}

void Graph::deleteAdjacencyMatrix()
{
	if (nodesCount > 0)
	{
		for (int i = 0; i < nodesCount; i++)
			delete[] adjacencyMatrix[i];
	}

	delete[] adjacencyMatrix;
}

void Graph::createAdjacencyMatrix()
{
	adjacencyMatrix = new int* [nodesCount];

	for (int i = 0; i < nodesCount; i++)
		adjacencyMatrix[i] = new int[nodesCount];
}

bool Graph::empty()
{
	if (nodesCount <= 0)
		return true;

	if (!adjacencyMatrix)
		return true;

	return false;
}

std::string Graph::toString()
{
	std::string result = "";

	result += "Nodes count: " + std::to_string(nodesCount) + "\n";
	
	for (int i = 0; i < nodesCount; i++)
	{
		for (int j = 0; j < nodesCount; j++)
			result += std::to_string(adjacencyMatrix[i][j]) + " ";

		result += "\n";
	}

	return result;
}

int Graph::bruteForceTSP(int start_node)
{
	int* path = new int[nodesCount + 1];

	path[0] = start_node;
	path[nodesCount] = start_node;

	for (int val = 0, i = 1; i < nodesCount; val++)
	{
		if (val != start_node)
		{
			path[i] = val;
			i++;
		}
	}

	int* best_path = new int[nodesCount + 1];
	int best_path_weight = -1;

	int weight;

	do
	{
		weight = 0;

		for (int i = 0; i < nodesCount; i++)
		{
			weight += adjacencyMatrix[path[i]][path[i + 1]];
		}

		if (best_path_weight == -1 || weight < best_path_weight)
		{
			std::copy(path, path + nodesCount + 1, best_path);
			best_path_weight = weight;
		}
	} while (std::next_permutation(path + 1, path + nodesCount));

	delete[] path;
	delete[] best_path;

	return best_path_weight;
}

/*
int Graph::branchAndBoundTSP(int start_node)
{
	int* path = new int[nodesCount + 1];

	path[0] = start_node;
	path[nodesCount] = start_node;

	for (int val = 0, i = 1; i < nodesCount; val++)
	{
		if (val != start_node)
		{
			path[i] = val;
			i++;
		}
	}

	int* best_path = new int[nodesCount + 1];
	int best_path_weight = -1;

	int weight;

	do
	{
		weight = 0;

		for (int i = 0; i < nodesCount; i++)
		{
			weight += adjacencyMatrix[path[i]][path[i + 1]];
		}

		if (best_path_weight == -1 || weight < best_path_weight)
		{
			std::copy(path, path + nodesCount + 1, best_path);
			best_path_weight = weight;
		}
	} while (std::next_permutation(path + 1, path + nodesCount));

	return best_path_weight;
}
*/
