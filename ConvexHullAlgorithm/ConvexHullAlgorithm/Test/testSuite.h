#pragma once
/*
 *	Testsuite
 *	Testsuite to test sub algorithms & data structures for the main program
 *
 * Part of the seminar talk at FH Wedel 2018
 *
 * @author -	Jonas Sorgenfrei
 *				Minf101767
 */


#ifndef TESTSUITE_H
#define TESTsUITE_H

#include <cstdlib>

#include "point3D.h"
#include "point2D.h"
#include "math.h"
#include "dcel.h"
#include "dcelHalfEdge.h"
#include "conflictGraph.h"

/**
 * Test Suite to check algorithms, libraries & implementations.
 */
class TestSuite {
public:
	// constructor
	TestSuite() {
	}

	/**
	 * Run all tests in Test Suite.
	 */
	void run() {
		/* ---------- DON'T TOUCH THIS ---------- */
		std::cout << "--- TESTS Start ---" << std::endl;
		/* -------------------------------------- */

		/* Run Test Collections */

		mathClassTests();
		convexHull2DTest();
		dcelTest();
		conflictGraphTest();
		convexHull3DTest();

		/* ---------- DON'T TOUCH THIS ---------- */
		std::cout << "--- TESTS SUCCEEDED ---" << std::endl;
		/* -------------------------------------- */
	}
private:
	/* ---------- Add Test Collections as Functions ---------- */

	/* Tests for the math class */
	void mathClassTests() {
		string testClass = "math.h";
		int testID = 1;
		std::cout << "RUN: mathClassTests" << std::endl;
		/* Test-ID:		#1
		 * Class:		math.h
		 * Function:	checkCollinear
		 * check collinear points - collinear
		 */
		{
			Point * point1 = new Point3D(1.0, 1.0, 0.0);
			Point * point2 = new Point3D(1.0, 1.0, 0.0);
			Point * point3 = new Point3D(1.0, 1.0, 0.0);

			errorCheck(checkCollinear(point1, point2, point3), testClass, "checkColinear", testID);
			testID++;
		}

		/* Test-ID:		#2
		 * Class:		math.h
		 * Function:	checkCollinear
		 * check collinear points - not collinear
		 */
		{
			
			Point * point1 = new Point3D(1.0, 0.0, 0.0);
			Point * point2 = new Point3D(0.0, 1.0, 0.0);
			Point * point3 = new Point3D(0.0, 0.0, 1.0);

			errorCheck(!checkCollinear(point1, point2, point3), testClass, "checkColinear", testID);
			testID++;
		}

		/* Test-ID:		#3
		 * Class:		math.h
		 * Function:	checkCoPlanar
		 * check coplanar points - coplanar
		 */
		{
			
			Point * point1 = new Point3D(-1.0, 0.0, 1.0);
			Point * point2 = new Point3D(3.0, 2.0, 0.0);
			Point * point3 = new Point3D(1.0, 1.0, 2.0);
			Point * point4 = new Point3D(3.0, 2.0, 4.0);

			errorCheck(checkCoplanar(point1, point2, point3, point4), testClass, "checkColinear", testID);
			testID++;
		}

		/* Test-ID:		#4
		 * Class:		math.h
		 * Function:	checkCoPlanar
		 * check coplanar points - not coplanar
		 */
		{
			
			Point * point1 = new Point3D(0.0, 1.0, 5.0);
			Point * point2 = new Point3D(4.0, 6.0, 1.0);
			Point * point3 = new Point3D(-2.0, 2.0, 1.0);
			Point * point4 = new Point3D(2.0, -1.0, 0.0);

			errorCheck(!checkCoplanar(point1, point2, point3, point4), testClass, "checkColinear", testID);
			testID++;
		}

		/* Test-ID:		#5
		 * Class:		math.h
		 * Function:	angle
		 * Checks the angle for Vectors
		 */
		{
			
			Vec3 v1 = {0,0,1};
			Vec3 v2 = {1,0,0};
			Vec3 v3 = {0,0,-1};
			Vec3 v4 = {-1,0,0};
			Vec3 v5 = {0.5,0,0.5};
			Vec3 v6 = {-0.5,0,-0.5};
			Vec3 n = {0.0, 1.0, .0};

			errorCheck(cmpd(angleVec3(v1, v2, n), 90.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v3, n), 180.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v4, n), 270.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v2, v1, n), 270.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v5, v6, n), 180.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v6, n), 225.0), testClass, "angle", testID);
			errorCheck(cmpd(angleVec3(v1, v5, n), 45.0), testClass, "angle", testID);

			
			testID++;
		}

		/* Test-ID:		#6
		 * Class:		math.h
		 * Function:
		 * Checks algorithm for finding nearest edges to new edge
		 */
		{
			
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

		/* Test-ID:		#7
		 * Class:		math.h
		 * Function:	projectVec3onPlane
		 * Checks algorithm for finding nearest edges to new edge
		 */
		{
			
			Vec3 v1 = { 1.0, 1.0, 1.0 };
			Vec3 planeNormal = { 0.0, 1.0, 0.0 };
			Vec3 v3 = {1.0, 0.0, 0.0};

			Vec3 projV1 = projectVec3onPlane(v1, planeNormal);
			
			errorCheck(cmpd(angleVec3(projV1, v3, planeNormal),45.0), testClass, "projectVec3onPlane", testID);

			testID++;
		}
		std::cout << "Passed: mathClassTests" << std::endl;
	}

	/* Test for the convex hull 2D algorithm implmented in convexHull.h */
	void convexHull2DTest() {
		string testClass = "convexHull.h";
		int testID = 1;

		std::cout << "RUN: convexHull2DTest" << std::endl;

		/* Test-ID:		#1
		 * Class:		convexHull.h
		 * Function:	convexHull2D
		 * Checks convex Hull in 2D
		 */
		{
			
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

		std::cout << "Passed: convexHull2DTest" << std::endl;
	}

	/* Test for the dcel Data structure */
	void dcelTest() {
		string testClass = "dcel.h";
		int testID = 1;
		
		std::cout << "RUN: dcelTest" << std::endl;

		/* Test-ID:		#1
		 * Class:		dcel.h
		 * Function:	addVertex & getVerticeCount
		 * Adds Points to DCEL and creates new Vertices
		 */
		{
			
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

		/* Test-ID:		#2
		 * Class:		dcel.h
		 * Function:	isConnected
		 * Checks the Function is connected
		 */
		{
			
			Point * point1 = new Point3D(0.0, 1.0, 5.0);
			Point * point2 = new Point3D(4.0, 6.0, 1.0);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);

			errorCheck(!dcel.isConnected(v1, v2), testClass, "isConnected", testID);
			errorCheck(!dcel.isConnected(v2, v1), testClass, "isConnected", testID);

			DCELHalfEdge * h1 = dcel.createEdge(v1, v2);

			errorCheck(dcel.isConnected(v1, v2), testClass, "isConnected", testID);
			errorCheck(dcel.isConnected(v2, v1), testClass, "isConnected", testID);

			testID++;
		}

		/* Test-ID:		#3
		 * Class:		dcel.h
		 * Function:	createEdges
		 * Adds Points to DCEL and creates new Vertices
		 */
		{
			
			Point * point1 = new Point3D(0.0, 0.0, 0.0);
			Point * point2 = new Point3D(-1.0, -1.0, 0.0);
			Point * point3 = new Point3D(0.0, -1.0, 1.0);
			Point * point4 = new Point3D(1.0, -1.0, 0.0);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);

			DCELHalfEdge * h1 = dcel.createEdge(v2, v1);
			DCELHalfEdge * h2 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h3 = dcel.createEdge(v1, v3);
			DCELHalfEdge * h4 = dcel.createEdge(v3, v2);

			errorCheck(h1 == h1->next->next->next, testClass, "createEdges", testID);
			errorCheck(h1->twin->next->next->next == h2, testClass, "createEdges", testID);

			testID++;
		}
	
		/* Test-ID:		#4
		 * Class:		dcel.h
		 * Function:	createEdges & getEdgeCount & getFaceCount
		 * Adds Points to DCEL and creates new Vertices
		 */
		{
			
			Point * point1 = new Point3D(0.0, 1.0, 5.0);
			Point * point2 = new Point3D(4.0, 6.0, 1.0);
			Point * point3 = new Point3D(-2.0, 2.0, 1.0);
			Point * point4 = new Point3D(2.0, -1.0, 0.0);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);

			Vec3 a = {point2->getX() -point1->getX(),
					  point2->getY() - point1->getY(),
			          point2->getZ() - point1->getZ()
					};
			Vec3 b = { point3->getX() - point1->getX(),
					  point3->getY() - point1->getY(),
					  point3->getZ() - point1->getZ() };

			Vec3 c = { point4->getX() - point1->getX(),
					  point4->getY() - point1->getY(),
					  point4->getZ() - point1->getZ() };
			
			double orientation = dotVec3(normalize(c), normalize(crossVec3(a, b)));
			// if orientation == 0, points are coplanar!!
			DCELHalfEdge * h1;
			DCELHalfEdge * h2;
			DCELHalfEdge * h3;

			if (orientation > 0) {
				h1 = dcel.createEdge(v1, v2);
				h2 = dcel.createEdge(v2, v3);
				h3 = dcel.createEdge(v3, v1);
			}
			else {
				h1 = dcel.createEdge(v1, v3);
				h2 = dcel.createEdge(v3, v2);
				h3 = dcel.createEdge(v2, v1);
			}
			DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			DCELHalfEdge * h6 = dcel.createEdge(v3, v4);

			//dcel.printDCELInfo();
			//dcel.printDCEL();

			errorCheck(dcel.getFaceCount() == 4, testClass, "createEdges & getVerticeCount", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "createEdges & getEdgeCount", testID);

			testID++;
		}

		/* Test-ID:		#5
		 * Class:		dcel.h
		 * Function:	createEdges & getEdgeCount & getFaceCount
		 * Adds Points to DCEL and creates new Vertices
		 */
		{
			
			Point * point1 = new Point3D(12.6847, 10.3433, 12.5895);
			Point * point2 = new Point3D(10.9549, 18.2318, 6.22456);
			Point * point3 = new Point3D(11.9449, 18.1762, 1.83477);
			Point * point4 = new Point3D(16.2151, 9.76775, 10.7395);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);

			Vec3 a = { point2->getX() - point1->getX(),
					  point2->getY() - point1->getY(),
					  point2->getZ() - point1->getZ()
			};
			Vec3 b = { point3->getX() - point1->getX(),
					  point3->getY() - point1->getY(),
					  point3->getZ() - point1->getZ() };

			Vec3 c = { point4->getX() - point1->getX(),
					  point4->getY() - point1->getY(),
					  point4->getZ() - point1->getZ() };

			double orientation = dotVec3(normalize(c), normalize(crossVec3(a, b)));

			DCELHalfEdge * h1;
			DCELHalfEdge * h2;
			DCELHalfEdge * h3;

			if (orientation > 0) {
				h1 = dcel.createEdge(v1, v2);
				h2 = dcel.createEdge(v2, v3);
				h3 = dcel.createEdge(v3, v1);
			}
			else {
				h1 = dcel.createEdge(v1, v3);
				h2 = dcel.createEdge(v3, v2);
				h3 = dcel.createEdge(v2, v1);
			}
			DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			DCELHalfEdge * h6 = dcel.createEdge(v3, v4);

			errorCheck(dcel.getFaceCount() == 4, testClass, "createEdges & getVerticeCount", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "createEdges & getEdgeCount", testID);

			testID++;
		}

		/* Test-ID:		#6
		 * Class:		dcel.h
		 * Function:	createEdges & getEdgeCount & getFaceCount
		 * Adds Points to DCEL and creates new Vertices
		 */
		{
			
			Point * point1 = new Point3D(14.1697, 17.7721, 15.6413);
			Point * point2 = new Point3D(12.8306, 8.39137, 11.7557);
			Point * point3 = new Point3D(7.21335, 7.10898, 9.85198);
			Point * point4 = new Point3D(0.615253, 2.51289, 10.7682);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);

			Vec3 a = { point2->getX() - point1->getX(),
					  point2->getY() - point1->getY(),
					  point2->getZ() - point1->getZ()
			};
			Vec3 b = { point3->getX() - point1->getX(),
					  point3->getY() - point1->getY(),
					  point3->getZ() - point1->getZ() };

			Vec3 c = { point4->getX() - point1->getX(),
					  point4->getY() - point1->getY(),
					  point4->getZ() - point1->getZ() };

			double orientation = dotVec3(normalize(c), normalize(crossVec3(a, b)));
			// if orientation == 0, points are coplanar!!
			DCELHalfEdge * h1;
			DCELHalfEdge * h2;
			DCELHalfEdge * h3;

			if (orientation > 0) {
				h1 = dcel.createEdge(v1, v2);
				h2 = dcel.createEdge(v2, v3);
				h3 = dcel.createEdge(v3, v1);
			}
			else {
				h1 = dcel.createEdge(v1, v3);
				h2 = dcel.createEdge(v3, v2);
				h3 = dcel.createEdge(v2, v1);
			}
			DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			DCELHalfEdge * h6 = dcel.createEdge(v3, v4);

			errorCheck(dcel.getFaceCount() == 4, testClass, "createEdges & getVerticeCount", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "createEdges & getEdgeCount", testID);

			testID++;
		}

		/* Test-ID:		#7
		 * Class:		dcel.h
		 * Function:	deleteFace
		 * Deletes a Face of the DCEL
		 */
		{

			Point * point1 = new Point3D( 100.0,   0.0,   0.0);
			Point * point2 = new Point3D(   0.0, 100.0,   0.0);
			Point * point3 = new Point3D(   0.0,   0.0, 100.0);
			Point * point4 = new Point3D(-100.0,   0.0,   0.0);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);

			Vec3 a = { point2->getX() - point1->getX(),
					  point2->getY() - point1->getY(),
					  point2->getZ() - point1->getZ()
			};
			Vec3 b = { point3->getX() - point1->getX(),
					  point3->getY() - point1->getY(),
					  point3->getZ() - point1->getZ() };

			Vec3 c = { point4->getX() - point1->getX(),
					  point4->getY() - point1->getY(),
					  point4->getZ() - point1->getZ() };

			double orientation = dotVec3(normalize(c), normalize(crossVec3(a, b)));
			// if orientation == 0, points are coplanar!!
			DCELHalfEdge * h1;
			DCELHalfEdge * h2;
			DCELHalfEdge * h3;

			if (orientation > 0) {

				h1 = dcel.createEdge(v1, v2);
				h2 = dcel.createEdge(v2, v3);
				h3 = dcel.createEdge(v3, v1);
			}
			else {
				h1 = dcel.createEdge(v1, v3);
				h2 = dcel.createEdge(v3, v2);
				h3 = dcel.createEdge(v2, v1);
			}
			DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			DCELHalfEdge * h6 = dcel.createEdge(v3, v4);
			
			DCELHalfEdge * deleteHalfEdge = dcel.deleteFace(dcel.surfaces[0]); // Delete 1. triangle

			errorCheck(dcel.getFaceCount() == 3, testClass, "deleteFace Face count 3", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "deleteFace Edge count 12", testID);
			errorCheck(deleteHalfEdge->face == dcel.openFace, testClass, "deleteFace", testID);

			DCELFace * wrongFace = new DCELFace();
			DCELHalfEdge * nullPtr = dcel.deleteFace(wrongFace);
			errorCheck(dcel.getFaceCount() == 3, testClass, "deleteFace Face count 3", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "deleteFace Edge count 12", testID);
			errorCheck(nullPtr == nullPtr, testClass, "deleteFace", testID);

			deleteHalfEdge = dcel.deleteFace(dcel.surfaces[0]); // Delete 2. triangle

			errorCheck(dcel.getFaceCount() == 2, testClass, "deleteFace Face count 2", testID);
			errorCheck(dcel.getEdgeCount() == 10, testClass, "deleteFace Edeg count 10", testID);
			errorCheck(dcel.getVerticeCount() == 4, testClass, "deleteFace Vert count 4", testID);
			errorCheck(deleteHalfEdge->face == dcel.openFace, testClass, "deleteFace", testID);
			errorCheck(v1->leavingEdges().size() == 2, testClass, "deleteFace v1 leaving 2", testID);
			errorCheck(v2->leavingEdges().size() == 2, testClass, "deleteFace v2 leaving 2", testID);		
			errorCheck(v3->leavingEdges().size() == 3, testClass, "deleteFace v3 leaving 3", testID);
			errorCheck(v4->leavingEdges().size() == 3, testClass, "deleteFace v4 leaving 4", testID);
			errorCheck(h4->twin->face == dcel.openFace, testClass, "deleteFace h4->twin face openface", testID);

			deleteHalfEdge = dcel.deleteFace(dcel.surfaces[0]); // Delete 3. triangle

			errorCheck(dcel.getFaceCount() == 1, testClass, "deleteFace Face 1", testID);
			errorCheck(dcel.getEdgeCount() == 6, testClass, "deleteFace Edge count 6", testID);
			errorCheck(dcel.getVerticeCount() == 3, testClass, "deleteFace Vert count 3", testID);
			errorCheck(deleteHalfEdge != nullptr, testClass, "deleteFace", testID);
			deleteHalfEdge = dcel.deleteFace(dcel.surfaces[0]); // Delete 4. triangle

			errorCheck(dcel.getFaceCount() == 0, testClass, "deleteFace", testID);
			errorCheck(dcel.getEdgeCount() == 0, testClass, "deleteFace", testID);
			errorCheck(dcel.getVerticeCount() == 0, testClass, "deleteFace", testID);
			errorCheck(deleteHalfEdge == nullptr, testClass, "deleteFace", testID);
			testID++;
		}


		/* Test-ID:		#8
		 * Class:		dcel.h
		 * Function:	createEdges
		 * All edges connected to faces
		 */
		{

			Point * point1 = new Point3D(14.1697, 17.7721, 15.6413);
			Point * point2 = new Point3D(12.8306, 8.39137, 11.7557);
			Point * point3 = new Point3D(7.21335, 7.10898, 9.85198);
			Point * point4 = new Point3D(0.615253, 2.51289, 10.7682);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);

			Vec3 a = { point2->getX() - point1->getX(),
					  point2->getY() - point1->getY(),
					  point2->getZ() - point1->getZ()
			};
			Vec3 b = { point3->getX() - point1->getX(),
					  point3->getY() - point1->getY(),
					  point3->getZ() - point1->getZ() };

			Vec3 c = { point4->getX() - point1->getX(),
					  point4->getY() - point1->getY(),
					  point4->getZ() - point1->getZ() };

			double orientation = dotVec3(normalize(c), normalize(crossVec3(a, b)));
			// if orientation == 0, points are coplanar!!
			DCELHalfEdge * h1;
			DCELHalfEdge * h2;
			DCELHalfEdge * h3;

			if (orientation > 0) {
				h1 = dcel.createEdge(v1, v2);
				h2 = dcel.createEdge(v2, v3);
				h3 = dcel.createEdge(v3, v1);
			}
			else {
				h1 = dcel.createEdge(v1, v3);
				h2 = dcel.createEdge(v3, v2);
				h3 = dcel.createEdge(v2, v1);
			}
			DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			DCELHalfEdge * h6 = dcel.createEdge(v3, v4);

			errorCheck(h1->face != nullptr && h1->face  != dcel.openFace, testClass, "createEdges", testID);
			errorCheck(h2->face != nullptr && h2->face != dcel.openFace, testClass, "createEdges", testID);
			errorCheck(h3->face != nullptr && h3->face != dcel.openFace, testClass, "createEdges", testID);
			errorCheck(h4->face != nullptr && h4->face != dcel.openFace, testClass, "createEdges", testID);
			errorCheck(h5->face != nullptr && h5->face != dcel.openFace, testClass, "createEdges", testID);
			errorCheck(h6->face != nullptr && h6->face != dcel.openFace, testClass, "createEdges", testID);
			testID++;
		}

		std::cout << "PASSED: dcelTest" << std::endl;
	}

	/* Test for the Conflict Graph */
	void conflictGraphTest() {
		string testClass = "conflictGraph.h";
		int testID = 1;

		std::cout << "RUN: conflictGraphTest" << std::endl;

		/* Test-ID:		#1
		 * Class:		conflictGraph.h
		 * Function:	checkForConflict
		 * Check if Conflict Detection works fine
		 */
		{

			Point * point1 = new Point3D(-100.0, 0.0, 0.0);
			Point * point2 = new Point3D(100.0, 0.0, 0.0);
			Point * point3 = new Point3D(0.0, 100.0, 0.0);
			Point * point4 = new Point3D(0.0, 0.0, 100.0);

			Point * point5 = new Point3D(0.0, 25.0, 25.0);
			Point * point6 = new Point3D(0.0, 25.0, -25.0);
			Point * point7 = new Point3D(0.0, 25.0, 200.0);
			Point * point8 = new Point3D(-10.0, -200.0, 700.0);

			DCEL dcel = DCEL();
			DCELVertex * v1 = dcel.addVertex(point1);
			DCELVertex * v2 = dcel.addVertex(point2);
			DCELVertex * v3 = dcel.addVertex(point3);
			DCELVertex * v4 = dcel.addVertex(point4);

			Vec3 a = { point2->getX() - point1->getX(),
					  point2->getY() - point1->getY(),
					  point2->getZ() - point1->getZ()
			};
			Vec3 b = { point3->getX() - point1->getX(),
					  point3->getY() - point1->getY(),
					  point3->getZ() - point1->getZ() };

			Vec3 c = { point4->getX() - point1->getX(),
					  point4->getY() - point1->getY(),
					  point4->getZ() - point1->getZ() };

			double orientation = dotVec3(normalize(c), normalize(crossVec3(a, b)));
			// if orientation == 0, points are coplanar!!
			DCELHalfEdge * h1;
			DCELHalfEdge * h2;
			DCELHalfEdge * h3;

			if (orientation > 0) {
				h1 = dcel.createEdge(v1, v2);
				h2 = dcel.createEdge(v2, v3);
				h3 = dcel.createEdge(v3, v1);
			}
			else {
				h1 = dcel.createEdge(v1, v3);
				h2 = dcel.createEdge(v3, v2);
				h3 = dcel.createEdge(v2, v1);
			}
			DCELHalfEdge * h4 = dcel.createEdge(v1, v4);
			DCELHalfEdge * h5 = dcel.createEdge(v2, v4);
			DCELHalfEdge * h6 = dcel.createEdge(v3, v4);
			
			ConflictGraph G = ConflictGraph();
			ConflictPoint * p1 = G.createConflictPoint(point5);
			ConflictPoint * p2 = G.createConflictPoint(point6);
			ConflictPoint * p3 = G.createConflictPoint(point7);
			ConflictPoint * p4 = G.createConflictPoint(point8);

			for (DCELFace * face : dcel.surfaces) {
				ConflictFace * cF = G.createConflictFace(face);
				G.checkForConflict(p1, cF);
				G.checkForConflict(p2, cF);
				G.checkForConflict(p3, cF);
				G.checkForConflict(p4, cF);
			}

			errorCheck(p1->conflicts.size() == 0, testClass, "checkForConflict 0 Conflicts", testID);
			errorCheck(p2->conflicts.size() == 1, testClass, "checkForConflict 1 Conflict", testID);
			errorCheck(p3->conflicts.size() == 2, testClass, "checkForConflict 2 Conflict", testID);
			errorCheck(p4->conflicts.size() == 3, testClass, "checkForConflict 3 Conflict", testID);

			testID++;
		}


		/* Test-ID:		#1
		 * Class:		conflictGraph.h
		 * Function:	deleteCorrespondingNodes
		 * deleteCorrespondingNodes 
		 */
		{
			ConflictGraph G = ConflictGraph();
			ConflictPoint * p1 = new ConflictPoint(new Point3D());
			G.conflictPoints.push_back(p1);
			ConflictPoint * p2 = new ConflictPoint(new Point3D());
			G.conflictPoints.push_back(p2);
			ConflictPoint * p3 = new ConflictPoint(new Point3D());
			G.conflictPoints.push_back(p3);
			ConflictPoint * p4 = new ConflictPoint(new Point3D());
			G.conflictPoints.push_back(p4);
			ConflictPoint * p5 = new ConflictPoint(new Point3D());
			G.conflictPoints.push_back(p5);
			ConflictPoint * p6 = new ConflictPoint(new Point3D());
			G.conflictPoints.push_back(p6);


			ConflictFace * f1 = new ConflictFace(new DCELFace());
			G.conflictFaces.push_back(f1);
			ConflictFace * f2 = new ConflictFace(new DCELFace());
			G.conflictFaces.push_back(f2);
			ConflictFace * f3 = new ConflictFace(new DCELFace());
			G.conflictFaces.push_back(f3);
			ConflictFace * f4 = new ConflictFace(new DCELFace());
			G.conflictFaces.push_back(f4);
			ConflictFace * f5 = new ConflictFace(new DCELFace());
			G.conflictFaces.push_back(f5);
			
			Conflict * c1 = new Conflict(p1, f2);
			p1->conflicts.push_back(c1);
			f2->conflicts.push_back(c1);

			Conflict * c2 = new Conflict(p2, f2);
			p2->conflicts.push_back(c2);
			f2->conflicts.push_back(c2);

			Conflict * c3 = new Conflict(p4, f4);
			p4->conflicts.push_back(c3);
			f4->conflicts.push_back(c3);

			Conflict * c4 = new Conflict(p5, f1);
			p5->conflicts.push_back(c4);
			f1->conflicts.push_back(c4);

			Conflict * c5 = new Conflict(p5, f2);
			p5->conflicts.push_back(c5);
			f2->conflicts.push_back(c5);

			Conflict * c6 = new Conflict(p5, f4);
			p5->conflicts.push_back(c6);
			f4->conflicts.push_back(c6);

			Conflict * c7 = new Conflict(p5, f5);
			p5->conflicts.push_back(c7);
			f5->conflicts.push_back(c7);

			Conflict * c8 = new Conflict(p6, f3);
			p6->conflicts.push_back(c8);
			f3->conflicts.push_back(c8);

			Conflict * c9 = new Conflict(p6, f4);
			p6->conflicts.push_back(c9);
			f4->conflicts.push_back(c9);

			Conflict * c10 = new Conflict(p6, f5);
			p6->conflicts.push_back(c10);
			f5->conflicts.push_back(c10);

			errorCheck(p5->conflicts.size() == 4, testClass, "deleteCorrespondingNodes 1", testID);
			errorCheck(p3->conflicts.size() == 0, testClass, "deleteCorrespondingNodes 2", testID);

			G.deleteCorrespondingNodes(p5);
			
			errorCheck(G.conflictPoints.size() == 5, testClass, "deleteCorrespondingNodes 3", testID);
			errorCheck(G.conflictFaces.size() == 1, testClass, "deleteCorrespondingNodes 4", testID);
			errorCheck(p1->conflicts.size() == 0, testClass, "deleteCorrespondingNodes 5", testID);
			errorCheck(p2->conflicts.size() == 0, testClass, "deleteCorrespondingNodes 6", testID);
			errorCheck(p6->conflicts.size() == 1, testClass, "deleteCorrespondingNodes 7", testID);
			errorCheck(p6->conflicts[0] == c8, testClass, "deleteCorrespondingNodes 8", testID);

			testID++;
		}

		std::cout << "PASSED: conflictGraphTest" << std::endl;
	}

	/* Test for the convex hull 3D algorithm implmented in convexHull.h */
	void convexHull3DTest() {
		string testClass = "convexHull.h";
		int testID = 1;

		std::cout << "RUN: convexHull3DTest" << std::endl;

		/* Test-ID:		#1
		 * Class:		convexHull.h
		 * Function:	convexHull3D
		 * CH3 with 4 Points 
		 */
		{
			vector<Point * > vec;
			vec.push_back(new Point3D(  0.00, 100.000,   0.000));
			vec.push_back(new Point3D(-81.65, -33.333, -47.140));
			vec.push_back(new Point3D( 81.65, -33.333, -47.140));
			vec.push_back(new Point3D(  0.00, -33.333,  94.281));

			DCEL dcel = ConvexHull3D(vec);

			errorCheck(dcel.getVerticeCount() == 4, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getFaceCount() == 4, testClass, "ConvexHull3D", testID);
			testID++;
		}


		/* Test-ID:		#2
		 * Class:		convexHull.h
		 * Function:	convexHull3D
		 * CH3 with 4 Points with 5 Inside Points
		 */
		{
		vector<Point * > vec;
			vec.push_back(new Point3D(  0.00, 100.000,   0.000));
			vec.push_back(new Point3D(-81.65, -33.333, -47.140));
			vec.push_back(new Point3D( 81.65, -33.333, -47.140));
			vec.push_back(new Point3D(  0.00, -33.333,  94.281));
			vec.push_back(new Point3D(  0.00,   0.000,   0.000));
			vec.push_back(new Point3D( 10.00,  10.000,  10.000));
			vec.push_back(new Point3D(-10.00, -10.000, -10.000));
			vec.push_back(new Point3D( 15.00,   0.000, -10.000));
			vec.push_back(new Point3D(  -0.08, -13.501, 67.402));

			vector<Point *> res = ConvexHull2D(vec);

			DCEL dcel = ConvexHull3D(vec);

			errorCheck(dcel.getVerticeCount() == 4, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getEdgeCount() == 12, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getFaceCount() == 4, testClass, "ConvexHull3D", testID);

			testID++;
		}

		/* Test-ID:		#3
		 * Class:		convexHull.h
		 * Function:	convexHull3D
		 * CH3 with Cube
		 */
		{
				vector<Point * > vec;
				vec.push_back(new Point3D( 100,  100, -100));
				vec.push_back(new Point3D( 100,  100,  100));
				vec.push_back(new Point3D( 100, -100, -100));
				vec.push_back(new Point3D( 100, -100,  100));
				vec.push_back(new Point3D(-100,  100,  100));
				vec.push_back(new Point3D(-100,  100, -100));
				vec.push_back(new Point3D(-100, -100,  100));
				vec.push_back(new Point3D(-100, -100, -100));

				DCEL dcel = ConvexHull3D(vec);

				errorCheck(dcel.getVerticeCount() == 8, testClass, "ConvexHull3D", testID);
				errorCheck(dcel.getEdgeCount() == 36, testClass, "ConvexHull3D", testID);
				errorCheck(dcel.getFaceCount() == 12, testClass, "ConvexHull3D", testID);

				testID++;
		
		}

		/* Test-ID:		#4
		 * Class:		convexHull.h
		 * Function:	convexHull3D
		 * CH3 with Cube & Interior points
		 */
		{
			vector<Point * > vec;
			vec.push_back(new Point3D(100, 100, -100));
			vec.push_back(new Point3D(100, 100, 100));
			vec.push_back(new Point3D(100, -100, -100));
			vec.push_back(new Point3D(100, -100, 100));
			vec.push_back(new Point3D(0, -12, -100));
			vec.push_back(new Point3D(0, -100, 15));
			vec.push_back(new Point3D(-100, 100, 100));
			vec.push_back(new Point3D(-100, 100, -100));
			vec.push_back(new Point3D(-100, -100, 100));
			vec.push_back(new Point3D(-100, -100, -100));
			vec.push_back(new Point3D(-50, 50, -20));
			vec.push_back(new Point3D(-10, -10, 10));
			
			DCEL dcel = ConvexHull3D(vec);

			errorCheck(dcel.getVerticeCount() == 8, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getEdgeCount() == 36, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getFaceCount() == 12, testClass, "ConvexHull3D", testID);

			testID++;

		}
		
		/* Test-ID:		#5
		 * Class:		convexHull.h
		 * Function:	convexHull3D
		 * CH3 with Icosa Platonic Object
		 */
		{
			vector<Point * > vec;
			vec.push_back(new Point3D(0, 52.573, 85.065));
			vec.push_back(new Point3D(-52.573, 85.065, 0));
			vec.push_back(new Point3D(52.573, 85.065, 0));
			vec.push_back(new Point3D(0, 52.573, -85.065));
			vec.push_back(new Point3D(85.065, 0, -52.573));
			vec.push_back(new Point3D(85.065, 0, 52.573));
			vec.push_back(new Point3D(-85.065, 0, -52.573));
			vec.push_back(new Point3D(0, -52.573, -85.065));
			vec.push_back(new Point3D(52.573, -85.065, 0));
			vec.push_back(new Point3D(-52.573, -85.065, 0));
			vec.push_back(new Point3D(0, -52.573, 85.065));
			vec.push_back(new Point3D(-85.065, 0, 52.573));
			
			DCEL dcel = ConvexHull3D(vec);

			errorCheck(dcel.getVerticeCount() == 12, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getEdgeCount() == 60, testClass, "ConvexHull3D", testID);
			errorCheck(dcel.getFaceCount() == 20, testClass, "ConvexHull3D", testID);

			testID++;
		}

		/* Test-ID:		#6
		 * Class:		convexHull.h
		 * Function:	convexHull3D
		 * Test 30
		 */
		{
			vector<Point * > vec;
			vec.push_back(new Point3D(12.5736, 2.91269, 12.2782));
			vec.push_back(new Point3D(7.36045, 9.60112, 19.3506));
			vec.push_back(new Point3D(13.8835, 9.3643, 4.4203));
			vec.push_back(new Point3D(8.53359, 17.0116, 13.1407));
			vec.push_back(new Point3D(18.0255, 19.6789, 7.42332));
			vec.push_back(new Point3D(11.182, 13.61, 11.182));
			vec.push_back(new Point3D(15.9966, 2.66243, 8.23145));
			vec.push_back(new Point3D(18.1304, 8.6874, 6.04877));
			vec.push_back(new Point3D(7.48131, 10.7926, 3.56945));
			vec.push_back(new Point3D(10.4941, 3.78613, 2.20771));
			vec.push_back(new Point3D(0.554826, 9.15738, 8.97183));
			vec.push_back(new Point3D(13.0052, 19.4519, 7.5515));
			vec.push_back(new Point3D(1.70537, 11.5677, 11.9517));
			vec.push_back(new Point3D(10.0595, 0.446791, 17.6818));
			vec.push_back(new Point3D(3.19956, 17.7416, 2.53365));
			vec.push_back(new Point3D(7.77978, 8.55312, 0.552995));
			vec.push_back(new Point3D(0.0231941, 9.91913, 1.16031));
			vec.push_back(new Point3D(17.1032, 12.9203, 6.59871));
			vec.push_back(new Point3D(15.9905, 5.07645, 10.4038));
			vec.push_back(new Point3D(13.5685, 5.92303, 15.4466));
			vec.push_back(new Point3D(0.698264, 12.7262, 15.5492));
			vec.push_back(new Point3D(13.5984, 0.19715, 7.24876));
			vec.push_back(new Point3D(8.27296, 0.49501, 2.4659));
			vec.push_back(new Point3D(17.0757, 12.3765, 17.0067));
			vec.push_back(new Point3D(17.7569, 2.86142, 1.64251));
			vec.push_back(new Point3D(4.52528, 3.65795, 16.541));
			vec.push_back(new Point3D(16.1815, 5.00076, 17.4981));
			vec.push_back(new Point3D(6.60848, 19.0698, 13.5075));
			vec.push_back(new Point3D(11.1301, 16.2609, 14.4401));
			vec.push_back(new Point3D(6.48213, 18.7329, 2.50618));

		   DCEL dcel = ConvexHull3D(vec);

			errorCheck(dcel.getVerticeCount() == 19, testClass, "ConvexHull3D Vert", testID);
			errorCheck(dcel.getEdgeCount() == 102, testClass, "ConvexHull3D Edge", testID);
			errorCheck(dcel.getFaceCount() == 34, testClass, "ConvexHull3D Face", testID);

			testID++;
		}

		std::cout << "PASSED: convexHull3DTest" << std::endl;
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