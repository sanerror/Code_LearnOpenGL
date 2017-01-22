
#include <iostream>

// GLEW
#pragma comment(lib, "glew32.lib")
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "shader.h"

// Function prototype
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

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
	Shader ourShader("./VertexShader.glsl", "./FragmentShader.glsl");

// set up vertex data (and buffer(s)) and attribute pointers
	GLfloat Vertices[] = {
		// positions		// colors
		0.0f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
// Bind the Vertex Array Object first, then bind and 
// set vertex buffer(s) and attribute pointer(s).

	// ====================
	// First Triangle setup
	// ====================
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // it's safe to unbind VBO.
	glBindVertexArray(0); // unbind VAO after configuration.					  

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
		ourShader.use();
		/*
		GLfloat timeValue = glfwGetTime();
		GLfloat lastValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(ourShader.Program, "uniColor");
		glUniform1f(vertexColorLocation, lastValue);*/

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// swap the screen buffers
		glfwSwapBuffers(window); // double buffer swap
	}
	// properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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