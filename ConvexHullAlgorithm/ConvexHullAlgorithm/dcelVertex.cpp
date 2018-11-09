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
