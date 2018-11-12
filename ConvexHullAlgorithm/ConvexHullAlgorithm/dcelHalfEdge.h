#pragma once

#ifndef DCELHALFEDGE_H
#define DCELHALFEDGE_H

#include <iostream>

//#include "math.h"

class DCELVertex;
class DCELFace;

class DCELHalfEdge {
public:
	DCELVertex * origin; // origin from which the HalfEdge starts
	DCELFace * face;    //face on the left side
	DCELHalfEdge * twin; //points to the HaldEdge on the right side of the HalfEdge
	DCELHalfEdge * next; //points to the HaldEdge that starts from h->twin->origin
						 //ends at the next vertex h->face
						 //traveling counterclockwise around the boundary

	DCELHalfEdge() : origin(nullptr), twin(nullptr), next(nullptr), face(nullptr) {}

	void printEdge(int space);

	/**
	* Returns the destination vertex of a HalfEdge.
	* Note that we only store the source vertex of a HalfEdge.
	* This function is used to return the destination vertex as well.
	*/
	DCELVertex * destination();
	
	/**
	 * Get Vector
	 */
	//Vec3 vec3();
};

#endif
