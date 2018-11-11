#pragma once

#include <cstdlib>

#include "point3D.h"
#include "point2D.h"
#include "math.h"
#include "dcel.h"

class TestSuite {
public:
	TestSuite() {

	}

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
			/* Test-ID:		#3
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
	}

	void convexHull2DTest() {
		string testClass = "convexHull.h";
		int testID = 1;
		{
			/* Test-ID:		#1
			 * Class:		convexHull.h
			 * Function:	convexHull2D
			 * DESCRIPTION
			 */
			
			testID++;
		}
	}

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
	
	void dcelTest() {
		string testClass = "dcel.h";
		int testID = 1;
		{
			/* Test-ID:		#1
			 * Class:		dcel.h
			 * Function:	addVertex & getVerticeCount
			 * Adds Points to DCEL and creates new Vertices
			 */
			Point * point1 = new Point3D( 0.0,  1.0, 5.0);
			Point * point2 = new Point3D( 4.0,  6.0, 1.0);
			Point * point3 = new Point3D(-2.0,  2.0, 1.0);
			Point * point4 = new Point3D( 2.0, -1.0, 0.0);

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
			DCELHalfEdge * h3 = dcel.createEdge(v3, v1);

			DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			DCELHalfEdge * h6 = dcel.createEdge(v3, v4);

			//errorCheck(dcel.getFaceCount() == 4, testClass, "createEdges & getVerticeCount", testID);
			//errorCheck(dcel.getEdgeCount() == 6, testClass, "createEdges & getEdgeCount", testID);

			testID++;
		}
	}


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