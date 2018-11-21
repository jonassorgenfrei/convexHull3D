#pragma once

#ifndef CONFLICT_GRAPH_H
#define CONFLICT_GRAPH_H

#include <vector>

#include "math.h"
#include "point.h"
#include "point3D.h"
#include "dcelFace.h"

class Conflict;

class ConflictPoint {
public:
	ConflictPoint(Point * point) {
		this->point = point;
	};

	Point * point;
	vector<Conflict*> conflicts;

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

class ConflictFace {
public:
	ConflictFace(DCELFace * face) {
		this->face = face;
	};

	DCELFace * face;
	vector<Conflict*> conflicts;
};

class Conflict {
public:

	Conflict(ConflictPoint * point, ConflictFace * face) {
		this->point = point;
		this->face = face;
	};

	ConflictPoint * point;
	ConflictFace * face;
};

class ConflictGraph {
public:
	ConflictGraph() {};

	Conflict * checkForConflict(ConflictPoint * point, ConflictFace * face) {
		// Check on Which side of the face the Point lies
		vector<DCELVertex*> vertices = face->face->getSavedBoundary();
		if(vertices.size() > 0){
			Vec3 faceNormal = crossVec3(
				{ vertices[2]->point->getX() - vertices[1]->point->getX(),
					vertices[2]->point->getY() - vertices[1]->point->getY(),
					vertices[2]->point->getZ() - vertices[1]->point->getZ() },
				{ vertices[0]->point->getX() - vertices[1]->point->getX(),
					vertices[0]->point->getY() - vertices[1]->point->getY(),
					vertices[0]->point->getZ() - vertices[1]->point->getZ() }
			);
			Vec3 vecFP = { point->point->getX() - vertices[1]->point->getX(),
							point->point->getY() - vertices[1]->point->getY(),
							point->point->getZ() - vertices[1]->point->getZ()
			};
			if (dotVec3(faceNormal, vecFP) < 0.0) {
				Conflict * conflict = new Conflict(point, face);
				face->conflicts.push_back(conflict);
				point->conflicts.push_back(conflict);
			}
		}
		return nullptr;
	}

	ConflictPoint* createConflictPoint(Point * point) {
		for (ConflictPoint * cP : this->conflictPoints) {
			if (cP->point == point) {
				return cP;
			}
		}
		ConflictPoint * nCP = new ConflictPoint(point);
		this->conflictPoints.push_back(nCP);
		return nCP;
	}

	ConflictFace* createConflictFace(DCELFace * face) {
		for (ConflictFace * cF : this->conflictFaces) {
			if (cF->face == face) {
				return cF;
			}
		}
		ConflictFace * nCF = new ConflictFace(face);
		this->conflictFaces.push_back(nCF);
		return nCF;
	}

	bool deleteCorrespondingNodes(ConflictPoint * point) {
		bool res = true;

		// Loop through conflict faces
		for (Conflict * c : point->conflicts) {
			ConflictFace * cf = c->face;
			// Delete Conflicts of Faces
			for (Conflict * fc : cf->conflicts) {
				res = res && fc->point->deleteConflict(fc);
			}
			// Delete Face
			int idx = -1;
			int counter = 0;

			while (idx < 0 && counter < this->conflictFaces.size()) {
				if (this->conflictFaces[counter] == cf)
					idx = counter;
				counter++;
			}

			if (idx > -1)
				this->conflictFaces.erase(this->conflictFaces.begin() + idx);

			res = res && (idx > -1);
		}

		// Delete Point
		int idx = -1;
		int counter = 0;

		while (idx < 0 && counter < this->conflictPoints.size()) {
			if (this->conflictPoints[counter] == point)
				idx = counter;
			counter++;
		}

		if (idx > -1)
			this->conflictPoints.erase(this->conflictPoints.begin() + idx);
		
		res = res && (idx > -1);

		return res;
	}

private:
	vector<ConflictPoint*> conflictPoints;
	vector<ConflictFace*> conflictFaces;
};

#endif // !CONFLICT_GRAPH_H

