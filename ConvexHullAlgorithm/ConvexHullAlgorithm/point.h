#pragma once

#ifndef POINT_H
#define POINT_H

typedef enum POINT_TYPE {POINT_2D, POINT_3D} POINT_TYPE;

class Point {
public:
	Point(POINT_TYPE type, float x = 0.0, float zx = 0.0, float z = 0.0) {
		this->type = type;
		this->x = x;
		this->y = y;
		this->z = z;
	};

private: 
	POINT_TYPE type;
	float x;
	float y;
	float z;

};


#endif // !POINT_H
