#include <iostream>
#include <fstream>

// GLEW
#pragma comment(lib, "glew32s.lib")
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Function prototype
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// shader
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSourceOrange = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const GLchar* fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
// GLFW initialization, version, window, context, key_callback
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	glfwInit(); // init GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // major version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core-profile
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // unresizable

// create a GLFW window object that we can use for GLFW's funcitons
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // make the context on the current thread 创建环境
	glfwSetKeyCallback(window, key_callback); // set the required callback functions

											  // GLEW 
											  // set to use modern approach to retrive function pointer and extensions
	glewExperimental = GL_TRUE;
	// initialize GLEW to setup te OpenGL function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initializa GLEW" << std::endl;
		return -1;
	}

// VIEWPORT definition 
	int width, height;
	// retrieve the width and height from the window
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height); // 视口起始点（在窗口的坐标）和大小

//Build and compile our shader program
	// vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// fragment shader Orange
	GLuint fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL);
	glCompileShader(fragmentShaderOrange);

	// fragment shader Yellow
	GLuint fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	// link shaders for Orange
	GLuint shaderProgramOrange = glCreateProgram();
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glLinkProgram(shaderProgramOrange);

	// link shaders for Yellow
	GLuint shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);
	glDeleteShader(fragmentShaderYellow);

	// set up vertex data (and buffer(s)) and attribute pointers
	GLfloat firstVertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	GLfloat secondVertices[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	GLuint VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	// Bind the Vertex Array Object first, then bind and 
	// set vertex buffer(s) and attribute pointer(s).

	// ====================
	// First Triangle setup
	// ====================
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstVertices), firstVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // it's safe.
	glBindVertexArray(0); // unbind VAO after configuration.					  
						  // ====================
						  // First Triangle setup
						  // ====================
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		// check if any events have been activated (key pressed, mouse moved etc.) 
		// and call corresponding response function.
		glfwPollEvents(); // 检测键鼠输入操作，回调函数

						  // render
						  // clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgramOrange);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw our second triangle
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		// swap the screen buffers
		glfwSwapBuffers(window); // double buffer swap
	}
	// properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	// cleanring any resources allocated by glfw
	glfwTerminate();
	return 0;
}

// Is called when a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}