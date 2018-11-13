#pragma once

#include "dcelHalfEdge.h"
#include "dcelVertex.h"

void DCELHalfEdge::printEdge(int space) {
	for (int i = 0; i < space; i++)
		std::cout << " ";
	std::cout << "Edge { ";
	//for (int i = 0; i < space; i++)
	//	std::cout << " ";
	printf(" Start: ");
	this->origin->point->print();
	//for (int i = 0; i < space; i++)
	//	std::cout << " ";
	std::cout << " End: ";
	this->next->origin->point->print();
	for (int i = 0; i < space; i++)
		std::cout << " ";
	std::cout << "}" << std::endl;
}

/**
* Returns the destination vertex of a HalfEdge.
* Note that we only store the source vertex of a HalfEdge.
* This function is used to return the destination vertex as well.
*/
DCELVertex * DCELHalfEdge::destination() {
	return this->next->origin;
}

Vec3 DCELHalfEdge::vec3()
{
	return {	next->origin->point->getX() - origin->point->getX(), 
				next->origin->point->getY() - origin->point->getY(),
				next->origin->point->getZ() - origin->point->getZ() };
}