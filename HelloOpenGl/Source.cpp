#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"

GLFWwindow* getWindow();

bool GLADstatus();
bool shaderStatus(GLuint shader, std::string_view shader_type);
bool programStatus(GLuint program);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

constexpr int width = 1200;
constexpr int height = 800;

// Resources
// =========

float vertices[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2,   // first triangle
};

constexpr int numOfTraingles{ 1 };


int main()
{

	// Initialization
	// ==============

	GLFWwindow* window{ getWindow() };
	// Make sure all libraries are working
	if ((!window) || (!GLADstatus()))
		return 1;

	// Viewport size, should be same as window
	glViewport(0, 0, width, height);
	// Bind to resize viewport on window resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Get the shader program
	Shader shader("VertexShader.txt", "FragmentShader.txt");

	// Buffers
	// =======

	// Create a vertex array object to store a VBO and vertex attrib pointers that i'll use
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create a vertex buffer object to hold all vertices
	GLuint VBO;
	glGenBuffers(1, &VBO);
	// Bind VBO to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// import vertices into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Stores vertices data
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Tell vertex shader how to interpret data in VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Set which color the window will turn into when cleared
	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

	// Render loop
	// ===========

	// Debug thing do draw in wire frame mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// Get input to close window
		processInput(window);

		// Rendering
		// =========

		// Clear screen and use background color
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		// Uniforms
		float timeValue = static_cast<float>(glfwGetTime());
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		shader.setFloat("greenValue", greenValue);

		float scaler{ 1.9f };
		shader.setFloat("scaler", scaler);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numOfTraingles * 3, GL_UNSIGNED_INT, 0);


		// Swap buffers every frame to switch the image
		glfwSwapBuffers(window);
		// lotta stuff
		glfwPollEvents();
	}


	// End the program, delete everything
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.ID);

	glfwTerminate();
	return 0;
}

GLFWwindow* getWindow()
{
	// Initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window and opengl context
	GLFWwindow* window{ glfwCreateWindow(width, height, "second attempt vro", nullptr, nullptr) };

	// Check for successful creation
	if (!window)
	{
		std::cout << "Failed to create glfwWindow\n";
		glfwTerminate();
		return nullptr;
	}

	// Set that context as the current context
	// Opengl context is like a workspace opengl uses to execute all functions
	glfwMakeContextCurrent(window);
	return window;
}

// GLAD finds locations of opengl functions on the pc
bool GLADstatus()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	std::cout << "GLAD working\n";
	return true;
}

// Resize viewport on window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Get input to close window end terminate
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		std::cout << '\n';
		std::cout << "Key pressed, closing window.\n";
	}
}