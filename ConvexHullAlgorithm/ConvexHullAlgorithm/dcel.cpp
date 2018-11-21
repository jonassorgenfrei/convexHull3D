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

/**
* Returns total Count of Edges
*/
int DCEL::getEdgeCount()
{
	return this->halfEdges.size();
}

/**
* Returns total Count of Vertices
*/
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
	//this->printDCEL();
	// if already connected
	if (v1 == v2 || isConnected(v1, v2)) {
		return nullptr;
	}

	/* --- Create new Edges --- */

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

	/* --- Connect new Edges --- */

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
		double smallest = angleVec3(newEdge, projectVec3onPlane(hs->vec3(), normal), normal);
		DCELHalfEdge * hb = leavingEdgesOfv2[0];
		double biggest = angleVec3(newEdge, projectVec3onPlane(hs->vec3(), normal), normal);

		for (int i = 1; i < leavingEdgesOfv2.size(); i++) {
			double angle = angleVec3(newEdge, projectVec3onPlane(leavingEdgesOfv2[i]->vec3(), normal), normal);
			if (angle < smallest) {
				smallest = angle;
				hs = leavingEdgesOfv2[i];
			}

			if (angle > biggest) {
				biggest = angle;
				hb = leavingEdgesOfv2[i];
			}
		}

		hb->twin->next = halfEdgeRet;
		halfEdge->next = hs;

		v2->normal = normal;
	}

	if (v1->leaving == nullptr) {
		// Case 1: No edge leaving from v2 
		v1->leaving = halfEdge;
		halfEdgeRet->next = halfEdge;
	}	else {
		// Case 2: At least 1 edge leaving from v1 
		vector<DCELHalfEdge *> leavingEdgesOfv1 = v1->leavingEdges();

		// Normal of Vertex v1 (with added edge) 
		Vec3 normal;

		for (auto e : leavingEdgesOfv1) {
			normal = addVec3(normal, e->vec3());
		}
		normal = addVec3(normal, halfEdge->vec3());
		normal = normalize(scalarDivVec3(-((double)(leavingEdgesOfv1.size() + 1)), normal));

		Vec3 newEdge = projectVec3onPlane(halfEdge->vec3(), normal);
		
		DCELHalfEdge * hs = leavingEdgesOfv1[0];
		double smallest = angleVec3(newEdge, projectVec3onPlane(hs->vec3(), normal), normal);
		DCELHalfEdge * hb = leavingEdgesOfv1[0];
		double biggest = angleVec3(newEdge, projectVec3onPlane(hs->vec3(), normal), normal);
		

		for (int i = 1; i < leavingEdgesOfv1.size(); i++) {
			double angle = angleVec3(newEdge, projectVec3onPlane(leavingEdgesOfv1[i]->vec3(), normal), normal);
			if (angle < smallest) {
				smallest = angle;
				hs = leavingEdgesOfv1[i];
			}

			if (angle > biggest) {
				biggest = angle;
				hb = leavingEdgesOfv1[i];
			}
		}

		hb->twin->next = halfEdge;
		halfEdgeRet->next = hs;

		v1->normal = normal;
	}
	
	/* --- Check if face has to be created ---*/

	// check if new circle is closed
	DCELHalfEdge * tempEdge = halfEdge;
	bool twinEdgeInc = tempEdge->next == halfEdgeRet;
	while (!twinEdgeInc && tempEdge->next != halfEdge ) {
		tempEdge = tempEdge->next;
		twinEdgeInc = tempEdge->next == halfEdgeRet;
	}

	// create new face
	if (!twinEdgeInc) {
		DCELFace * newFace = new DCELFace();

		DCELHalfEdge * tempE = halfEdge;
		DCELHalfEdge * tempER = halfEdgeRet;
		// if first triangle (1. triangle has to be created in ClockWiseOrder, to show the coordinate System!)
		if (this->surfaces.size() == 0) {
			newFace->edge = halfEdge;
			halfEdge->face = newFace;
			tempE = halfEdge->next;
			while (tempE != halfEdge) {
				tempE->face = newFace;
				tempE = tempE->next;
			}
		} else {
			/* TODO COMPARE LOOP LENGTH & */
			while (tempE->next != halfEdge && tempER->next != halfEdgeRet) {
				tempE = tempE->next;
				tempER = tempER->next;
			}

			// if same length
			if (tempE->next == halfEdge && tempER->next == halfEdgeRet) {
				// if closing hole
				if (halfEdge->next->face == halfEdgeRet->next->face && halfEdgeRet->next->face != openFace) {
					newFace->edge = halfEdge;
					halfEdge->face = newFace;
					tempE = halfEdge->next;
					while (tempE != halfEdge) {
						tempE->face = newFace;
						tempE = tempE->next;
					}

					halfEdgeRet->face = halfEdgeRet->next->face;
					halfEdgeRet->next->face->edge = halfEdgeRet;
				} // if spliting facet
				else {
					newFace->edge = halfEdge;
					halfEdge->face = newFace;
					tempE = halfEdge->next;
					while (tempE != halfEdge) {
						tempE->face = newFace;
						tempE = tempE->next;
					}

					DCELFace * closingFace2 = new DCELFace();
					closingFace2->edge = halfEdgeRet;

					halfEdgeRet->face = closingFace2;
					tempE = halfEdgeRet->next;
					while (tempE != halfEdgeRet) {
						tempE->face = closingFace2;
						tempE = tempE->next;
					}

					this->surfaces.push_back(closingFace2);
				} // Another Case ???
				/* TODO: !! */
			} else {
				if (tempE->next == halfEdge) {
					newFace->edge = halfEdge;
					halfEdge->face = newFace;
					tempE = halfEdge->next;
					while (tempE != halfEdge) {
						tempE->face = newFace;
						tempE = tempE->next;
					}
				}
				else {
					newFace->edge = halfEdgeRet;
					halfEdgeRet->face = newFace;
					tempER = halfEdgeRet->next;
					while (tempER != halfEdgeRet) {
						tempER->face = newFace;
						tempER = tempER->next;
					}
				}
			}
		}

		this->surfaces.push_back(newFace);
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

	if (this->surfaces.size() == 1) {
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
	}

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
	DCELHalfEdge * startEdge = v1->leaving;
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
	}

	return false;
}

/**
 * If exists, returns the HalfEdge that has v as its source vertex and f as its face.
 * If such an edge does not exist, a NULL pointer is returned.
 */
DCELHalfEdge * DCEL::findIncidentEdge(DCELVertex * v, DCELFace * f) {
	DCELHalfEdge * tempEdge = NULL;
	for (auto &edge : this->halfEdges) {
		if (edge->face == f && edge->origin == v) {
			return edge;
		}
	}
	return tempEdge;
}

/**
 * Deletes an existing Face
 * If on any Edge the twin is already connected to the open Face, delete edge
 * else connect all surrounding edges to the open Face
 * 
 * @param
 * @return one HalfEdge which isn't deleted at this state 
 */
DCELHalfEdge* DCEL::deleteFace(DCELFace * face) {
	int idx = 0;
	while (idx < this->surfaces.size() && this->surfaces[idx] != face) {
		idx++;
	}
	// If face is not part of the curretn DCEL
	if (idx == this->surfaces.size()) {
		return nullptr;
	}
	
	DCELHalfEdge * retEdge = new DCELHalfEdge();

	vector<DCELHalfEdge*> hB = face->getEdgeBoundary();
	for (DCELHalfEdge * halfEdge : hB) {
		if (halfEdge->twin->face == openFace) {
			halfEdge->face = openFace;
			deleteEdge(halfEdge->origin, halfEdge->next->origin);
		}
		else {
			halfEdge->oldFace = halfEdge->face;
			halfEdge->face = openFace;
			retEdge = halfEdge;
		}
	}
	this->surfaces.erase(this->surfaces.begin() + idx);
	return retEdge;
}

/**
 * Deletes an existing edge between two vertices.
 * If an edge is successfully deleted, a true is returned.
 * Otherwise, if the given two vertices are not connected, a false is returned.
 * Keep in mind that deleting an edge might result in merging two faces together.
 */
bool DCEL::deleteEdge(DCELVertex * v1, DCELVertex * v2) {
	if (v1 == v2 || !isConnected(v1, v2)) {
		return false;
	}

	DCELHalfEdge * tempEdge = v1->leaving;;

	while (tempEdge->destination() != v2) {
		tempEdge = v1->nextLeaving(tempEdge);
	}

	if (v1->leavingEdges().size() == 1) {
		v1->leaving = nullptr;
		deleteVertex(v1);
	}
	else {
		if (v1->leaving == tempEdge || v1->leaving == tempEdge->twin) {
			v1->leaving = v1->nextLeaving(v1->leaving);
		}
	}
	if (v2->leavingEdges().size() == 1) {
		v2->leaving = nullptr;
		deleteVertex(v2);
	}
	else {
		if (v2->leaving == tempEdge || v2->leaving == tempEdge->twin) {
			v2->leaving = v2->nextLeaving(v2->leaving);
		}
	}

	if (tempEdge->face != tempEdge->twin->face) {
		// connecting edges to right face
		for (DCELHalfEdge * edge : tempEdge->twin->face->getEdgeBoundary()) {
			edge->face = tempEdge->face;
		}
	}
	
	if (tempEdge->next != tempEdge->twin) {
		DCELHalfEdge * start = tempEdge->twin;
		DCELHalfEdge * temp = start;
		while (temp->next != start) {
			temp = temp->next;
		}
		temp->next = tempEdge->next;
	}

	if (tempEdge->twin->next != tempEdge) {
		DCELHalfEdge * start = tempEdge;
		DCELHalfEdge * temp = start;
		while (temp->next != start) {
			temp = temp->next;
		}
		temp->next = tempEdge->twin->next;
	}

	tempEdge->next = tempEdge->twin;
	tempEdge->twin->next = tempEdge;

	int idx = 0;	//index from edge
	while (this->halfEdges[idx] != tempEdge) {
		idx++;
	}
	this->halfEdges.erase(this->halfEdges.begin() + idx);

	idx = 0;	//index from twin edge
	while (this->halfEdges[idx] != tempEdge->twin) {
		idx++;
	}
	this->halfEdges.erase(this->halfEdges.begin() + idx);

	return true;
}

bool DCEL::deleteVertex(DCELVertex * v) {
	int idx = 0;

	while (idx < this->vertices.size() && this->vertices[idx] != v) {
		idx++;
	}

	if (idx < this->vertices.size()) {
		this->vertices.erase(this->vertices.begin() +idx);
		return true;
	}
	else {
		return false;
	}

}

/**
 * Prints the whole DCEL incl. exact Point Information
 */
void DCEL::printDCEL() {
	printf("DCEL : {\n");
		printf(" Vertices: {\n");
		for (auto &vert : this->vertices) {
			vert->point->print();
			printf("\n");
		}
		printf(" }\n\n");

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

/**
 * Prints all Infos about the DCEL
 */
void DCEL::printDCELInfo() {
	std::cout << "DCEL-Info: {" << std::endl;
		std::cout << " Vertices: " << this->getVerticeCount() << std::endl;
		std::cout << " Edges: " << this->getEdgeCount() <<std::endl;
		std::cout << " Faces: " << this->getFaceCount() << std::endl;
	std::cout << "}" << std::endl;
}