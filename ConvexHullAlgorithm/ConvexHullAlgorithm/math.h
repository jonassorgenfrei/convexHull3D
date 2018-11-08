#pragma once

#ifndef MATH_H
#define MATH_H

#include <math.h>
#include "point.h"

typedef struct Vec2 {
	double x;
	double y;
} Vec2;

typedef struct Vec3 {
	double x;
	double y;
	double z;
} Vec3;

double crossVec2(Vec2 v1, Vec2 v2) {
	return v1.x*v2.y - v1.y*v2.x;
};

double length2Vec2(Vec2 v) {
	return v.x*v.x + v.y*v.y;
}

double lengthVec2(Vec2 v) {
	return sqrt(length2Vec2(v));
}

double ccw(Point * p1, Point * p2, Point * p3) {
	return (p2->getX() - p1->getX())*(p3->getY() - p1->getY()) - (p2->getY() - p1->getY())*(p3->getX() - p1->getX());
}

bool cmpf(double A, double B, double epsilon = std::numeric_limits<double>::epsilon())
{
	return (fabs(A - B) < epsilon);
}

#endif // !MATH_H
