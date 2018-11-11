#pragma once

#ifndef MATH_H
#define MATH_H

#include <math.h>
#include "point.h"

typedef struct Vec2 {
	double x = 0.0;
	double y = 0.0;
} Vec2;

typedef struct Vec3 {
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
} Vec3;

double length2Vec2(Vec2 v) {
	return v.x*v.x + v.y*v.y;
}

double length2Vec3(Vec3 v) {
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

double lengthVec2(Vec2 v) {
	return sqrt(length2Vec2(v));
}

double lengthVec3(Vec3 v) {
	return sqrt(length2Vec3(v));
}

Vec3 normalize(Vec3 v) {
	double lV = lengthVec3(v);
	return {v.x/lV, v.y/lV, v.z/lV};
}

Vec3 addVec3(Vec3 v1, Vec3 v2) {
	return {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

Vec3 subVec3(Vec3 v1, Vec3 v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vec3 scalarMulVec3(double skalar, Vec3 v) {
	return {v.x*skalar, v.y*skalar, v.z*skalar};
}

Vec3 scalarDivVec3(double skalar, Vec3 v) {
	return {v.x/skalar, v.y/skalar, v.z/skalar};
}

bool cmpd(double A, double B, double epsilon = std::numeric_limits<double>::epsilon())
{
	return (fabs(A - B) < epsilon);
}

bool compVec3(Vec3 v1, Vec3 v2) {
	return cmpd(v1.x, v2.x) && cmpd(v1.y, v2.y) && cmpd(v1.z, v2.z);
}

double crossVec2(Vec2 v1, Vec2 v2) {
	return v1.x*v2.y - v1.y*v2.x;
}

Vec3 crossVec3(Vec3 v1, Vec3 v2) {
	Vec3 res = {(v1.y*v2.z-v1.z*v2.y), (v1.z*v2.x - v1.x*v2.z), (v1.x*v2.y - v1.y*v2.x)};
	return res;
};

double dotVec3(Vec3 v1, Vec3 v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

double ccw(Point * p1, Point * p2, Point * p3) {
	return (p2->getX() - p1->getX())*(p3->getY() - p1->getY()) - (p2->getY() - p1->getY())*(p3->getX() - p1->getX());
}


/**
 * @param v - vector to project
 * @param n - plane normal
 * @return vec3 projected on plane described by plane normal n
 */
Vec3 projectVec3onPlane(Vec3 v, Vec3 n) {
	//						   v * n
	// projectplane(v) = v - ---------n
	//					      ||n||^2
	return subVec3(v,scalarMulVec3((dotVec3(v,n)/length2Vec3(n)),n));
}

/*
 * Checks if 3 Points are collinear
 * @paran p1-p3 points to test
 * @return boolean if points are collinear
 */
bool checkCollinear(Point * p1, Point * p2, Point * p3) {
	if (p1->getType() != PointType::POINT_3D ||
		p2->getType() != PointType::POINT_3D ||
		p3->getType() != PointType::POINT_3D) {
		// RETURN ERROR EXCEPTION
	}

	// calc det from Matrix A
	
	//		(p1.x	p2.x	p3.x)
	//	A = (p1.y	p2.y	p3.y)
	//		(p1.z	p2.z	p3.z)
	
	// get det(A) = p1.x*p2.y*p3.z + p2.x*p3.y*p1.z + p3.x*p1.y*p2.z - p3.x*p2.y*p1.z - p2.x*p1.y*p3.z - p1.x*p3.y*p2.z
	double det =	(p1->getX()*p2->getY()*p3->getZ()) +
					(p2->getX()*p3->getY()*p1->getZ()) +
					(p3->getX()*p1->getY()*p2->getZ()) -
					(p3->getX()*p2->getY()*p1->getZ()) -
					(p2->getX()*p1->getY()*p3->getZ()) -
					(p1->getX()*p3->getY()*p2->getZ());

	// collinear if det != 0
	return cmpd(det, 0.0);
}

/*
 * Checks if 3 Points are colinear
 * @paran p1-p3 points to test
 * @return boolean
 */
bool checkCoplanar(Point * p1, Point * p2, Point * p3, Point * p4) {
	if (p1->getType() != PointType::POINT_3D ||
		p2->getType() != PointType::POINT_3D ||
		p3->getType() != PointType::POINT_3D ||
		p4->getType() != PointType::POINT_3D) {

		// RETURN ERROR EXCEPTION
	}
	
	// The points are coplanar, if the vectors connecting one point to the other three points satisfy the scalar 
	// triple product:
	// v1 * (v2 x v3) = 0
	//
	// The vectors: 
	// v1 = (p2.x - p1.x)i + (p2.y - p1.y)j + (p2.z - p1.z)k
	// v2 = (p3.x - p1.x)i + (p3.y - p1.y)j + (p3.z - p1.z)k
	// v3 = (p4.x - p1.x)i + (p4.y - p1.y)j + (p4.z - p1.z)k

	Vec3 v1 = { (p2->getX() - p1->getX()), (p2->getY() - p1->getY()), (p2->getZ() - p1->getZ()) };
	Vec3 v2 = { (p3->getX() - p1->getX()), (p3->getY() - p1->getY()), (p3->getZ() - p1->getZ()) };
	Vec3 v3 = { (p4->getX() - p1->getX()), (p4->getY() - p1->getY()), (p4->getZ() - p1->getZ()) };


	double scalTripleProduct = dotVec3(v1, crossVec3(v2, v3));

	return cmpd(scalTripleProduct, 0.0);
}

#endif // !MATH_H
