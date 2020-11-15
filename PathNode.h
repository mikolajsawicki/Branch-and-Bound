#pragma once
#include "SquareMatrix.h"

class PathNode
{
private:
	int indexInPath;
	int indexInMatrix;
	SquareMatrix* matrix;
	int bound;
	int pathWeight;

	void calculateBound();

public:
	PathNode(SquareMatrix* matrix, int index_in_matrix);
	PathNode(PathNode &node, int index_in_matrix);
	~PathNode();
	SquareMatrix getMatrix();
	int getPathWeight() { return pathWeight; }
};

