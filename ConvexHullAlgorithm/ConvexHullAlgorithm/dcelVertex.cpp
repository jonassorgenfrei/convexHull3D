#pragma once

#include "dcelVertex.h"
#include "dcelHalfEdge.h"

/**
 * Given an edge leaving a vertex v, this function returns the next edge
 * leaving the vertex v. Note that in the vertex, we only store one leaving edge.
 * This function should be used to iterate over all leaving edges of a vertex.
 */
DCELHalfEdge * DCELVertex::nextLeaving(DCELHalfEdge * edge) {
	return edge->twin->next;
}

/**
 * Returns a Vector containing all leaving halfedges of the Vertex
 */
vector<DCELHalfEdge*> DCELVertex::leavingEdges()
{
	vector<DCELHalfEdge*> res;

	DCELHalfEdge * startEdge = this->leaving;
	DCELHalfEdge * nextLeaving = this->nextLeaving(startEdge);

	while (nextLeaving != startEdge) {
		res.push_back(nextLeaving);
		nextLeaving = this->nextLeaving(nextLeaving);
	}
	res.push_back(nextLeaving);
	   	 
	return res;
}
