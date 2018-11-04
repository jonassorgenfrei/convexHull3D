#pragma once
#ifndef POINT_H
#define POINT_H

class Point {
protected:
	Point() {}
public:
	virtual ~Point() {};

	virtual void print() = 0;;

};


#endif // !POINT_H
