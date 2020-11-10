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
	std::vector<int> road;
	road.push_back(start_node);

	std::vector<int> best_road;
	int best_road_weight = -1;

	// node_visited[i] contains true, if the i node has been visited, or false if it hasn't
	bool* node_visited = new bool[nodesCount];

	for(int i = 0; i < nodesCount; i++)
		node_visited[i] = false;

	node_visited[start_node] = true;
	//
	
	checkAllRoads(road, node_visited, best_road, &best_road_weight);

	delete[] node_visited;

	return best_road_weight;
}

int Graph::getRoadWeight(std::vector<int> road)
{
	int weight = 0;

	for (int i = 0; i < road.size() - 1; i++)
	{
		weight += adjacencyMatrix[road[i]][road[i + 1]];
	}

	return weight;
}

// Let's consider the tree, which contains all possible paths in our graph.
// This function is based on the recursive deep search of such a tree.
// The road vector must contain the start node number.
void Graph::checkAllRoads(std::vector<int> road, bool* node_visited, std::vector<int> best_road, int *best_road_weight)
{
	if (road.size() < nodesCount)
	{
		for (int i = 0; i < nodesCount; i++)
		{
			if (!node_visited[i])
			{
				road.push_back(i);
				node_visited[i] = true;

				checkAllRoads(road, node_visited, best_road, best_road_weight);

				node_visited[i] = false;
				road.pop_back();
			}
		}
	}
	else if (adjacencyMatrix[road.back()][road.front()] > 0) {
		road.push_back(road.front());
		
		int road_weight = getRoadWeight(road);

		if (*best_road_weight == -1 || road_weight < *best_road_weight)
		{
			best_road = road;
			*best_road_weight = road_weight;
		}

		road.pop_back();

		return;
	}
}