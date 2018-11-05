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

#include "dcel.h"
#include "convexHull.h"
#include "visualisation.h"
#include "point.h"
#include "point2D.h"
#include "point3D.h"

#define MAX_VALUE 20.0

typedef enum SPACE {TWO_DIM, THREE_DIM} Space;
 
// Which space we are operating in
Space space = THREE_DIM;
// How many input points we want to calculate
int pointCount = 20;
// Visualize Result
bool vis = false;
// run Convex Hull Algorithm
bool run = false;


float RandomFloat(float a, float b) {
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
					// argv[++current_arg] check if Number then adjust pointCount
				} else if (strcmp(argv[current_arg], "-space") == 0 ||
					strcmp(argv[current_arg], "--space") == 0 ||
					strcmp(argv[current_arg], "--s") == 0)
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
					std::cout << "Help" << std::endl;
				}
				else if (strcmp(argv[current_arg], "-run") == 0 ||
					strcmp(argv[current_arg], "--run") == 0 ||
					strcmp(argv[current_arg], "-r") == 0)
				{
					run = true;
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

		std::srand(std::time(nullptr)); // use current time as seed for random generator

		/* random points set */
		std::vector<Point*> pointSet;
		
		switch (space) {
			case TWO_DIM:
				for (int i = 0; i < pointCount; i++) {
					float x = RandomFloat(0.0, (float)MAX_VALUE);
					float y = RandomFloat(0.0, (float)MAX_VALUE);
					Point2D * point = new Point2D(x,y);
					pointSet.push_back(point);
				}
			
				break;
			case THREE_DIM:
				for (int i = 0; i < pointCount; i++) {
					float x = RandomFloat(0.0, (float)MAX_VALUE);
					float y = RandomFloat(0.0, (float)MAX_VALUE);
					float z = RandomFloat(0.0, (float)MAX_VALUE);
					Point3D * point = new Point3D(x, y, z);
					pointSet.push_back(point);
				}
				break;
			default:
				break;
		}

		// Print Generated Points
		std::cout << "Generated Points (" << pointSet.size() << "):" << std::endl;
		for (auto p : pointSet) {
			p->print();
			std::cout << std::endl;
		}

		if (run) {
			std::vector<Point*> ch;
			DCEL dcel;
			switch (space) {
				case TWO_DIM:
					//Run Convex Hull Algorithm 
					ch = ConvexHull2D(pointSet);
					std::cout << "Convex Hull Points (" << ch.size() << "):" << std::endl;
					for (auto p : ch) {
						p->print();
						std::cout << std::endl;
					}
					if (vis) {
						ch.push_back(ch[0]);
						Visualisation * visu = new Visualisation();
						visu->addRender(pointSet, GL_POINTS);
						visu->addRender(ch, GL_LINE_STRIP);
						visu->render();
					}
					break;
				case THREE_DIM:
					//Run Convex Hull Algorithm 
					dcel = ConvexHull3D(pointSet);
					if (vis) {
						//Visualise Points and CH Result
						//TODO:
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