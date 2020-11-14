#include "PathNode.h"
#include <algorithm>

// Constructor for first node in path
PathNode::PathNode(SquareMatrix& matrix, int index_in_matrix)
{
	indexInPath = 0;
	indexInMatrix = index_in_matrix;
	this->matrix = new SquareMatrix(matrix);
}

// Constructor for next node after "node"
PathNode::PathNode(PathNode &node, int index_in_matrix)
{
	indexInPath = node.indexInPath + 1;
	indexInMatrix = index_in_matrix;
	this->matrix = new SquareMatrix(node.getMatrix());
}

PathNode::~PathNode()
{
	delete this->matrix;
}

SquareMatrix PathNode::getMatrix()
{
	return *this->matrix;
}