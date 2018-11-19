#pragma once

#ifndef DCEL_H
#define DCEL_H

#include <iostream>
#include <vector>
#include <utility>
#include "point.h"
#include "math.h"

//#include "math.h"

class DCELVertex;
class DCELHalfEdge;
class DCELFace;

using namespace std;

class DCEL {
	public:
		/* Vertices*/
		vector<DCELVertex*> vertices;
		/* Edges */
		vector<DCELHalfEdge*> halfEdges;
		/* Facets */
		vector<DCELFace*> surfaces;
		
		DCELFace * openFace;

		DCEL();

		/**
		 *  Creates and returns a new vertex at the given point location.
		 */
		DCELVertex * addVertex(Point * point);

		/**
		 * Returns total count of faces
		 */
		int getFaceCount();

		/**
		 * Return total count of edges
		 */
		int getEdgeCount();

		/**
		 * Return total count of vertices
		 */
		int getVerticeCount();

		/**
		 * Creates an edge, i.e., two half edges, between two existing vertices.
		 * One of the two half edges is returned. The other half edge can be found
		 * using the twin attribute. Keep in mind that inserting an edge could result in
		 * creating a new face.
		 */
		DCELHalfEdge * createEdge(DCELVertex * v1, DCELVertex * v2);

		/**
		 * Returns all faces that are adjacent to a vertex v.
		 */
		vector<DCELFace * > findFaces(DCELVertex * vertex);

		/**
		 * Finds a common face between two vertices. A common face has the two given
		 * vertices on its boundary. If the two vertices have more than one common face,
		 * any bounded face is returned. That is, if they have two common faces and one
		 * of them is the unbounded face, the other one is returned. If more than one bounded
		 * face exists, any of them is returned. If no common faces exist,
		 * a NULL pointer is returned.
		 */
		DCELFace * findCommonFace(DCELVertex * v1, DCELVertex * v2);

		/**
		 * Returns true if and only if the two given vertices have a common edge between them.
		 */
		bool isConnected(DCELVertex * v1, DCELVertex * v2);

		/**
		 * If exists, returns the HalfEdge that has v as its source vertex and f as its face.
		 * If such an edge does not exist, a NULL pointer is returned.
		 */
		DCELHalfEdge * findIncidentEdge(DCELVertex * v, DCELFace * f);

		/**
		 * Deletes an existing Face
		 * If on any Edge the twin is already connected to the open Face, delete edge
		 * else connect all surrounding edges to the open Face
		 *
		 * @param
		 * @return One HalfEdge which isn't deleted at this state
		 */
		DCELHalfEdge * deleteFace(DCELFace * face);

		/**
		 * Prints the whole DCEL incl. exact Point Information
		 */
		void printDCEL();

		/**
		 * Prints all Infos about the DCEL
		 */
		void printDCELInfo();

	private:
		/**
		 * Deletes an existing edge between two vertices.
		 * If an edge is successfully deleted, a true is returned.
		 * Otherwise, if the given two vertices are not connected, a false is returned.
		 * Keep in mind that deleting an edge might result in merging two faces together.
		 */
		bool deleteEdge(DCELVertex * v1, DCELVertex * v2);

		bool deleteVertex(DCELVertex * v);
};

#endif // !DCEL_H