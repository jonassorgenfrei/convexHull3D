#pragma once

#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

#include "dcel.h"
#include "point2D.h"
#include "point3D.h"

// OpenGL
#include "camera.h"
#include "shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Visualisation {
	public:
		Visualisation();

		int init();

		void addRender(DCEL * dcel);

		void addRender(std::vector<Point2D> points);

		void addRender(std::vector<Point3D> points);

		void render();
	private:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		/*void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		// current Mouse position 
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void processInput(GLFWwindow *window);*/
		GLFWwindow* window;
		Shader shader;
		Camera camera; 
		bool firstMouse = true;
		float lastX = SCR_WIDTH / 2.0;
		float lastY = SCR_HEIGHT / 2.0;

		// timing 
		float deltaTime = 0.0f; // time between current frame and last frame
		float lastFrame = 0.0f; // time of last frame


};

#endif // !VISUALISATION_H
