#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Input.hpp"
#include "core/Window.hpp"
#include "components/Transform.hpp"

struct CameraProjection
{
	glm::mat4 view;
	glm::mat4 projection;
};

class Camera
{

public:

	void InitCamera(const std::shared_ptr<Window>& window, const glm::vec3& position)
	{
		InternalInit();

		Logger_WriteConsole("Attempting to intialize camera...", LogLevel::INFO);

		transform->position = position;
		transform->rotation = glm::vec3{ 0.0f, -90.0f, 0.0f };
		transform->up = glm::vec3{ 0.0f, 1.0f, 0.0f };

		projection = glm::perspective<float>(glm::radians(45.0f), static_cast<float>((float)window->GetDimensions().x / (float)window->GetDimensions().y), 0.01, 100);
		view = glm::translate(view, transform->position);

		Input::SetCursorMode(false);

		Logger_WriteConsole("Successfully intialized camera!", LogLevel::INFO);
	}

	void Update(const std::shared_ptr<Window>& window)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		view = glm::lookAt(transform->position, transform->position + transform->rotation, transform->up);

		if (window->GetShouldUpdate())
		{
			projection = glm::perspective<float>(glm::radians(45.0f), static_cast<float>((float)window->GetDimensions().x / (float)window->GetDimensions().y), 0.01, 100);
			window->SetShouldUpdate(false);
		}

		UpdateInput();
	}

	CameraProjection GetProjection() const
	{
		CameraProjection out;
		out.projection = projection;
		out.view = view;
		return out;
	}

	std::shared_ptr<Transform> transform;

private:

	void InternalInit()
	{
		Logger_WriteConsole("Attempting to intialize internal stuff...", LogLevel::INFO);

		transform = std::make_shared<Transform>();

		Logger_WriteConsole("Successfully intialized internal stuff!", LogLevel::INFO);
	}

	void UpdateInput()
	{
		if (!isPaused)
		{
			float cameraSpeed = static_cast<float>(2.5 * deltaTime);
			if (Input::GetKeyDown(GLFW_KEY_W))
			{
				transform->position += cameraSpeed * transform->rotation;

				if (Input::GetKeyDown(GLFW_KEY_LEFT_CONTROL))
				{

					transform->position += cameraSpeed * 3 * transform->rotation;
				}
			}

			if (Input::GetKeyDown(GLFW_KEY_S))
				transform->position -= cameraSpeed * transform->rotation;
			if (Input::GetKeyDown(GLFW_KEY_A))
				transform->position -= glm::normalize(glm::cross(transform->rotation, transform->up)) * cameraSpeed;
			if (Input::GetKeyDown(GLFW_KEY_D))
				transform->position += glm::normalize(glm::cross(transform->rotation, transform->up)) * cameraSpeed;
		}

		if (Input::GetKeyDown(GLFW_KEY_ESCAPE) && !isPaused)
		{
			Input::SetCursorMode(true);

			isPaused = true;
		}
		else if (Input::GetKeyDown(GLFW_KEY_ESCAPE) && isPaused)
		{
			Input::SetCursorMode(false);

			isPaused = false;
		}

		if (isPaused) return;

		MouseCallbackData mouseData = Window::GetMouseCallback();
		float xpos = mouseData.mouseX;
		float ypos = mouseData.mouseY;

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->transform->rotation = glm::normalize(front);
	}

	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool isPaused;

	std::shared_ptr<Window> window;
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

};

#endif