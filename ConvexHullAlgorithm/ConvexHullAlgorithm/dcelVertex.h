#ifndef DCELVertex_H
#define DCELVertex_H

#include "point.h"
#include "dcelHalfEdge.h"

class DCELHalfEdge;

class DCELVertex {
public:
	Point * point;
	DCELHalfEdge * leaving;
	DCELVertex() : leaving(nullptr) {}

	/**
	* Given an edge leaving a vertex v, this function returns the next edge
	* leaving the vertex v. Note that in the vertex, we only store one leaving edge.
	* This function should be used to iterate over all leaving edges of a vertex.
	*/
	DCELHalfEdge * nextLeaving(DCELHalfEdge * edge);
};

#endif
