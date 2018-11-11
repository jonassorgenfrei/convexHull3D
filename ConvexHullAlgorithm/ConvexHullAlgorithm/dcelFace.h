#pragma once
#ifndef DCELFACE_H
#define DCELFACE_H

#include <vector>

class DCELHalfEdge;
class DCELVertex;


using namespace std;

class DCELFace {
public:
	DCELHalfEdge * edge; // points to a single HaldEdge that has this Face object as its face
	DCELFace() : edge(nullptr) {}
	void printFace();

	/**
	* Given a face, return its boundary vertices in CW order
	*/
	vector<DCELVertex*> getBoundary();

	/**
	* Given a face, return a list of all adjacent faces
	*/
	vector<DCELFace*> getAdjacentFaces();
};

#endif