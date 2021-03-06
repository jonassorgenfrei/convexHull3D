﻿#pragma once
/*
 * Convex Hull Modul
 * Algorithm for 2D & 3D convex hull calculation.
 *
 * Part of the seminar talk at FH Wedel 2018
 *
 * @author -	Jonas Sorgenfrei
 *				Minf101767
 */


#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <vector>
#include <algorithm>

#include "point.h"
#include "dcel.h"
#include "dcelHalfEdge.h"
#include "visualisation.h"
#include "conflictGraph.h"
#include "math.h"

/*
 * Convex Hull Algorithm in 2D Space
 * Graham Scan Algorithm
 *
 * @param points - Input Points to operate on
 * @return Convex Hull of the Points
 *
 */
std::vector<Point*> ConvexHull2D(std::vector<Point*> points) {
	// Empty Convex Hull
	std::vector<Point*> ch;

	if (points.size() < 3)
		return ch;

	// Select the Point with minimum y, if there are more points with min Y select Point with min X
	double y_min = points[0]->getY();
	Point * min_Y_Point = points[0];
	int idx_min = 0;

	for (int i = 0; i < points.size(); i++) {
		if (y_min > points[i]->getY() || (cmpd(y_min,points[i]->getY()) &&
			points[i]->getX() < points[idx_min]->getX())) {
				y_min = points[i]->getY();
				min_Y_Point = points[i];
				idx_min = i;
		}

	}

	//Swap points[0] with the point with the lowest y-coordinate
	points[idx_min] = points[0];
	points[0] = min_Y_Point;

	// Sort all points in CCW order {p0, p1, .. pn}
	std::sort(points.begin()+1, points.end(), [min_Y_Point](const auto& lhs, const auto& rhs)
	{
		Vec2 vec1 = {rhs->getX() - min_Y_Point->getX(), rhs->getY() - min_Y_Point->getY() };
		Vec2 vec2 = {lhs->getX() - min_Y_Point->getX(), lhs->getY() - min_Y_Point->getY() };

		double cross = crossVec2(vec1, vec2);

		if (cross < 0.0) {
			return true;
		}
		else if (cross > 0.0) {
			return false;
		}
		else {
			return (lengthVec2(vec1) > lengthVec2(vec2));
		}
	});

	// push points[{0,1,2}] to stack
	ch.push_back(points[0]);
	ch.push_back(points[1]);
	ch.push_back(points[2]);

	// for i = 3 to N-1
	for (int i = 3; i < points.size(); i++) {
		// while ccw(next_to_top(stack), top(stack), points[i]) <= 0:
		while (ch.size() >= 2 && ccw(ch[ch.size() -2 ], ch[ch.size()-1], points[i]) <= 0.0) {
			// pop stack
			ch.pop_back();
		}
		// push points[i] to stack
		ch.push_back(points[i]);
	}

	return ch;
}


/*
 * Convex Hull Algorithm in 3D Space
 *
 * Algorithm Convex Hull(P)
 *
 * @param points - A set P of n points (x,y,z) in three-space. 
 * @return The convex hull CH(P) of P.
 */
DCEL ConvexHull3D(std::vector<Point*> points) {
	DCEL dcel;

	if (points.size() < 4)
		return dcel;

	// 1. Find 4 points that form a tetrahedron (don't lie in a common plane)
	int p1 = -1, p2 = -1, p3 = -1, p4=-1;
	
	// TODO: IMPROVE ALL POSSIBLE PERMUTATIONS!!!!!!!!!1
	p1 = 0;
	p2 = 1;
	
	int i = 2;
	while (p3 < 0 && i < points.size()) {
		if (!checkCollinear(points[p1], points[p2], points[i])) {
			p3 = i;
		}
		i++;
	}

	i = 2;
	while (p4 < 0 && i < points.size()) {
		if (i != p1 && i != p2 && i != p3) {
			if (!checkCoplanar(points[p1], points[p2], points[p3], points[i])) {
				p4 = i;
			}
		}
		i++;
	}
	
	// TODO: END

	// If there are no 4 points coplanar in the set of points
	if (p4 < 0 || p3 < 0 || p2 < 0 || p1 < 0) {
		// Theoretical run 2D Convex Hull!!
		return dcel;
	}

	// 2. Insert Points into DCEL (by creating a initialise tetrahedron)
	DCELVertex * point1 = dcel.addVertex(points[p1]);
	DCELVertex * point2 = dcel.addVertex(points[p2]);
	DCELVertex * point3 = dcel.addVertex(points[p3]);
	DCELVertex * point4 = dcel.addVertex(points[p4]);

	Vec3 a = { points[p2]->getX() - points[p1]->getX(),
			   points[p2]->getY() - points[p1]->getY(),
					  points[p2]->getZ() - points[p1]->getZ()
	};
	Vec3 b = { points[p3]->getX() - points[p1]->getX(),
			  points[p3]->getY() - points[p1]->getY(),
			  points[p3]->getZ() - points[p1]->getZ() };
	Vec3 c = { points[p4]->getX() - points[p1]->getX(),
			  points[p4]->getY() - points[p1]->getY(),
			  points[p4]->getZ() - points[p1]->getZ() };

	double orientation = dotVec3(normalize(c), normalize(crossVec3(a, b)));

	DCELHalfEdge * h1;
	DCELHalfEdge * h2;
	DCELHalfEdge * h3;

	if (orientation > 0) {
		h1 = dcel.createEdge(point1, point2);
		h2 = dcel.createEdge(point2, point3);
		h3 = dcel.createEdge(point3, point1);
	}
	else {
		h1 = dcel.createEdge(point1, point3);
		h2 = dcel.createEdge(point3, point2);
		h3 = dcel.createEdge(point2, point1);
	}

	DCELHalfEdge * edge4 = dcel.createEdge(point1, point4);
	DCELHalfEdge * edge5 = dcel.createEdge(point2, point4);
	DCELHalfEdge * edge6 = dcel.createEdge(point3, point4);

	// theortical 4 Faces then!
	// Delete from input set
	if (p4 > p3) {
		points.erase(points.begin() + p4);
		points.erase(points.begin() + p3);
	}
	else {
		points.erase(points.begin() + p3);
		points.erase(points.begin() + p4);
	}
	
	points.erase(points.begin() + p2);
	points.erase(points.begin() + p1);
	// 3. comput random permutation of points p5, p6, …, pn of the remaining points

	// TODO! 

	// 4. Initialize the conflict graph G with all visible pairs (pt, f), where f is a facet of C and t > 4
	ConflictGraph G = ConflictGraph();
	// loop over all points and create conflicts with faces
	for (Point * point : points) {
		ConflictPoint * cP = G.createConflictPoint(point);
		for (DCELFace * face : dcel.surfaces) {
			ConflictFace * cF = G.createConflictFace(face);
			G.checkForConflict(cP, cF);
		}
	}
	
	int run = 0; // TODO DEBUG DELETE 
	//5. for r = 5; r < n; r++		
	for (Point * point : points) {
		//6. Insert pr into C:
		ConflictPoint * cP = G.createConflictPoint(point);
		// 7. if Fconflict(pr) is not empty	// pr lies outside of C
		if (cP->conflicts.size() != 0) {
			vector<DCELHalfEdge *> horizonCandidates;

			for (auto conflict : cP->conflicts) {
				// 8. delete all facets in Fconflict(pr) from C
				DCELHalfEdge * half = dcel.deleteFace(conflict->face->face);
				if (half != nullptr)
					horizonCandidates.push_back(half);
			}

			DCELHalfEdge * horizonStart = nullptr;
			for (auto hE: horizonCandidates) {
				if (hE->twin->face != dcel.openFace) {
					horizonStart = hE;
				}
			}

			// 9. Walk along the boundary of the visible region of pr (which consists exactly of the facets in Fconflict(pr)) and create a list L of horizon edges in order.
			vector<DCELHalfEdge *> horizon;
			int i = 0;
			DCELHalfEdge * tempEdge = horizonStart;
			while (tempEdge->next != horizonStart) {
				horizon.push_back(tempEdge);
				tempEdge = tempEdge->next;
			}
			horizon.push_back(tempEdge);

			DCELVertex * newVertex = dcel.addVertex(point);

			// 10. for all e ∈ L
			//int i = 0;
			for (DCELHalfEdge * e : horizon) {
				//i++;
				// 11. Connect e to pr by creating a triangluar facet f3
				DCELHalfEdge * createdEdge = dcel.createEdge(e->origin, newVertex);
				DCELHalfEdge * createdEdge2 = dcel.createEdge(e->destination(), newVertex);
				// 12. if f is coplanar with its neighbor f‘ along e	// skipped since we need triangles for OpenGL
						//13. merge f and f‘ into one facet, whose conflict list is the same as that of f‘	// skipped since we need triangles for OpenGL
							// 14. else 	// Determine conflicts for f:
				DCELFace * face = e->face;
				// 15. Create a node for f in G.
				ConflictFace * cF = G.createConflictFace(face);
				// 16. Let f1 and f2 be the facets incident to e in the old convex hull.
				DCELFace * f1 = e->oldFace;
				DCELFace * f2 = e->twin->face;
				// 17. P(e) <-- Pconflict(f1) U Pconflict(f2)
				vector<Conflict*> conflictsf1 = G.createConflictFace(f1)->conflicts;
				vector<Conflict*> conflictsf2 = G.createConflictFace(f2)->conflicts;
				vector<ConflictPoint*> pconflictf1Uponflictf2;

				for (Conflict* conflict : conflictsf1) {
					bool isInside = false;
					int i = 0;
					while (!isInside && i < pconflictf1Uponflictf2.size()) {
						isInside = (pconflictf1Uponflictf2[i] == conflict->point);
						i++;
					}
					if (!isInside)
						pconflictf1Uponflictf2.push_back(conflict->point);
				}

				for (Conflict* conflict : conflictsf2) {
					bool isInside = false;
					int i = 0;
					while (!isInside && i < pconflictf1Uponflictf2.size()) {
						isInside = (pconflictf1Uponflictf2[i] == conflict->point);
						i++;
					}
					if (!isInside)
						pconflictf1Uponflictf2.push_back(conflict->point);
				}
				// 18. for all points p ∈ P(e)		
				for (ConflictPoint * p : pconflictf1Uponflictf2) {
					// 19. if f is visible from p
						// add (p,f) to G
					if (p != cP)
						G.checkForConflict(p, cF);
				}
			}
		}
		// 20. 	Delete the node corresponding to pr and the nodes corresponding to the facets in Fconflict(pr) from G, together with their incident arcs		
		G.deleteCorrespondingNodes(cP);
	}

	// 21. return C (convex hull)
	return dcel;
};

#endif // !CONVEX_HULL_H


