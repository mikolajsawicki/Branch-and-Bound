#pragma once
#include "SquareMatrix.h"

class PathNode
{
private:
	int indexInPath;
	int indexInMatrix;
	SquareMatrix* matrix;

public:
	PathNode(SquareMatrix& matrix, int index_in_matrix);
	PathNode(PathNode &node, int index_in_matrix);
	~PathNode();
	SquareMatrix getMatrix();
};

