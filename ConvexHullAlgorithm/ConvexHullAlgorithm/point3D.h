#pragma once

#ifndef POINT_3D_H
#define POINT_3D_H

#include "point.h"
#include <iostream>

class Point3D : public Point {
	public:
		Point3D(float x = 0.0, float y = 0.0, float z = 0.0) {
			this->x = x;
			this->y = y;
			this->z = z;
		};

		void print() {
			std::cout << "Point {" << this->x << ", " << this->y << ", " << this->z << "}";

		}

	private:
		float x;
		float y;
		float z;
};


#endif // !POINT_3D_H
