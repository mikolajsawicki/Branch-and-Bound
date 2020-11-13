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

void Graph::createAdjacencyMatrix()
{
	adjacencyMatrix = new int* [nodesCount];

	for (int i = 0; i < nodesCount; i++)
		adjacencyMatrix[i] = new int[nodesCount];
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

void Graph::setLightestEdges(int* lightest_edges)
{
	for (int i = 0; i < nodesCount; i++)
	{
		lightest_edges[i] = adjacencyMatrix[i][0];

		for (int j = 0; j < nodesCount; j++)
		{
			if (adjacencyMatrix[i][j] > 0 && adjacencyMatrix[i][j] < lightest_edges[i])
			{
				lightest_edges[i] = j;
			}
		}
	}
}

void Graph::updateLightestAvailableEdges(int* lightest_available_edges, int node, int* path, bool* node_available_as_destination)
{
	for (int i = node; i < nodesCount; i++)
	{
		// If path[i] node is present in lightest edges of any remaining node, find a new lightest edge for it.
		if (lightest_available_edges[path[i]] == node)
		{
			int min;
			if (node != 0)
				min = adjacencyMatrix[i][0];
			else
				min = adjacencyMatrix[i][1];

			for (int k = 0; k < nodesCount; k++)
			{
				if (adjacencyMatrix[i][k] < min && node_available_as_destination[k] && adjacencyMatrix[i][k] > 0)
					min = adjacencyMatrix[i][k];
			}

			lightest_available_edges[path[i]] = min;
		}
	}
}

int Graph::getLowBound(int* lightest_available_edges, int node)
{
	int sum = 0;

	for (int i = node; i < nodesCount; i++)
		sum += adjacencyMatrix[i][lightest_available_edges[i]];

	return sum;
}

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

	bool* node_available_as_destination = new bool[nodesCount];
	// There is the minimum available (not used before) outside edge's weight for each node at the moment.
	int* lightest_edges = new int[nodesCount];
	setLightestEdges(lightest_edges);

	int* lightest_available_edges = new int[nodesCount];

	int low_bound;
	
	bool bound_exceeded;
	int weight_sum;

	do
	{
		for (int i = 0; i < nodesCount; i++)
			node_available_as_destination[i] = true;

		weight_sum = 0;
		bound_exceeded = false;

		std::copy(lightest_edges, lightest_edges + nodesCount, lightest_available_edges);

		for (int i = 1; i < nodesCount; i++)
		{
			weight_sum += adjacencyMatrix[path[i - 1]][path[i]];
			node_available_as_destination[i] = false;

			updateLightestAvailableEdges(lightest_available_edges, i, path, node_available_as_destination);

			low_bound = getLowBound(lightest_available_edges, i);

			if (low_bound + weight_sum >= best_path_weight && best_path_weight != -1)
			{
				bound_exceeded = true;
				break;
			}
		}

		if (bound_exceeded)
			continue;

		weight_sum += adjacencyMatrix[path[nodesCount - 1]][path[nodesCount]];

		if (weight_sum < best_path_weight || best_path_weight == -1)
		{
			std::copy(path, path + nodesCount + 1, best_path);
			best_path_weight = weight_sum;
		}
	} while (std::next_permutation(path + 1, path + nodesCount));

	return best_path_weight;
}

