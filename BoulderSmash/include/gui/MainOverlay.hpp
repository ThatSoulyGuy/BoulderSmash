#ifndef MAIN_OVERLAY_HPP
#define MAIN_OVERLAY_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gui/TextManager.hpp"

struct MainOverlay
{
	static double lastTime;
	static int frameCount;
	static std::string text;

	static void Init()
	{
		lastTime = glfwGetTime();
	}

	static void UpdateTime()
	{
		frameCount++;

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		if (deltaTime >= 1.0f)
		{
			double fps = double(frameCount) / deltaTime;

			text = "FPS: " + std::to_string((int)round(fps));

			frameCount = 0;
			lastTime = currentTime;
		}
	}

	static void RenderTime()
	{
		TextManager::RenderText(text, glm::vec2{ 0.0f, 0.95f }, glm::vec2{ 0.5f, 0.5f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
	}
};

double MainOverlay::lastTime;
int MainOverlay::frameCount;
std::string MainOverlay::text;

#endif // !MAIN_OVERLAY_HPP