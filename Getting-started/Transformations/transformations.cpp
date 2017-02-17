#include <iostream>

// GLEW
#pragma comment(lib, "glew32.lib")
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// other libs
#pragma comment(lib, "SOIL.lib")
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// other includes
#include "shader.h"

// Function prototype
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// holds uniform value of texture mix
GLfloat mixValue = 0.2f;

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
	glfwMakeContextCurrent(window); // make the context on the current thread 创建环境

	glfwSetKeyCallback(window, key_callback); // set the required callback functions

	// GLEW 
	// set to use modern approach to retrive function pointer and extensions
	glewExperimental = GL_TRUE;
	// initialize GLEW to setup te OpenGL function pointers
	glewInit();

	// VIEWPORT definition 
	glViewport(0, 0, WIDTH, HEIGHT); // 视口起始点（在窗口的坐标）和大小

									 //Build and compile our shader program
	Shader ourShader("./VertexShader.glsl", "./FragmentShader.glsl");

	// TEXTURES
	GLuint texture1, texture2;
	// ===========
	// texture 1
	// ===========
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height;
	unsigned char* image = SOIL_load_image("./container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// free the image and unbind the texture
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// ===========
	// texture 2
	// ===========
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	image = SOIL_load_image("./awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// free the image and unbind the texture
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// DATA
	GLfloat Vertices[] = {
		// positions		// colors		   // texture coord.
		0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// VAO
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and 
	// set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // it's safe to unbind VBO.
	glBindVertexArray(0); // unbind VAO after configuration.					  

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// check if any events have been activated (key pressed, mouse moved etc.) 
		// and call corresponding response function.
		glfwPollEvents(); // 检测键鼠输入操作，回调函数

		// render
		// clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader
		ourShader.use();
		/*
		GLfloat timeValue = glfwGetTime();
		GLfloat lastValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(ourShader.Program, "uniColor");
		glUniform1f(vertexColorLocation, lastValue);*/

		// draw container
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

		glUniform1f(glGetUniformLocation(ourShader.Program, "mixValue"), mixValue);

		// Transformation
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime()*50.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// swap the screen buffers
		glfwSwapBuffers(window); // double buffer swap
	}
	// properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
	if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		mixValue += 0.1f;
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		mixValue -= 0.1f;
		if (mixValue >= 0.0f)
			mixValue = 0.0f;
	}
}