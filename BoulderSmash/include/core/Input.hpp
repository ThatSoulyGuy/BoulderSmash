#ifndef INPUT_HPP
#define INPUT_HPP

#include "core/Window.hpp"

namespace Input
{
	bool GetKeyDown(const int& key)
	{
		return glfwGetKey(Window::raw, key) == GLFW_PRESS;
	}

	bool GetKeyUp(const int& key)
	{
		return glfwGetKey(Window::raw, key) == GLFW_RELEASE;
	}

	bool GetMouseButtonDown(const int& button)
	{
		return glfwGetMouseButton(Window::raw, button) == GLFW_PRESS;
	}

	bool GetMouseButtonUp(const int& button)
	{
		return glfwGetMouseButton(Window::raw, button) == GLFW_RELEASE;
	}

	glm::vec2 GetMousePosition()
	{
		return Window::mousePosition;
	}

	void SetCursorMode(const bool& value)
	{
		if (!value)
			glfwSetInputMode(Window::raw, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(Window::raw, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

#endif // !INPUT_HPP