#pragma once
/*
 * Sample program for convex hull calculation in 2-/3-Space
 * 
 * Part of the seminar talk at FH Wedel 2018
 *
 * @author -	Jonas Sorgenfrei
 *				Minf101767
 */

//System Headers
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <stdexcept>
#include <sstream>
#include <set>

//#include "dcel.h"
#include "convexHull.h"
#include "visualisation.h"
#include "point.h"
#include "point2D.h"
#include "point3D.h"
#include "Test/testSuite.h"

#define MAX_VALUE 20.0

typedef enum SPACE {TWO_DIM, THREE_DIM} Space;
 
// Which space we are operating in
Space space = THREE_DIM;
// How many input points we want to calculate
int pointCount = 1000;		// NOTE: a to high amount of numbers can result as a memory-problem in the CH 3D Calculations
// Visualize Result
bool vis = false;
// run Convex Hull Algorithm
bool run = false;
// run tests
bool runT = false;
// print points
bool printPoints = false;
// wireframe
bool wireFrame = false;
// test Suite
TestSuite testSuite = TestSuite();

float random(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

int main(int argc, char* argv[]) {
	std::cout << "Convex Hull Program" << std::endl << std::endl;

	try {
		// Arguments passed to the program
		int current_arg = 1;
		if (argc > 1) {
			while (current_arg < argc) {
				if (strcmp(argv[current_arg], "-count") == 0 ||
					strcmp(argv[current_arg], "--count") == 0 ||
					strcmp(argv[current_arg], "-c") == 0)
				{
					int tempInp = std::atoi(argv[++current_arg]);
					if (tempInp > 0)
						pointCount = tempInp;
				} else if (strcmp(argv[current_arg], "-space") == 0 ||
					strcmp(argv[current_arg], "--space") == 0 ||
					strcmp(argv[current_arg], "-s") == 0)
				{
					// Decide about space
					if (strcmp(argv[current_arg + 1], "3D") == 0 ||
						strcmp(argv[current_arg + 1], "3d") == 0) {
						space = THREE_DIM;
						current_arg++;
					}
					else if (strcmp(argv[current_arg + 1], "2D") == 0 ||
						strcmp(argv[current_arg + 1], "2d") == 0) {
						space = TWO_DIM;
						current_arg++;
					}
				}
				else if (strcmp(argv[current_arg], "-visualisation") == 0 ||
					strcmp(argv[current_arg], "--visualisation") == 0 ||
					strcmp(argv[current_arg], "-v") == 0)
				{
					vis = true;
				}
				else if (strcmp(argv[current_arg], "-help") == 0 ||
					strcmp(argv[current_arg], "--help") == 0 ||
					strcmp(argv[current_arg], "-h") == 0)
				{
					std::cout << "Sample program for convex hull calculation in 2-/3-Space" << std::endl;
					std::cout << "Part of the seminar talk at FH Wedel 2018" << std::endl;
					std::cout << "By Jonas Sorgenfrei (Minf101767)" << std::endl << std::endl;
					std::cout << "" << std::endl;
					std::cout << "ConvexHullAlgorithm.exe [-c Count] [-s Space] [-v] [-h] [-r]" << std::endl;
					std::cout << " -c Count   Count (>0) of random points in space" << std::endl;
					std::cout << " -s Space   choose Space {3D,2D}, default 3D" << std::endl;
					std::cout << " -v         visualise input and output" << std::endl;
					std::cout << " -h         show help" << std::endl;
					std::cout << " -r         run algorithm depending on current space" << std::endl;
					std::cout << " -w         wireframe" << std::endl;
					std::cout << " -t         run testsuite tests" << std::endl;
					std::cout << " -p         print Points" << std::endl;
					return 0;
				}
				else if (strcmp(argv[current_arg], "-run") == 0 ||
					strcmp(argv[current_arg], "--run") == 0 ||
					strcmp(argv[current_arg], "-r") == 0)
				{
					run = true;
				}
				else if (strcmp(argv[current_arg], "-wireframe") == 0 ||
					strcmp(argv[current_arg], "--wireframe") == 0 ||
					strcmp(argv[current_arg], "-w") == 0)
				{
					wireFrame = true;
				}
				else if (strcmp(argv[current_arg], "-testsuite") == 0 ||
					strcmp(argv[current_arg], "--testsuite") == 0 ||
					strcmp(argv[current_arg], "-t") == 0)
				{
					runT = true;
				}
				else if (strcmp(argv[current_arg], "-printpoints") == 0 ||
					strcmp(argv[current_arg], "--printpoints") == 0 ||
					strcmp(argv[current_arg], "-p") == 0)
				{
					printPoints = true;
				}
				else {
					std::stringstream msg;
					msg << "Error: Invalide Argument. ("
						<< current_arg
						<< ") "
						<< argv[current_arg]
						<< std::endl;
					throw std::invalid_argument(msg.str());
				}
				current_arg++;
			}
		}

		// Test Suite to proof correctness of sub algorithms
		if (runT) 
			testSuite.run();

		std::srand(std::time(nullptr)); // use current time as seed for random generator

		/* random points set */
		std::vector<Point*> pointSet;
		
		switch (space) {
			case TWO_DIM:
				for (int i = 0; i < pointCount; i++) {
					double x = (double)random(0.0f, (float)MAX_VALUE);
					double y = (double)random(0.0f, (float)MAX_VALUE);
					Point2D * point = new Point2D(x,y);
					pointSet.push_back(point);
				}
			
				break;
			case THREE_DIM:
				for (int i = 0; i < pointCount; i++) {
					double x = (double)random(0.0f, (float)MAX_VALUE);
					double y = (double)random(0.0f, (float)MAX_VALUE);
					double z = (double)random(0.0f, (float)MAX_VALUE);
					Point3D * point = new Point3D(x, y, z);
					pointSet.push_back(point);
				}
				break;
			default:
				break;
		}

		// Remove double points
		int pC = 0;
		while (pC < pointSet.size()) {
			int pC2 = 0;
			while (pC2 < pointSet.size() && (pC == pC2 || !pointSet[pC]->isEqual(pointSet[pC2]))) {
				pC2++;
			}
			if (pC2 < pointSet.size()) {
				pointSet.erase(pointSet.begin() + pC);
			}
			else {
				pC++;
			}
		}


		// Print Generated Points
		std::cout << "Generated Points (" << pointSet.size() << "):" << std::endl;
		
		if (printPoints) {
			for (auto p : pointSet) {
				p->print();
				std::cout << std::endl;
			}
		}

		if (run) {
			std::vector<Point*> ch;
			DCEL dcel;
			switch (space) {
				case TWO_DIM:
					//Run Convex Hull Algorithm 
					ch = ConvexHull2D(pointSet);
					std::cout << "Convex Hull Points (" << ch.size() << "):" << std::endl;
				
					if (printPoints) {
						for (auto p : ch) {
							p->print();
							std::cout << std::endl;
						}
					}


					if (vis) {
						//Visualise Points and CH Result
						ch.push_back(ch[0]);
						Visualisation &visu = Visualisation::getInstance(); // initialize the singleton
						visu.addRender(ch, GL_LINE_STRIP, 1.0, 0.0, 0.0);
						visu.addRender(pointSet, GL_POINTS);
						visu.render();
					}
					break;
				case THREE_DIM:
					//Run Convex Hull Algorithm 
					dcel = ConvexHull3D(pointSet);
					dcel.printDCELInfo();
					if (printPoints) {
						dcel.printDCEL();
					}
					if (vis) {
						//Visualise Points and CH Result
						Visualisation &visu = Visualisation::getInstance(); // initialize the singleton
						visu.addRender(&dcel, wireFrame);
						visu.addRender(pointSet, GL_POINTS);
						visu.render();
					}
					break;
				default:
					break;
			}
		} 
		else {
			switch (space) {
				case TWO_DIM:
					if (vis) {
						//Visualise Points and CH Result
						Visualisation &visu = Visualisation::getInstance(); // initialize the singleton
						visu.addRender(pointSet, GL_POINTS);
						visu.render();
					}
					break;
				case THREE_DIM:
					if (vis) {
						//Visualise Points and CH Result
						Visualisation &visu = Visualisation::getInstance(); // initialize the singleton
						visu.addRender(pointSet, GL_POINTS);
						visu.render();
					}
					break;
				default:
					break;
			}

		}

	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}