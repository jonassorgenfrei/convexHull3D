#pragma once

#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <vector>
#include <algorithm>


#include "point.h"
#include "dcel.h"
#include "visualisation.h"
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
	std::vector<Point*> ch;

	// Select the Point with minimum y 
	float y_min = points[0]->getY();
	Point * min_Y_Point = points[0];
	int idx_min = 0;

	for (int i = 0; i < points.size(); i++) {
		if (y_min > points[i]->getY()) {
			y_min = points[i]->getY();
			min_Y_Point = points[i];
			idx_min = i;
		}

	}

	//Swap point [0] and lowestPoint
	points[idx_min] = points[0];
	points[0] = min_Y_Point;


	// Sort all points in CCW order {p0, p1, .. pn}
	std::sort(points.begin(), points.end(), [min_Y_Point](const auto& lhs, const auto& rhs)
	{
		Vec2 vec1 = {rhs->getX() - min_Y_Point->getX(), rhs->getY() - min_Y_Point->getY() };
		Vec2 vec2 = {lhs->getX() - min_Y_Point->getX(), lhs->getY() - min_Y_Point->getY() };

		float cross = crossVec2(vec1, vec2);

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

	// S = {p0, p1}
	ch.push_back(points[0]);
	ch.push_back(points[1]);
	ch.push_back(points[2]);


	for (int i = 3; i < points.size(); i++) {
		while (ch.size() > 2 && ccw(ch[ch.size() -2 ], ch[ch.size()-1], points[i]) < 0) {
			ch.pop_back();
		}
		ch.push_back(points[i]);
	}


	return ch;
}

/*
 * Convex Hull Algorithm in 3D Space
 */
DCEL ConvexHull3D(std::vector<Point*> points) {
	DCEL dcel;

	/*
	
	Algorithm Convex Hull(P)
	Input: A set P of n points (x,y,z) in three-space. 
	Output: The convex hull CH(P) of P.
	1.	Find 4 points p1, p2, p3, p4 in P that form a tetrahedron.
	2.	C  CH({p1, p2, p3, p4})
	3.	Compute a random permutation p5, p6, …, pn of the remaining points
	4.	Initialize the conflict graph G with all visibile pairs (pt, f), where f is a facet of C and t > 4
	5.	for r = 5; r < n; r++
	6.		do 	// Insert pr into C:
	7.			if Fconflict(pr) is not empty	// pr lies outside of C
	8.				then Delete all facets in Fconflict(pr) from C
	9.	Walk along the boundary of the visible region of pr (which consists exactly of the facets in Fconflict(pr)) and create a list L of horizon edges in order.
	10.					for all e ∈ L
	11.						do Connect e to pr by creating a triangluar facet f
	12.							if f is coplanar with its neighbor f‘ along e
	13.		then merge f and f‘ into one facet, whose conflict list is the same as that of f‘
	14.							else 	// Determine conflicts for f:
	15.								Create a node for f in G.
	16.	Let f1 and f2 be the facets incident to e in the old convex hull.
	17.								P(e)  Pconflict(f1) U Pconflict(f2)
	18.								for all points p ∈ P(e)
	19.									do if f is visible from p, 
	add (p,f) to G
	20.	Delete the node corresponding to pr and the nodes corresponding to the facets in Fconflict(pr) from G, together with their incident arcs
	21.	return C

	
	
	*/
	return dcel;
};

#endif // !CONVEX_HULL_H


