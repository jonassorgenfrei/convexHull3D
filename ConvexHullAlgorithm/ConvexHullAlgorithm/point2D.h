#pragma once

#ifndef POINT_2D_H
#define POINT_2D_H

#include <iostream>
#include "point.h"

class Point2D : public Point {
	public:
		Point2D(double x = 0.0, double y = 0.0) {
			this->x = x;
			this->y = y;
		};

		void print() {
			std::cout << "Point (" << this->x << "; " << this->y << ")";
		}

		double getX() {
			return this->x;
		};

		double getY() {
			return this->y;
		};

		double getZ() {
			return 0.0;
		};

		PointType getType() {
			return PointType::POINT_2D;
		}
	private: 
		double x;
		double y;
};


#endif // !POINT_2D_H
