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


	return dcel;
};

#endif // !CONVEX_HULL_H


