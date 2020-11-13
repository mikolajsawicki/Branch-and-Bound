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

int Graph::getWeight(int from, int to)
{
	return adjacencyMatrix[from][to];
}