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
	glfwSetCursorPosCallback(window,&Visualisation::mouse_callback);
	glfwSetScrollCallback(window, &Visualisation::scroll_callback);

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
	this->camera = Camera(glm::vec3(10, 10, 35.0f));
	this->geoShader = Shader(".\\shader\\vertexShader.vert", ".\\shader\\fragmentShader.frag", ".\\shader\\geometryShader.geom");
	//this->geoShader = Shader(".\\shader\\vertexShader.vert", ".\\shader\\fragmentShader.frag");
	this->pointShader = Shader(".\\shader\\pointShader.vert", ".\\shader\\pointShader.frag");
	return 1;
}

/**
 * Returns Index of Vertex in Indice List or -1 if not Inside
 */
int getIndex(std::vector<DCELVertex *> list, DCELVertex * vertex) {
	int i = 0;
	for (DCELVertex * v : list) {
		if (v == vertex) {
			return i;
		}
		i++;
	}
	return -1;
}

void Visualisation::addRender(DCEL * dcel, bool wireFrame)
{
	unsigned int VBO, NBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &EBO);

	std::vector<DCELVertex *> dcelVertices;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	
	for (DCELFace * surface : dcel->surfaces) {
		vector<DCELVertex*> surfaceVertices = surface->getBoundary();
		if (surfaceVertices.size() == 3) {
			
			for (DCELVertex* vertex : surfaceVertices) {
				int idx = getIndex(dcelVertices, vertex);
				if (idx == -1) {
					vertices.push_back(vertex->point->getX());
					vertices.push_back(vertex->point->getY());
					vertices.push_back(vertex->point->getZ());
					indices.push_back(dcelVertices.size());
					dcelVertices.push_back(vertex);
				} else {
					indices.push_back(idx);
				}
			}
		}
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	glBindVertexArray(0);

	DrawAble drawAble;
	drawAble.count = (unsigned int)indices.size();
	drawAble.VAO = VAO;
	drawAble.VBO = VBO;
	drawAble.EBO = EBO;
	drawAble.model = glm::mat4(1.0f);
	drawAble.wireFrame = wireFrame;
	drawAble.mode = GL_TRIANGLES;

	this->drawAbles.push_back(drawAble);
}

void Visualisation::addRender(std::vector<Point*> points, GLenum mode, float r, float g, float b)
{
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	float * vertices = new float[points.size()*3];
	int idx = 0;
	for (int i = 0; i < points.size(); i ++) {
		vertices[idx++] = (float)points[i]->getX();
		vertices[idx++] = (float)points[i]->getY();
		vertices[idx++] = (float)points[i]->getZ();
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size()*3*sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	DrawAble drawAble;
	drawAble.count = (unsigned int)points.size();
	drawAble.VAO = VAO;
	drawAble.VBO = VBO;
	drawAble.mode = mode;
	drawAble.model = glm::mat4(1.0f);
	drawAble.color = glm::vec3(r,g,b);
	
	this->drawAbles.push_back(drawAble);
	delete[] vertices;
}

/**
 * 3D Render for scene/points
 */
void Visualisation::render()
{
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	this->geoShader.use();
	this->geoShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	this->geoShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	this->geoShader.setVec3("lightPos", glm::vec3(10.0f, 10.0f, 30.0f));

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (!this->wireFrame) {
			// Back-Face Culling
			glEnable(GL_CULL_FACE);
		} else {
			// No Back-Face Culling
			glDisable(GL_CULL_FACE);
		}

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// activate shader 
		this->geoShader.use();
		
		// pass projection matrix to shader (note that in this cas it could change every frame)
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		this->geoShader.setMat4("projection", projection);
		

		// camera/view transformation 
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		this->geoShader.setMat4("view", view);
		this->geoShader.setVec3("viewPos", camera.Position);

		this->pointShader.use();
		this->pointShader.setMat4("projection", projection);
		this->pointShader.setMat4("view", view);
		this->pointShader.setVec3("viewPos", camera.Position);
		// Render Data 
		for (auto drawObj : this->drawAbles) {
			if (drawObj.EBO == -1) {
				this->pointShader.use();
				glBindVertexArray(drawObj.VAO);
				this->pointShader.setMat4("model", drawObj.model);
				this->pointShader.setVec3("color", drawObj.color);
				glDrawArrays(drawObj.mode, 0, drawObj.count);
			} else {
				this->geoShader.use();
				if(drawObj.wireFrame && this->wireFrame)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(drawObj.VAO);
				this->geoShader.setMat4("model", drawObj.model);
				this->geoShader.setVec3("color", glm::vec3(1.0, 1.0, 1.0));
				this->geoShader.setInt("wireFrame", (drawObj.wireFrame && this->wireFrame));
				glDrawElements(drawObj.mode, drawObj.count, GL_UNSIGNED_INT, 0);
				if (drawObj.wireFrame && this->wireFrame)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
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

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Visualisation::mouse_callbackImpl(GLFWwindow* window, double xpos, double ypos)
{
	//Rotation 
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS &&
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		// reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;

		camera.rotate(xoffset, yoffset);
	}

	//Moving 
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS &&
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		// reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;

		//camera.ProcessMouseMovement(xoffset, yoffset);
		if (xoffset > 0) {
			camera.translate(LEFT, xoffset*0.05);
		}
		else {
			camera.translate(RIGHT, xoffset*-0.05);
		}

		if (yoffset > 0) {
			camera.translate(DOWN, yoffset*0.05);
		}
		else {
			camera.translate(UP, yoffset*-0.05);
		}
	}

	//Zoom 
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS &&
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (firstMouse)
		{
			lastX = xpos;
			//lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		//float yoffset = lastY - ypos;
		// reversed since y-coordinates range from bottom to top
		lastX = xpos;
		//lastY = ypos;

		//camera.ProcessMouseMovement(xoffset, yoffset);
		if (xoffset > 0) {
			camera.translate(FORWARD, xoffset*0.05);
		}
		else {
			camera.translate(BACKWARD, xoffset*-0.05);
		}
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE ||
		glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) &&
		(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE ||
			glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) &&
			(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE ||
				glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)) {
		firstMouse = true;
	}

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Visualisation::scroll_callbackImpl(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Visualisation::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// called every loop
// -------------------------------------------------------
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

	/* Wire-Frame */
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		wireFrameKey = true;
		
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
	{
		if (wireFrameKey) {
			this->wireFrame = !this->wireFrame;
			wireFrameKey = false;
		}
	}
}
