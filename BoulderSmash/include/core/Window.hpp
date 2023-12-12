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
	
	void CleanUp()
	{
		glfwTerminate();
	}
};
#endif // !WINDOW_HPP