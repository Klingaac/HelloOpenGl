#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string_view>
#include <string>

#include "shader.h"
#include "stb_image.h"

GLFWwindow* getWindow();

bool GLADstatus();
bool shaderStatus(GLuint shader, std::string_view shader_type);
bool programStatus(GLuint program);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLuint createTexture(const char* location, GLenum textureUnit, GLenum colorFormat);

constexpr int width = 1200;
constexpr int height = 800;

// Resources
// =========

float vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {  // note that we start from 0!
	3, 0, 1,   // first triangle
	1, 2, 3,   // second triangle
};

constexpr int numOfTraingles{ 2 };


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
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Stores vertices data
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Tell vertex shader how to interpret data in VBO
	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind buffers until we need to use them (in the render loop)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Textures
	// ========


	// construction of the texture is handled inside createTexture(location)
	// just bind the texture using glBindTexture when i want to use it
	GLuint texture0{ createTexture("container.jpg", GL_TEXTURE0, GL_RGB)};
	GLuint texture1{ createTexture("ajwm5.png", GL_TEXTURE1, GL_RGBA) };


	// Render loop
	// ===========

	// Set which color the window will turn into when cleared
	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

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
		float sinValue = (sin(timeValue) / 2.0f) + 0.5f;
		shader.setFloat("sinValue", sinValue);

		// Math
		float scaler{ 1.0f };

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(scaler, scaler, scaler));

		shader.setMat4("transform", trans);


		// Textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		shader.setInt("texture0", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		shader.setInt("texture1", 1);

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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

GLuint createTexture(const char* location, GLenum textureUnit, GLenum colorFormat)
{
	stbi_set_flip_vertically_on_load(true);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Texture wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(location, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "FAILED to create texture\n";
	}
	stbi_image_free(data);

	return texture;
}

void hi()
{

}