#pragma once

#ifndef MATH_H
#define MATH_H

#ifndef M_PI
#define	M_PI		3.14159265358979323846
#endif // !M_PI

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

inline double length2Vec2(Vec2 v) {
	return v.x*v.x + v.y*v.y;
}

inline double length2Vec3(Vec3 v) {
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

inline double lengthVec2(Vec2 v) {
	return sqrt(length2Vec2(v));
}

inline double lengthVec3(Vec3 v) {
	return sqrt(length2Vec3(v));
}

inline Vec3 normalize(Vec3 v) {
	double lV = lengthVec3(v);
	return {v.x/lV, v.y/lV, v.z/lV};
}

inline Vec3 addVec3(Vec3 v1, Vec3 v2) {
	return {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

inline Vec3 subVec3(Vec3 v1, Vec3 v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline Vec3 scalarMulVec3(double skalar, Vec3 v) {
	return {v.x*skalar, v.y*skalar, v.z*skalar};
}

inline Vec3 scalarDivVec3(double skalar, Vec3 v) {
	return {v.x/skalar, v.y/skalar, v.z/skalar};
}

inline bool cmpd(double A, double B, double epsilon = std::numeric_limits<double>::epsilon())
{
	return (fabs(A - B) < epsilon);
}

inline bool compVec3(Vec3 v1, Vec3 v2) {
	return cmpd(v1.x, v2.x) && cmpd(v1.y, v2.y) && cmpd(v1.z, v2.z);
}

inline double crossVec2(Vec2 v1, Vec2 v2) {
	return v1.x*v2.y - v1.y*v2.x;
}

inline Vec3 crossVec3(Vec3 v1, Vec3 v2) {
	Vec3 res = {(v1.y*v2.z-v1.z*v2.y), (v1.z*v2.x - v1.x*v2.z), (v1.x*v2.y - v1.y*v2.x)};
	return res;
};

inline double dotVec3(Vec3 v1, Vec3 v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline double ccw(Point * p1, Point * p2, Point * p3) {
	return (p2->getX() - p1->getX())*(p3->getY() - p1->getY()) - (p2->getY() - p1->getY())*(p3->getX() - p1->getX());
}

/* Clockwise Angle */
inline double angleVec3(Vec3 v1, Vec3 v2, Vec3 normal) {

	double det = dotVec3(normal, crossVec3(v1, v2));
	double angle = atan2(det, dotVec3(v1, v2)) * 180.0 / M_PI;
	if (angle < 0.0) {
		angle = 360.0 + angle;
	}
	return angle;
}

/**
 * @param v - vector to project
 * @param n - plane normal
 * @return vec3 projected on plane described by plane normal n
 */
inline Vec3 projectVec3onPlane(Vec3 v, Vec3 n) {
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
inline bool checkCollinear(Point * p1, Point * p2, Point * p3) {
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
inline bool checkCoplanar(Point * p1, Point * p2, Point * p3, Point * p4) {
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

#endif // !MATHTEST_H
