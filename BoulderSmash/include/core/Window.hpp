#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "core/Logger.hpp"

namespace Window
{
	glm::ivec2 size;
	glm::ivec2 position;
	glm::vec3 color;
	glm::vec2 mousePosition;
	GLFWwindow* raw;

	void Initalize()
	{
		if (!glfwInit())
			Logger_ThrowError("NULL", "Do you have OpenGL compatible graphics card?", true);

		glfwWindowHint(GLFW_SAMPLES, 16);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	
	static void FSC(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
	{
		Window::mousePosition.x = static_cast<float>(xposIn);
		Window::mousePosition.y = static_cast<float>(yposIn);
	}

	void Generate(const std::string& name, const glm::ivec2& size, const glm::vec3& color)
	{
		Logger_WriteConsole("Attempting to create GLFW window...", LogLevel::INFO);

		Window::size = size;
		Window::color = color;

		raw = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL);
		if(!raw)
			Logger_ThrowError("NULL", "Do you have OpenGL compatible graphics card?", true);

		glfwMakeContextCurrent(raw);
		glfwSetFramebufferSizeCallback(raw, FSC);
		glfwSetCursorPosCallback(raw, MouseCallback);

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			Logger_WriteConsole("Unexpected >>NULL<< at Window::CreateWindow::31 (Do you have GLAD compatible graphics card?)", LogLevel::ISSUE);

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
		return glfwWindowShouldClose(raw);
	}

	void UpdateColors()
	{
		glClearColor(color.x, color.y, color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void UpdateBuffers()
	{
		glfwSwapBuffers(raw);
		glfwPollEvents();
	}

	void CleanUp()
	{
		glfwTerminate();
	}
};
#endif // !WINDOW_HPP