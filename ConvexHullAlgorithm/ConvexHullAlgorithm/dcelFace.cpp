#pragma once
#include "stdio.h"
#include "dcelFace.h"
#include "dcelHalfEdge.h"
#include "dcelVertex.h"


void DCELFace::printFace() {
	printf("Face {\n");
	edge->printEdge(1);
	DCELHalfEdge * temp = edge->next;
	while (temp->origin != edge->origin) {
		temp->printEdge(1);
		temp = temp->next;
	}
	printf("}\n");
}

/**
* Given a face, return its boundary vertices in CW order
*/
vector<DCELVertex*> DCELFace::getBoundary() {
	vector<DCELVertex*> tempVertices;
	/*DCELVertex * tempVertex;
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

	*/
	return tempVertices;
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
