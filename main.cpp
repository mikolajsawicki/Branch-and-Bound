#include "Graph.h"
#include <fstream>
#include <iostream>
#include <random>
#include "TSPSolver.h"

void print_menu()
{
	printf(	"----- Menu -----\n"
			"0. Quit.\n"
			"1. Load data from file.\n"
			"2. Generate random graph.\n"
			"3. Display the recently loaded/generated data.\n"
			"4. Find TSP solution using the brute force algorithm.\n"
			"5. Find TSP solution using the branch and bound algorithm.\n");
}

bool file_load(Graph* graph)
{
	std::string file_name;
	std::fstream file;

	std::cout << "Enter the file name: ";
	std::cin >> file_name;

	file.open(file_name);

	if (!file.good())
		return false;

	int nodes_count;
	file >> nodes_count;

	if (nodes_count <= 0)
		return false;

	graph->reset(nodes_count);

	SquareMatrix* matrix = graph->getAdjacencyMatrix();

	for (int i = 0; i < nodes_count; i++)
		for (int j = 0; j < nodes_count; j++)
			file >> (*matrix)[i][j];

	return true;
}

void generate_random_graph(Graph* graph, int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);

	SquareMatrix* matrix = graph->getAdjacencyMatrix();

	for (int i = 0; i < graph->getNodesCount(); i++)
		for (int j = 0; j < graph->getNodesCount(); j++)
			(*matrix)[i][j] = dist(gen);
}

int start_node_prompt()
{
	int start_node;
	printf("Enter the start (and end) node: ");
	std::cin >> start_node;

	return start_node;
}

int main()
{	
	Graph graph;
	int opt;

	do
	{
		print_menu();

		std::cin >> opt;
	
		switch (opt)
		{
		case 1:
			if (file_load(&graph))
				printf("The data has been loaded.\n");
			else
				printf("Error: Could not open the file.\n");

			break;

		case 2:
			int size, min, max;
			printf("Set the graph nodes count: ");
			std::cin >> size;

			graph.reset(size);

			printf("Set the minimum edge's weight: ");
			std::cin >> min;

			printf("Set the maximum edge's weight: ");
			std::cin >> max;

			generate_random_graph(&graph, min, max);
			printf("The random graph has been generated.\n");
			break;

		case 3:
			if (graph.empty()) {
				printf("Error: There's no graph to display.\n");
				break;
			}
			
			std::cout << graph.toString();
			break;

		case 4:
		{
			TSPSolver solver(&graph, start_node_prompt());
			solver.solveBruteForce();
			printf("Best path weight: %d", solver.getBestPathWeight());

			break;
		}

		case 5:
		{
			TSPSolver solver(&graph, start_node_prompt());
			solver.solveBranchAndBound();
			printf("Best path weight: %d", solver.getBestPathWeight());

			break;
		}
		}
	} while (opt != 0);
}