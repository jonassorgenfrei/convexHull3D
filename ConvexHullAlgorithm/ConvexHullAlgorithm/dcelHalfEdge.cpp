#include "dcelHalfEdge.h"
#include "dcelVertex.h"
#include "stdio.h"


void DCELHalfEdge::printEdge(int space) {
	for (int i = 0; i < space; i++)
		printf(" ");
	printf("Edge {\n");
	for (int i = 0; i < space; i++)
		printf(" ");
	printf(" Start: ");
	this->origin->point->print();
	for (int i = 0; i < space; i++)
		printf(" ");
	printf(" End: ");
	this->next->origin->point->print();
	for (int i = 0; i < space; i++)
		printf(" ");
	printf("}\n");
}

/**
* Returns the destination vertex of a HalfEdge.
* Note that we only store the source vertex of a HalfEdge.
* This function is used to return the destination vertex as well.
*/
DCELVertex * DCELHalfEdge::destination() {
	return this->next->origin;
}
