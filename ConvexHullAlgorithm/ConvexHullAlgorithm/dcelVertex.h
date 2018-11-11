#ifndef DCELVertex_H
#define DCELVertex_H

#include <iostream>
#include <vector>
#include "math.h"

#include "point.h"
#include "dcelHalfEdge.h"

class DCELHalfEdge;

using namespace std;

class DCELVertex {
public:
	Point * point;
	DCELHalfEdge * leaving;
	Vec3 normal;
	DCELVertex() : leaving(nullptr) {}

	/**
	* Given an edge leaving a vertex v, this function returns the next edge
	* leaving the vertex v. Note that in the vertex, we only store one leaving edge.
	* This function should be used to iterate over all leaving edges of a vertex.
	*/
	DCELHalfEdge * nextLeaving(DCELHalfEdge * edge);
	
	/**
	 * Returns a Vector containing all leaving halfedges of the Vertex
	 */
	vector<DCELHalfEdge *> leavingEdges();

};

#endif
