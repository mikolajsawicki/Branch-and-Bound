#include "Graph.h"
#include <vector>

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
	std::vector<int> path;
	path.push_back(start_node);

	std::vector<int> best_path;
	int best_path_weight = -1;

	// node_visited[i] contains true, if the i node has been visited, or false if it hasn't
	bool* node_visited = new bool[nodesCount];

	for(int i = 0; i < nodesCount; i++)
		node_visited[i] = false;

	node_visited[start_node] = true;
	//
	
	checkAllPaths(path, node_visited, best_path, &best_path_weight);

	delete[] node_visited;

	return best_path_weight;
}

int Graph::getPathWeight(std::vector<int> path)
{
	int weight = 0;

	for (int i = 0; i < path.size() - 1; i++)
	{
		weight += adjacencyMatrix[path[i]][path[i + 1]];
	}

	return weight;
}

// Let's consider the tree, which contains all possible paths in our graph.
// This function is based on the recursive deep search of such a tree.
// The path vector must contain the start node number.
void Graph::checkAllPaths(std::vector<int> path, bool* node_visited, std::vector<int> best_path, int *best_path_weight)
{
	if (path.size() < nodesCount)
	{
		for (int i = 0; i < nodesCount; i++)
		{
			if (!node_visited[i])
			{
				path.push_back(i);
				node_visited[i] = true;

				checkAllPaths(path, node_visited, best_path, best_path_weight);

				node_visited[i] = false;
				path.pop_back();
			}
		}
	}
	else if (adjacencyMatrix[path.back()][path.front()] > 0) {
		path.push_back(path.front());
		
		int path_weight = getPathWeight(path);

		if (*best_path_weight == -1 || path_weight < *best_path_weight)
		{
			best_path = path;
			*best_path_weight = path_weight;
		}

		path.pop_back();

		return;
	}
}