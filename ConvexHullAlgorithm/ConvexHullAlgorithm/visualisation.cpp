#include "visualisation.h"

Visualisation::Visualisation()
{
	
	this->init();
}

int Visualisation::init()
{
	// glfw: initialize and configure
// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

// glfw window creation
// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, Visualisation::framebuffer_size_callback);
	//glfwSetCursorPosCallback(window,mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	//tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	this->camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	this->shader = Shader(".\\shader\\vertexShader.vert", ".\\shader\\fragmentShader.frag");

	return 1;
}

void Visualisation::addRender(DCEL * dcel)
{
}

void Visualisation::addRender(std::vector<Point*> points, GLenum mode)
{
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	float * vertices = new float[points.size()*3];
	int idx = 0;
	for (int i = 0; i < points.size(); i ++) {
		vertices[idx++] = points[i]->getX();
		vertices[idx++] = points[i]->getY();
		vertices[idx++] = points[i]->getZ();
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size()*3*sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	DrawAble drawAble;
	drawAble.count = points.size() * 3;
	drawAble.VAO = VAO;
	drawAble.VBO = VBO;
	drawAble.mode = mode;

	this->drawAbles.push_back(drawAble);
	delete[] vertices;
}

void Visualisation::render()
{
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	this->shader.use();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// activate shader 
		this->shader.use();

		// pass projection matrix to shader (note that in this cas it could change every frame)
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		this->shader.setMat4("projection", projection);

		// camera/view transformation 
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		this->shader.setMat4("view", view);

		// Render Data 
		for (auto drawObj : this->drawAbles) {
			glBindVertexArray(drawObj.VAO);
			glm::mat4 model = glm::mat4(1.0f);
			this->shader.setMat4("model", model);
			glDrawArrays(drawObj.mode, 0, drawObj.count);
		}
				
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// ------------------------------------------------------------------------------_
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	for (auto drawObj : this->drawAbles) {
		glDeleteVertexArrays(1, &drawObj.VAO);
		glDeleteBuffers(1, &drawObj.VAO);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Visualisation::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Visualisation::processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	/* Full Screen */
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	}
}
