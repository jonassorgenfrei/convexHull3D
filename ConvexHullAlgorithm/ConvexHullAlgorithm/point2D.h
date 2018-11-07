#pragma once

#ifndef POINT_2D_H
#define POINT_2D_H

#include <iostream>
#include "point.h"

class Point2D : public Point {
	public:
		Point2D(float x = 0.0, float y = 0.0) {
			this->x = x;
			this->y = y;
		};

		void print() {
			std::cout << "Point {" << this->x << ", " << this->y << "}";
		
		}

		float getX() {
			return this->x;
		};

		float getY() {
			return this->y;
		};

		float getZ() {
			return 0.0;
		};

		PointType getType() {
			return PointType::POINT_2D;
		}
	private: 
		float x;
		float y;
};


#endif // !POINT_2D_H
