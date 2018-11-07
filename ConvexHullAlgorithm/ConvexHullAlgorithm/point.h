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
	virtual float getX() = 0;
	virtual float getY() = 0;
	virtual float getZ() = 0;

	/* Get Type */
	virtual PointType getType() = 0;

};


#endif // !POINT_H
