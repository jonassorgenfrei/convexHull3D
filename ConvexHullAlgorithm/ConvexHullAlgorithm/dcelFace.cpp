#pragma once

#include "dcelFace.h"
#include "dcelHalfEdge.h"
#include "dcelVertex.h"

/**
* Given a face, return its boundary vertices in CW order
*/
vector<DCELVertex*> DCELFace::getBoundary() {
	vector<DCELVertex*> tempVertices;
	if (this->edge == nullptr) {
		return tempVertices;
	}
	DCELVertex * tempVertex;
	DCELHalfEdge * tempEdge;

	if (this == this->edge->face) {
		tempEdge = this->edge;
	}
	else {
		tempEdge = this->edge->twin;
	}

	tempVertex = tempEdge->origin;
	tempVertices.push_back(tempVertex);

	while (tempVertex != tempEdge->next->origin) {
		tempEdge = tempEdge->next;
		tempVertices.push_back(tempEdge->origin);
	}
	return tempVertices;
}

vector<DCELHalfEdge*> DCELFace::getEdgeBoundary()
{
	vector<DCELHalfEdge*> tempEdges;
	DCELHalfEdge * startEdge;
	DCELHalfEdge * tempEdge;

	if (this == this->edge->face) {
		startEdge = this->edge;
	}
	else {
		startEdge = this->edge->twin;
	}
	tempEdge = startEdge->next;
	tempEdges.push_back(startEdge);

	while (startEdge != tempEdge) {
		tempEdges.push_back(tempEdge);
		tempEdge = tempEdge->next;
	}

	return tempEdges;
}

void DCELFace::saveBoundary()
{
	this->savedBoundary = getBoundary();
}

vector<DCELVertex*> DCELFace::getSavedBoundary()
{
	if (this->savedBoundary.size() == 0) {
		saveBoundary();
	}

	return this->savedBoundary;
}

/**
* Given a face, return a list of all adjacent faces
*/
vector<DCELFace*> DCELFace::getAdjacentFaces() {
	vector<DCELFace*> tempFaces;

	/*DCELVertex * tempVertex;
	DCELHalfEdge * tempEdge;

	if (this == this->edge->face) {
		tempEdge = this->edge;
	}
	else {
		tempEdge = this->edge->twin;
	}

	tempVertex = tempEdge->origin;
	tempFaces.push_back(tempEdge->twin->face);

	while (tempVertex != tempEdge->next->origin) {
		bool insert = true;
		tempEdge = tempEdge->next;

		for (auto &value : tempFaces) {
			if (value == tempEdge->twin->face) {
				insert = false;
			}
		}

		if (insert) {
			tempFaces.push_back(tempEdge->twin->face);
		}
	}*/

	return tempFaces;
}

void DCELFace::printFace() {
	std::cout << "Face {" << std::endl;
	edge->printEdge(1);
	DCELHalfEdge * temp = edge->next;
	while (temp->origin != edge->origin) {
		temp->printEdge(1);
		temp = temp->next;
	}
	std::cout << "}" << std::endl;
}