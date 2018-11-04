#pragma once
#ifndef EDGE_H
#define EDGE_H

#include "point.h"

class Edge {
public:
	Edge2D(Point * start, Point * end) {
		this->start = start;
		this->end = end;
	}
private:
	Point * start;
	Point * end;
};

#endif // !EDGE_H
