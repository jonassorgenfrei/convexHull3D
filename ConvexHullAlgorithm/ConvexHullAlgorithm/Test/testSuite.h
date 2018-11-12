#pragma once

#ifndef TESTSUITE_H
#define TESTsUITE_H

#include <cstdlib>

#include "point3D.h"
#include "point2D.h"
#include "math.h"
#include "dcel.h"

/**
 * Test Suite to check algorithms, libraries & implementations.
 */
class TestSuite {
public:
	TestSuite() {
	}

	/**
	 * Run all tests in Test Suite.
	 */
	void run() {
		mathClassTests();
		convexHull2DTest();
		convexHull3DTest();
		dcelTest();
	}
private:

	/* Tests for the math class */
	void mathClassTests() {
		string testClass = "math.h";
		int testID = 1;

		{
			/* Test-ID:		#1
			 * Class:		math.h
			 * Function:	checkCollinear
			 * check collinear points - collinear
			 */
			Point * point1 = new Point3D(1.0, 1.0, 0.0);
			Point * point2 = new Point3D(1.0, 1.0, 0.0);
			Point * point3 = new Point3D(1.0, 1.0, 0.0);

			errorCheck(checkCollinear(point1, point2, point3), testClass, "checkColinear", testID);
			testID++;
		}

		{
			/* Test-ID:		#2
			 * Class:		math.h
			 * Function:	checkCollinear
			 * check collinear points - not collinear
			 */
			Point * point1 = new Point3D(1.0, 0.0, 0.0);
			Point * point2 = new Point3D(0.0, 1.0, 0.0);
			Point * point3 = new Point3D(0.0, 0.0, 1.0);

			errorCheck(!checkCollinear(point1, point2, point3), testClass, "checkColinear", testID);
			testID++;
		}

		{
			/* Test-ID:		#3
			 * Class:		math.h
			 * Function:	checkCoPlanar
			 * check coplanar points - coplanar
			 */
			Point * point1 = new Point3D(-1.0, 0.0, 1.0);
			Point * point2 = new Point3D(3.0, 2.0, 0.0);
			Point * point3 = new Point3D(1.0, 1.0, 2.0);
			Point * point4 = new Point3D(3.0, 2.0, 4.0);

			errorCheck(checkCoplanar(point1, point2, point3, point4), testClass, "checkColinear", testID);
			testID++;
		}

		{
			/* Test-ID:		#4
			 * Class:		math.h
			 * Function:	checkCoPlanar
			 * check coplanar points - not coplanar
			 */
			Point * point1 = new Point3D(0.0, 1.0, 5.0);
			Point * point2 = new Point3D(4.0, 6.0, 1.0);
			Point * point3 = new Point3D(-2.0, 2.0, 1.0);
			Point * point4 = new Point3D(2.0, -1.0, 0.0);

			errorCheck(!checkCoplanar(point1, point2, point3, point4), testClass, "checkColinear", testID);
			testID++;
		}

		{
			/* Test-ID:		#5
			 * Class:		math.h
			 * Function:	angle
			 * Checks the angle for Vectors
			 */
			Vec3 v1 = {0,1,0};
			Vec3 v2 = {1,0,0};
			Vec3 v3 = {0,-1,0};
			Vec3 v4 = {-1,0,0};
			Vec3 v5 = {0.5,0.5,0.0};
			Vec3 v6 = {-0.5,-0.5,0.0};
			Vec3 n = {0.0, 0.0, 1.0};

			errorCheck(cmpd(angleVec3(v1, v2, n), 270.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v3, n), 180.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v4, n), 90.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v2, v1, n), 90.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v5, v6, n), 180.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v6, n), 135.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v5, n), 315.0), testClass, "angle", testID);

			
			testID++;
		}

		{
			/* Test-ID:		#6
			 * Class:		math.h
			 * Function:	
			 * Checks algorithm for finding nearest edges to new edge
			 */
			Vec3 v1 = { -1,-1,0 };
			Vec3 v2 = { 1,-1,0 };
			Vec3 v3 = { 0,-1,1 };
			Vec3 v4 = { 0,-1,-1 };

			Vec3 n;
			n = addVec3(v1, v2);
			n = addVec3(n, v3);
			n = addVec3(n, v4);
			n = scalarDivVec3(-4, n);
			n = normalize(n);

			/* CHECK NORMAL */
			errorCheck(cmpd(n.y,1.0), testClass, "normal", testID);
			

			/* TODO: !!! */
			
			testID++;
		}

		{
			/* Test-ID:		#7
			 * Class:		math.h
			 * Function:	projectVec3onPlane
			 * Checks algorithm for finding nearest edges to new edge
			 */
			Vec3 v1 = { 1.0, 1.0, 1.0 };
			Vec3 planeNormal = { 0.0, 1.0, 0.0 };
			Vec3 v3 = {1.0, 0.0, 0.0};

			Vec3 projV1 = projectVec3onPlane(v1, planeNormal);
			
			errorCheck(cmpd(angleVec3(projV1, v3, planeNormal),45.0), testClass, "projectVec3onPlane", testID);

			testID++;
		}
	}

	/* Test for the convex hull 2D algorithm implmented in convexHull.h */
	void convexHull2DTest() {
		string testClass = "convexHull.h";
		int testID = 1;
		{
			/* Test-ID:		#1
			 * Class:		convexHull.h
			 * Function:	convexHull2D
			 * Checks convex Hull in 2D
			 */
			vector<Point * > vec;
			vec.push_back(new Point2D(0, 3));
			vec.push_back(new Point2D(1, 1));
			vec.push_back(new Point2D(2, 2));
			vec.push_back(new Point2D(4, 4));
			vec.push_back(new Point2D(0, 0));
			vec.push_back(new Point2D(1, 2));
			vec.push_back(new Point2D(3, 1));
			vec.push_back(new Point2D(3, 3));

			vector<Point *> res = ConvexHull2D(vec);

			errorCheck(res.size() == 4, testClass, "ConvexHull2D", testID);
			errorCheck(cmpd(res[0]->getX(), 0.0) && cmpd(res[0]->getY(), 0.0), testClass, "ConvexHull2D", testID);
			errorCheck(cmpd(res[1]->getX(), 3.0) && cmpd(res[1]->getY(), 1.0), testClass, "ConvexHull2D", testID);
			errorCheck(cmpd(res[2]->getX(), 4.0) && cmpd(res[2]->getY(), 4.0), testClass, "ConvexHull2D", testID);
			errorCheck(cmpd(res[3]->getX(), 0.0) && cmpd(res[3]->getY(), 3.0), testClass, "ConvexHull2D", testID);

			testID++;
		}
	}

	/* Test for the dcel Data structure */
	void dcelTest() {
		string testClass = "dcel.h";
		int testID = 1;
		{
			/* Test-ID:		#1
			 * Class:		dcel.h
			 * Function:	addVertex & getVerticeCount
			 * Adds Points to DCEL and creates new Vertices
			 */
			Point * point1 = new Point3D(0.0, 1.0, 5.0);
			Point * point2 = new Point3D(4.0, 6.0, 1.0);
			Point * point3 = new Point3D(-2.0, 2.0, 1.0);
			Point * point4 = new Point3D(2.0, -1.0, 0.0);

			DCEL dcel = DCEL();
			dcel.addVertex(point1);
			dcel.addVertex(point2);
			dcel.addVertex(point3);
			dcel.addVertex(point4);

			errorCheck(dcel.getVerticeCount() == 4, testClass, "addVertex & getVerticeCount", testID);

			testID++;
		}

		{
			/* Test-ID:		#1
			 * Class:		dcel.h
			 * Function:	createEdges & getEdgeCount & getFaceCount
			 * Adds Points to DCEL and creates new Vertices
			 */
			Point * point1 = new Point3D(0.0, 1.0, 5.0);
			Point * point2 = new Point3D(4.0, 6.0, 1.0);
			Point * point3 = new Point3D(-2.0, 2.0, 1.0);
			Point * point4 = new Point3D(2.0, -1.0, 0.0);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);	// this one should be the outer ONE!

			DCELHalfEdge * h1 = dcel.createEdge(v1, v2);
			DCELHalfEdge * h2 = dcel.createEdge(v2, v3);
			//DCELHalfEdge * h3 = dcel.createEdge(v3, v1);

			//DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			//DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			//DCELHalfEdge * h6 = dcel.createEdge(v3, v4);

			dcel.printDCELInfo();
			dcel.printDCEL();

			errorCheck(dcel.getFaceCount() == 4, testClass, "createEdges & getVerticeCount", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "createEdges & getEdgeCount", testID);

			testID++;
		}
	}

	/* Test for the convex hull 3D algorithm implmented in convexHull.h */
	void convexHull3DTest() {
		string testClass = "convexHull.h";
		int testID = 1;
		{
			/* Test-ID:		#1
			 * Class:		convexHull.h
			 * Function:	convexHull3D
			 * DESCRIPTION
			 */

			testID++;
		}
	}
	

	
	/* ---------- DON'T TOUCH THIS ---------- */

	/*
	 * Returns Error for wrong test and Shows which error went Wrong
	 */
	void errorCheck(bool value, string testClass, string functionName, int testId) {
		if (!value) {
			std::cout << "Error: In class: "
					<< testClass 
					<< ". Function: " 
					<< functionName 
					<< " returns an unexpected value on Test #ID: "
					<< testId
					<< std::endl;

			exit(1);
		}
	}

};

#endif // !TESTSUITE_H