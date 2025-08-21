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
#include "camera.h"

GLFWwindow* getWindow();

bool GLADstatus();
bool shaderStatus(GLuint shader, std::string_view shader_type);
bool programStatus(GLuint program);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint createTexture(const char* location, GLenum textureUnit, GLenum colorFormat);

float width = 1200.0f;
float height = 800.0f;

// Resources
// =========

constexpr float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

constexpr glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

constexpr int numOfCubes{ 10 };
constexpr int numOfTraingles{ 36 };

// Non const globals

float deltaTime{ 0.0f };
float lastFrame{ 0.0f };

// i have NO idea how the camera works
bool firstMouse = true;
float lastX = width / 2.0f;
float lastY = height / 2.0f;
float fov = 45.0f;

Camera camera;

// Input
bool mouseLocked = true;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// Initialization
	// ==============

	GLFWwindow* window{ getWindow() };
	// Make sure all libraries are working
	if ((!window) || (!GLADstatus()))
		return 1;

	// Viewport size, should be same as window
	glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
	// Bind to resize viewport on window resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Get the shader program
	Shader cubeShader("VertexShader.vert", "FragmentShader.frag");
	Shader lightSourceShader("LightSourceVertex.vert", "LightSourceFragment.frag");

	// Buffers
	// =======
	
	// first we do the cubeVAO and also create universal VBO with vertices
	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	// secondly we make the lightSourceVAO for the lightsource
	GLuint lightSourceVAO;
	glGenVertexArrays(1, &lightSourceVAO);
	glBindVertexArray(lightSourceVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	//glEnableVertexAttribArray(1);


	// Textures
	// ========

	// construction of the texture is handled inside createTexture(location)
	// just bind the texture using glBindTexture when i want to use it
	GLuint diffuseTexture{ createTexture("container2.png", GL_TEXTURE0, GL_RGBA) };
	GLuint specularTexture{ createTexture("container2_specular.png", GL_TEXTURE1, GL_RGBA) };


	// Render loop
	// ===========

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set which color the window will turn into when cleared
	glEnable(GL_DEPTH_TEST);

	// Debug thing do draw in wire frame mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// Get input to close window
		processInput(window);
		camera.ProcessKeyboard(window, deltaTime);

		// Calculate deltaTime
		float currentFrame{ static_cast<float>(glfwGetTime()) };
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Clear screen and use background color
		glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rendering
		// =========

		// first i activate the shader for other cubes
		cubeShader.use();

		// Set properties of material
		cubeShader.setFloat("material.shininess", 32.0f);

		cubeShader.setInt("material.diffuse", 0);
		cubeShader.setInt("material.specular", 1);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexture);

		// Set properties of light
		bool funkyColors{ false };

		glm::vec3 lightColor{ glm::vec3(1.0f)};

		if (funkyColors)
		{
			lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
			lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
			lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
		}

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.3f);

		cubeShader.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
		cubeShader.setVec3("light.ambient", ambientColor);
		cubeShader.setVec3("light.diffuse", diffuseColor);
		cubeShader.setVec3("light.specular", 1.0f);

		// Math
		// ====

		// View/Projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
		cubeShader.setVec3("cameraPos", camera.Position.x, camera.Position.y, camera.Position.z);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		//cubeShader.setMat4("model", model);


		// DRAW all cubes
		for (unsigned int i{ 0 }; i < numOfCubes; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			//float angle{ 20.0f * i };
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			cubeShader.setMat4("model", model);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, numOfTraingles);
		}

		// DRAW the lightsource, we need to use a different VAO and different Shader
		lightSourceShader.use();
		lightSourceShader.setMat4("projection", projection);
		lightSourceShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lightSourceShader.setMat4("model", model);

		glBindVertexArray(lightSourceVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Swap buffers every frame to switch the image
		glfwSwapBuffers(window);
		// lotta stuff
		glfwPollEvents();
	}


	// End the program, delete everything
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shader.ID);

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
	GLFWwindow* window{ glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "second attempt vro", nullptr, nullptr)};

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
void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height)
{
	width = static_cast<float>(new_width);
	height = static_cast<float>(new_height);
	glViewport(0, 0, new_width, new_height);
}

bool wasEscPressed = false;

// Get input to unlock or lock camera, close window end terminate
void processInput(GLFWwindow* window)
{
	bool isEscPressed = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isEscPressed = true;
	}

	if (isEscPressed && !wasEscPressed)
	{
		// Unlock Camera
		if (mouseLocked)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mouseLocked = false;
			std::cout << "Unlocking camera\n";
		}
		// Terminate
		else
		{
			glfwSetWindowShouldClose(window, true);
			std::cout << '\n';
			std::cout << "Key pressed, closing window.\n";
		}
	}

	wasEscPressed = isEscPressed;

	// Lock Camera
	if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)	{
		if (!mouseLocked)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			mouseLocked = true;
		}
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

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 90.0f)
		fov = 90.0f;
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}