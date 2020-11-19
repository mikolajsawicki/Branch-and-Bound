#include "Graph.h"
#include <fstream>
#include <iostream>
#include <random>
#include "TSPSolver.h"
#include "chrono"
#include <algorithm>

#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX 9223372036854775807;
#endif


using namespace std::chrono;

void print_menu()
{
	printf(	"----- Menu -----\n"
			"0. Quit.\n"
			"1. Load data from file.\n"
			"2. Generate random graph.\n"
			"3. Display the recently loaded/generated data.\n"
			"4. Find TSP solution using the brute force algorithm (iterate trough permutations).\n"
			"5. Find TSP solution using the brute force algorithm (recursive tree search).\n"
			"6. Find TSP solution using the branch and bound algorithm.\n"
			"7. Run permutation brute force performance test.\n"
			"8. Run tree search brute force performance test.\n"
			"9. Run branch and bound performance test.\n");
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

void performance_test(int opt)
{
	int test_val_min = 10, test_val_max = 10;
	int test_repetitions = 100;

	high_resolution_clock::time_point start;
	high_resolution_clock::time_point stop;

	int* test_n_values = new int[7];

	if (opt == 9)
	{
		int n_values[7] = { 4, 7, 9, 11, 18, 22, 24 };
		std::copy(n_values, n_values + 7, test_n_values);
	}
		
	else
	{
		int n_values[7] = { 4, 6, 7, 8, 9, 10, 11 };
		std::copy(n_values, n_values + 7, test_n_values);
	}
		

	for (int n_index = 0; n_index < 7; n_index++)
	{
		long long min_time = INT_MAX;
		long long max_time = 0;
		long long avg_time = 0;

		for (int i = 1; i <= test_repetitions; i++)
		{
			Graph graph(test_n_values[n_index]);
			generate_random_graph(&graph, test_val_min, test_val_max);

			TSPSolver solver(&graph, 0);

			switch (opt)
			{
			case 7:
				start = high_resolution_clock::now();
				solver.solveBruteForce();
				stop = high_resolution_clock::now();
				break;
			case 8:
				start = high_resolution_clock::now();
				solver.solveBruteForceSearchTree();
				stop = high_resolution_clock::now();
			case 9:
				start = high_resolution_clock::now();
				solver.solveBranchAndBound();
				stop = high_resolution_clock::now();
			}
			

			long long time = duration_cast<nanoseconds>(stop - start).count();

			avg_time += time;

			if (time < min_time)
				min_time = time;
			if (time > max_time)
				max_time = time;
		}

		avg_time /= test_repetitions;

		printf("Dla n = %d: \n min time: %lldns \n max time: %lldns \n average time: %lldns\n", test_n_values[n_index], min_time, max_time, avg_time);
	}
}

void print_path(int* path, int n)
{
	for (int i = 0; i < n - 1; i++)
		printf("%d  ->  ", path[i]);

	printf("%d", path[n - 1]);
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
			int n = start_node_prompt();
			TSPSolver solver(&graph, n);
			solver.solveBruteForce();
			int* best_path = solver.getBestPath();
			printf("Best path weight: %d\n", solver.getBestPathWeight());
			print_path(best_path, graph.getNodesCount());

			break;
		}
		case 5:
		{
			int n = start_node_prompt();
			TSPSolver solver(&graph, n);
			solver.solveBruteForceSearchTree();
			int* best_path = solver.getBestPath();
			printf("Best path weight: %d\n", solver.getBestPathWeight());
			print_path(best_path, graph.getNodesCount());

			break;
		}
		case 6:
		{
			int n = start_node_prompt();
			TSPSolver solver(&graph, n);
			solver.solveBranchAndBound();
			int* best_path = solver.getBestPath();
			printf("Best path weight: %d\n", solver.getBestPathWeight());
			print_path(best_path, graph.getNodesCount());

			break;
		}
		case 7:
		{
			performance_test(opt);

			break;
		}
		case 8:
		{
			performance_test(opt);

			break;
		}
		case 9:
		{
			performance_test(opt);

			break;
		}
		default:
			break;
		}
	} while (opt != 0);
}