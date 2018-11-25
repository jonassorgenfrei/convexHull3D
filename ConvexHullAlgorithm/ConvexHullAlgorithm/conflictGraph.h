#pragma once

/*
 * Conflict Graph (Header-Only)
 * Data-Structure to store conflicts between Points & Faces
 *
 * Part of the seminar talk at FH Wedel 2018
 *
 * @author -	Jonas Sorgenfrei
 *				Minf101767
 */

#ifndef CONFLICT_GRAPH_H
#define CONFLICT_GRAPH_H

#include <vector>

#include "math.h"
#include "point.h"
#include "point3D.h"
#include "dcelFace.h"

class Conflict;

/**
 * Class for Conflict Points
 */
class ConflictPoint {
public:
	ConflictPoint(Point * point) {
		this->point = point;
	};

	Point * point;
	vector<Conflict*> conflicts;

	/**
	 * Deletes Conflict from Conflictlist in this point, 
	 * if its in the Conflict list
	 */
	bool deleteConflict(Conflict * conflict) {
		int idx = -1;
		int counter = 0;

		while (idx < 0 && counter < this->conflicts.size()) {
			if (this->conflicts[counter] == conflict)
				idx = counter;
			counter++;
		}

		if(idx > -1)
			this->conflicts.erase(this->conflicts.begin() + idx);

		return (idx > -1);
	};
};

/**
 * Class for Conflict Faces
 */
class ConflictFace {
public:
	ConflictFace(DCELFace * face) {
		this->face = face;
	};

	DCELFace * face;
	vector<Conflict*> conflicts;
};

/**
 * Class for Conflicts
 */
class Conflict {
public:

	Conflict(ConflictPoint * point, ConflictFace * face) {
		this->point = point;
		this->face = face;
	};

	ConflictPoint * point;
	ConflictFace * face;
};

/**
 * Class for Conflict Graph it self
 */
class ConflictGraph {
public:
	vector<ConflictPoint*> conflictPoints;
	vector<ConflictFace*> conflictFaces;

	ConflictGraph() {};

	/**
	 * Checks in which Halfspace of a plane hf (defined by the 3 points of the triangle/face)
	 * the input Lies
	 * @param point - point p to check
	 * @param face - face to check
	 * @return if a Conflict is created it returns the Conflict otherwise it returns a null pointer
	 */
	Conflict * checkForConflict(ConflictPoint * point, ConflictFace * face) {
		// Get vertices around the face
		vector<DCELVertex*> vertices = face->face->getBoundary();
		if(vertices.size() > 0){
			// create (front)-face normal, using the cross product on the 2 vectors defining the face
			Vec3 faceNormal = crossVec3(
				{ vertices[2]->point->getX() - vertices[1]->point->getX(),
					vertices[2]->point->getY() - vertices[1]->point->getY(),
					vertices[2]->point->getZ() - vertices[1]->point->getZ() },
				{ vertices[0]->point->getX() - vertices[1]->point->getX(),
					vertices[0]->point->getY() - vertices[1]->point->getY(),
					vertices[0]->point->getZ() - vertices[1]->point->getZ() }
			);
			// creating a vector from point 1 in the face to the point p
			Vec3 vecFP = { point->point->getX() - vertices[1]->point->getX(),
							point->point->getY() - vertices[1]->point->getY(),
							point->point->getZ() - vertices[1]->point->getZ()
			};
			// Using the dot-produkt to determine in which halfspace the point lies
			if (dotVec3(faceNormal, vecFP) <= 0.0) {
				// only create a new conflict, if the the point lies on the back-side of the 
				// face
				Conflict * conflict = new Conflict(point, face);
				face->conflicts.push_back(conflict);
				point->conflicts.push_back(conflict);
			}
		}
		return nullptr;
	}

	/**
	 * Creates a new conflict point datastructur if it doesn't exist yet, 
	 * otherwise returns the existing one for point p
	 *	@param point - point p to wrap in a Conflict Point
	 *	@return conflict-point
	 */
	ConflictPoint* createConflictPoint(Point * point) {
		// Check if a conflict-point for point p already exist
		for (ConflictPoint * cP : this->conflictPoints) {
			if (cP->point == point) {
				return cP; // return the exisiting one
			}
		}
		ConflictPoint * nCP = new ConflictPoint(point);	// create new Conflict Point
		this->conflictPoints.push_back(nCP);
		return nCP; // return new conflict Point
	}

	/**
	 * Creates a new conflict-face datastructur if it doesn't exist yet,
	 * otherwise returns the existing one for Face f
	 *	@param face - point p to wrap in a Conflict Point
	 *	@return conflict-face
	 */
	ConflictFace* createConflictFace(DCELFace * face) {
		// Check if a conflict-face for face f already exist
		for (ConflictFace * cF : this->conflictFaces) {
			if (cF->face == face) {
				return cF; // return the exisiting one
			}
		}
		ConflictFace * nCF = new ConflictFace(face); // create new conflict face
		this->conflictFaces.push_back(nCF);
		return nCF; // return new conflict face
	}

	/**
	 * Deletes a conflict points and all conflicts with correspoing conflict-faces
	 * & conflicts for these deleted conflict-faces
	 *	@param point - conflict-point to delete
	 *	@return true if successfull, false if not
	 */
	bool deleteCorrespondingNodes(ConflictPoint * point) {
		// return value
		bool res = true;

		// Loop through conflict faces
		for (Conflict * c : point->conflicts) {
			ConflictFace * cf = c->face;
			// Delete Conflicts of Faces
			for (Conflict * fc : cf->conflicts) {
				if(fc->point != point)
					res = res && fc->point->deleteConflict(fc);
			}
			// Delete Face
			int idx = -1;
			int counter = 0;

			while (idx < 0 && counter < this->conflictFaces.size()) {
				if (this->conflictFaces[counter] == cf) {
					idx = counter;
				}
				counter++;
			}
			if (idx > -1) {
				
				this->conflictFaces.erase(this->conflictFaces.begin() + idx);
			}

			res = res && (idx > -1);
		}

		// Delete Point
		int idx = -1;
		int counter = 0;

		while (idx < 0 && counter < this->conflictPoints.size()) {
			if (this->conflictPoints[counter] == point) {
				idx = counter;
			}
			counter++;
		}

		if (idx > -1)
			this->conflictPoints.erase(this->conflictPoints.begin() + idx);
		
		res = res && (idx > -1);

		return res; // return
	}
};

#endif // !CONFLICT_GRAPH_H

