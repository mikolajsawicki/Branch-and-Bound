#include "PathNode.h"
#include <algorithm>

// Constructor for first node in path
PathNode::PathNode(SquareMatrix* matrix, int index_in_matrix)
{
	indexInPath = 0;
	indexInMatrix = index_in_matrix;
	this->matrix = new SquareMatrix(*matrix);
	pathWeight = 0;

	calculateBound();
}


// Constructor for next node after "node"
PathNode::PathNode(PathNode& node, int index_in_matrix)
{
	indexInPath = node.indexInPath + 1;
	indexInMatrix = index_in_matrix;
	matrix = new SquareMatrix(node.getMatrix());
	pathWeight = node.pathWeight + (*matrix)[node.indexInMatrix][indexInMatrix];

	calculateBound();
}

PathNode::~PathNode()
{
	delete matrix;
}

SquareMatrix PathNode::getMatrix()
{
	return *matrix;
}

void PathNode::calculateBound()
{

}