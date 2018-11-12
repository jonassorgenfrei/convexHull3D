#pragma once

#include "dcel.h"
#include "dcelVertex.h"
#include "dcelHalfEdge.h"
#include "dcelFace.h"

/**
 * DCEL implementation 
 */


/**
 * DCEL constructor
 */
DCEL::DCEL() {
	this->openFace = new DCELFace();
};

/**
*  Creates and returns a new vertex at the given point location.
*/
DCELVertex * DCEL::addVertex(Point * point) {
	DCELVertex * newVertex = new DCELVertex();
	newVertex->point = point;
	this->vertices.push_back(newVertex);
	return newVertex;
}

/**
* Returns total Count of Faces
*/
int DCEL::getFaceCount() {
	return this->surfaces.size();
}

int DCEL::getEdgeCount()
{
	return this->halfEdges.size();
}

int DCEL::getVerticeCount()
{
	return this->vertices.size();
}

/**
* Creates an edge, i.e., two half edges, between two existing vertices.
* One of the two half edges is returned. The other half edge can be found
* using the twin attribute. Keep in mind that inserting an edge could result in
* creating a new face.
*/
DCELHalfEdge * DCEL::createEdge(DCELVertex * v1, DCELVertex * v2) {
	// if already connected
	if (isConnected(v1, v2)) {
		return nullptr;
	}

	// Edge Conneting v1 -> v2
	DCELHalfEdge * halfEdge = new DCELHalfEdge();
	// Edge Conneting v2 -> v1
	DCELHalfEdge * halfEdgeRet = new DCELHalfEdge();

	// Set Edge Attributes
	halfEdge->origin = v1;
	halfEdge->twin = halfEdgeRet;
	halfEdge->next = halfEdgeRet;
	halfEdge->face = openFace;

	// Set Edge Attributes
	halfEdgeRet->origin = v2;
	halfEdgeRet->twin = halfEdge;
	halfEdgeRet->next = halfEdge;
	halfEdgeRet->face = openFace;

	if (v2->leaving == nullptr) {
		// Case 1: No edge leaving from v2
		v2->leaving = halfEdgeRet;
		halfEdge->next = halfEdgeRet;
	} else {
		// Case 2: At least 1 edge leaving from v2
		vector<DCELHalfEdge *> leavingEdgesOfv2 = v2->leavingEdges();
		// Normal of Vertex v2 (with added edge)
		Vec3 normal;

		for (auto e : leavingEdgesOfv2) {
			normal = addVec3(normal, e->vec3());
		}
		normal = addVec3(normal, halfEdgeRet->vec3());
		normal = normalize(scalarDivVec3(-((double)(leavingEdgesOfv2.size()+1)),normal));
		
		Vec3 newEdge = projectVec3onPlane(halfEdgeRet->vec3(), normal);

		DCELHalfEdge * hs = leavingEdgesOfv2[0];
		double smallest = angleVec3(halfEdgeRet->vec3(), hs->vec3(),normal);
		DCELHalfEdge * hb = leavingEdgesOfv2[0];
		double biggest = angleVec3(halfEdgeRet->vec3(), hs->vec3(), normal);
		
		v2->normal = normal;
	}

	if (v1->leaving == nullptr) {
		// Case 1: No edge leaving from v2 
		v1->leaving = halfEdge;
		halfEdgeRet->next = halfEdge;
	}	else {
		// Case 2: At least 1 edge leaving from v1 
		vector<DCELHalfEdge *> leavingEdgesOfv1 = v2->leavingEdges();
		// Normal of Vertex v1 (with added edge) 
		Vec3 normal;

		for (auto e : leavingEdgesOfv1) {
			normal = addVec3(normal, e->vec3());
		}
		normal = addVec3(normal, halfEdgeRet->vec3());
		normal = normalize(scalarDivVec3(-((double)(leavingEdgesOfv1.size() + 1)), normal));

		v1->normal = normal;
	}


	this->halfEdges.push_back(halfEdge);
	this->halfEdges.push_back(halfEdgeRet);

	return halfEdge;
}

/**
 * Returns all faces that are adjacent to a vertex v.
 */
vector<DCELFace * > DCEL::findFaces(DCELVertex * vertex) {
	vector<DCELFace *> tempFaces;

	/*if (this->surfaces.size() < 2) {
		tempFaces.push_back(this->surfaces[0]);
		return tempFaces;
	}

	DCELHalfEdge * startEdge = vertex->leaving;
	DCELHalfEdge * tempEdge = vertex->nextLeaving(startEdge);
	tempFaces.push_back(startEdge->face);

	while (tempEdge != startEdge) {
		DCELFace * insertFace = tempEdge->face;
		bool insert = true;
		for (auto &value : tempFaces) {
			if (value == insertFace) {
				insert = false;
			}
		}
		if (insert) {
			tempFaces.push_back(insertFace);
		}
		tempEdge = vertex->nextLeaving(tempEdge);
	}*/

	return tempFaces;
}

/**
* Finds a common face between two vertices. A common face has the two given
* vertices on its boundary. If the two vertices have more than one common face,
* any bounded face is returned. That is, if they have two common faces and one
* of them is the unbounded face, the other one is returned. If more than one bounded
* face exists, any of them is returned. If no common faces exist,
* a NULL pointer is returned.
*/
DCELFace * DCEL::findCommonFace(DCELVertex * v1, DCELVertex * v2) {
	DCELFace * face = NULL;
	/*vector<DCELFace *> vec;
	if (this->surfaces.size() < 2) {
		return surfaces[0];
	}

	for (auto &face : this->surfaces) {
		bool v1B = false, v2B = false;
		vector<DCELVertex*> points = face->getBoundary();
		for (auto & point : points) {
			if (point == v1) {
				v1B = true;
			}
			if (point == v2) {
				v2B = true;
			}
		}
		if (v1B && v2B) {
			vec.push_back(face);
		}
	}

	if (vec.size() == 1) {
		return vec[0];
	}
	if (vec.size() == 2) {
		if (vec[0] != this->getUnboundedFace()) {
			return vec[0];
		}
		else {
			return vec[1];
		}
	}*/

	return face;
}

/**
* Returns true if and only if the two given vertices have a common edge between them.
*/
bool DCEL::isConnected(DCELVertex * v1, DCELVertex * v2) {
	/*DCELHalfEdge * startEdge = v1->leaving;
	if (v1->leaving == NULL || v2->leaving == NULL) {
		return false;
	}

	if (startEdge->destination() == v2) {
		return true;
	}
	DCELHalfEdge * tempEdge = v1->nextLeaving(startEdge);
	while (tempEdge != startEdge) {
		if (tempEdge->destination() == v2) {
			return true;
		}
		tempEdge = v1->nextLeaving(tempEdge);
	}*/
	return false;
}

/**
* If exists, returns the HalfEdge that has v as its source vertex and f as its face.
* If such an edge does not exist, a NULL pointer is returned.
*/
DCELHalfEdge * DCEL::findIncidentEdge(DCELVertex * v, DCELFace * f) {
	DCELHalfEdge * tempEdge = NULL;
	/*for (auto &edge : this->halfEdges) {
		if (edge->face == f && edge->origin == v) {
			return edge;
		}
	}*/
	return tempEdge;
}

/**
* Deletes an existing edge between two vertices.
* If an edge is successfully deleted, a true is returned.
* Otherwise, if the given two vertices are not connected, a false is returned.
* Keep in mind that deleting an edge might result in merging two faces together.
*/
bool DCEL::deleteEdge(DCELVertex * v1, DCELVertex * v2) {
	/*if (!isConnected(v1, v2)) {
		return false;
	}

	DCELHalfEdge * tempEdge = v1->leaving;;

	while (tempEdge->destination() != v2) {
		tempEdge = v1->nextLeaving(tempEdge);
	}

	if (tempEdge->face != tempEdge->twin->face) {
		DCELFace * deleteFace = tempEdge->twin->face;

		DCELHalfEdge * edge = tempEdge->twin->face->edge;
		DCELHalfEdge * tEdge = edge->next;

		while (tEdge != edge) {
			tEdge->face = tempEdge->face;
			tEdge = tEdge->next;
		}

		tEdge->face = tempEdge->face;

		int idx = 0;	//index from edge
		while (this->surfaces[idx] != deleteFace) {
			idx++;
		}
		this->surfaces.erase(this->surfaces.begin() + idx);
	}

	int idx = 0;	//index from edge
	while (this->halfEdges[idx] != tempEdge) {
		idx++;
	}
	this->halfEdges.erase(this->halfEdges.begin() + idx);

	idx = 0;	//index from twin edge
	while (this->halfEdges[idx] != tempEdge->twin) {
		idx++;
	}
	this->halfEdges.erase(this->halfEdges.begin() + idx);*/

	return true;
}

/**
* Given two DCEL structures, this function combines them together into a new DCEL.
* The returned DCEL combines all the vertices, edges, and faces in both DCELs.
* Except for the unbounded face, it is assumed that all vertices, edges,
* and faces in both DCELs are different.
* For efficiency purposes, the two input DCELs are destroyed as a result of calling
* this function.
*/
DCEL * DCEL::mergeAndDestroy(DCEL * d1, DCEL * d2) {
	/*for (auto &vertex : d2->vertices) {
		d1->vertices.push_back(vertex);
	}

	for (auto &halfEdges : d2->halfEdges) {
		d1->halfEdges.push_back(halfEdges);
		if (halfEdges->face == d2->getUnboundedFace()) {
			halfEdges->face = d1->getUnboundedFace();
		}
	}

	for (auto &face : d2->surfaces) {
		if (face != d2->getUnboundedFace()) {
			d1->surfaces.push_back(face);
		}
	}*/

	return d1;
}

void DCEL::printDCEL() {
	printf("DCEL : {\n");
	printf(" EDGES: {\n");
	for (auto &ed : this->halfEdges) {
		ed->printEdge(2);
	}
	printf(" }\n\n");

	printf(" Face: {\n");
	for (auto &f : this->surfaces) {
		f->printFace();
	}
	printf(" }\n\n");


	printf("}\n");
}

void DCEL::printDCELInfo() {
	std::cout << "DCEL-Info: {" << std::endl;
	std::cout << " Vertices: " << this->getVerticeCount() << std::endl;
	std::cout << " Edges: " << this->getEdgeCount() <<std::endl;
	std::cout << " Faces: " << this->getFaceCount() << std::endl;
	std::cout << "}" << std::endl;
}