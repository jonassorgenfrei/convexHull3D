#pragma once

#ifndef POINT_3D_H
#define POINT_3D_H

#include "point.h"
#include <iostream>

class Point3D : public Point {
	public:
		Point3D(double x = 0.0, double y = 0.0, double z = 0.0) {
			this->x = x;
			this->y = y;
			this->z = z;
		};

		void print() {
			std::cout << "Point {" << this->x << ", " << this->y << ", " << this->z << "}";

		}

		double getX() {
			return this->x;
		};

		double getY() {
			return this->y;
		};

		double getZ() {
			return this->z;
		};
		PointType getType() {
			return PointType::POINT_3D;
		}
	private:
		double x;
		double y;
		double z;
};


#endif // !POINT_3D_H
