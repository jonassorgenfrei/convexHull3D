#pragma once

#ifndef POINT_H
#define POINT_H

typedef enum PointType {POINT_3D, POINT_2D} PointType;

/*
 * Interface for Point
 */
class Point {
protected:
	Point() {}
public:
	virtual ~Point() {};

	virtual void print() = 0;

	/* Get Values */
	virtual double getX() = 0;
	virtual double getY() = 0;
	virtual double getZ() = 0;

	/* Get Type */
	virtual PointType getType() = 0;

	/* Checks if 2 Points are equal (floating point with epsilon) */
	virtual bool isEqual(Point * p) = 0;

};

#endif // !POINT_H
