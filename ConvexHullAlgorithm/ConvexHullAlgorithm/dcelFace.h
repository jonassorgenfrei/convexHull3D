#pragma once

#ifndef DCELFACE_H
#define DCELFACE_H

#include <iostream>
#include <vector>

class DCELHalfEdge;
class DCELVertex;


using namespace std;

/**
 * Faces in Clock-Wise Order
 */
class DCELFace {
public:
	DCELHalfEdge * edge; // points to a single HaldEdge that has this Face object as its face
	DCELFace() : edge(nullptr) {}
	vector<DCELVertex*> savedBoundary;

	/**
	* Given a face, return its boundary vertices in CW order
	*/
	vector<DCELVertex*> getBoundary();

	vector<DCELHalfEdge *> getEdgeBoundary();

	void saveBoundary();

	vector<DCELVertex *> getSavedBoundary();

	/**
	* Given a face, return a list of all adjacent faces
	*/
	vector<DCELFace*> getAdjacentFaces();

	void printFace();
};

#endif
