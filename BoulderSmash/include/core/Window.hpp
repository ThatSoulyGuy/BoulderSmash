#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "core/Logger.hpp"

struct MouseCallbackData
{
	float mouseX;
	float mouseY;
};

bool shouldResize = false;

class Window
{

public:
	
	void GenerateWindow(const std::string& name, const int& width, const int& height)
	{
		Logger_WriteConsole("Attempting to create GLFW window...", LogLevel::INFO);

		if (!glfwInit())
			Logger_ThrowError("NULL", "Do you have OpenGL compatible graphics card?", true);

		glfwWindowHint(GLFW_SAMPLES, 16);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if(!window)
			Logger_ThrowError("NULL", "Do you have OpenGL compatible graphics card?", true);

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, FSC);
		glfwSetCursorPosCallback(window, MouseCallback);

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			Logger_WriteConsole("Unexpected >>NULL<< at Window::CreateWindow::31 (Do you have GLAD compatible graphics card?)", LogLevel::ERROR);

		backgroundColor = glm::vec3{ 0, 0, 0 };

		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Logger_WriteConsole("Successfully created GLFW window!", LogLevel::INFO);
	}

	bool ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void SetBackgroundColor(const glm::vec3& color)
	{
		Logger_WriteConsole("Set the current window's background color to: '" + std::to_string(color.x) + ", " + std::to_string(color.y) + ", " + std::to_string(color.z) + "'", LogLevel::INFO);
		backgroundColor = color;
	}

	void UpdateColors()
	{
		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void UpdateBuffers()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	GLFWwindow* GetInternal()
	{
		return window;
	}

	static void SetMouseCallback(const float& data1, const float& data2)
	{
		mouseData.mouseX = data1;
		mouseData.mouseY = data2;
	}

	static MouseCallbackData GetMouseCallback()
	{
		return mouseData;
	}

	void CleanUp()
	{
		glfwTerminate();
	}

	static glm::ivec2 GetDimensions()
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return glm::ivec2{width, height};
	}

	void SetShouldUpdate(bool value)
	{
		shouldResize = value;
	}

	bool GetShouldUpdate() const
	{
		return shouldResize;
	}

private:

	static void FSC(GLFWwindow* window, int width, int height)
	{
		shouldResize = true;
		glViewport(0, 0, width, height);
	}

	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
	{
		Window::SetMouseCallback(static_cast<float>(xposIn), static_cast<float>(yposIn));
	}

	glm::vec3 backgroundColor;
	static GLFWwindow* window;
	static MouseCallbackData mouseData;

};

GLFWwindow* Window::window;
MouseCallbackData Window::mouseData;

#endif // !WINDOW_HPP