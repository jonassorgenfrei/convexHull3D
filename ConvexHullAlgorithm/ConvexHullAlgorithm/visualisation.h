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

const unsigned int SCR_WIDTH = 3200;
const unsigned int SCR_HEIGHT = 2400;

typedef struct DrawAble {
	unsigned int VBO;
	unsigned int VAO;
	GLenum mode;
	unsigned int count;
} DrawAble;

class Visualisation {
	public:
		Visualisation(void);

		int init();

		void addRender(DCEL * dcel);

		void addRender(std::vector<Point*> points, GLenum mode);

		void render();
		static Visualisation& getInstance() // Singleton is accessed via getInstance()
		{
			static Visualisation instance; // lazy singleton, instantiated on first use
			return instance;
		};
	private:
		/* Members */
		GLFWwindow* window;
		Shader shader;
		Camera camera;
		std::vector<DrawAble> drawAbles;
		bool firstMouse = true;
		float lastX = SCR_WIDTH / 2.0;
		float lastY = SCR_HEIGHT / 2.0;

		// timing 
		float deltaTime = 0.0f; // time between current frame and last frame
		float lastFrame = 0.0f; // time of last frame
		
		/* Functions*/

		/* Callbacks */
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
			//here we access the instance via the singleton pattern and forward the callback to the instance method
			getInstance().mouse_callbackImpl(window,xpos, ypos);
		}
		void mouse_callbackImpl(GLFWwindow* window, double xpos, double ypos);

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			//here we access the instance via the singleton pattern and forward the callback to the instance method
			getInstance().scroll_callbackImpl(window, xoffset, yoffset);
		}
		void scroll_callbackImpl(GLFWwindow* window, double xoffset, double yoffset);

		void processInput(GLFWwindow *window);

		Visualisation(Visualisation const&); // prevent copies
		void operator=(Visualisation const&); // prevent assignments
};

#endif // !VISUALISATION_H
