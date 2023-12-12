#ifndef INPUT_HPP
#define INPUT_HPP

#include "core/Window.hpp"

namespace Input
{
	bool GetKeyDown(const int& key)
	{
		return glfwGetKey(window->GetInternal(), key) == GLFW_PRESS;
	}

	bool GetKeyUp(const int& key)
	{
		return glfwGetKey(window->GetInternal(), key) == GLFW_RELEASE;
	}

	bool GetMouseButtonDown(const int& button)
	{
		return glfwGetMouseButton(window->GetInternal(), button) == GLFW_PRESS;
	}

	bool GetMouseButtonUp(const int& button)
	{
		return glfwGetMouseButton(window->GetInternal(), button) == GLFW_RELEASE;
	}

	MouseCallbackData GetMousePosition()
	{
		return window->GetMouseCallback();

	}

	void SetCursorMode(const bool& value)
	{
		if (!value)
			glfwSetInputMode(window->GetInternal(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window->GetInternal(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

#endif // !INPUT_HPP