#include "core/Logger.hpp"
#include "core/Input.hpp"
#include "core/Window.hpp"
#include "core/ECS.hpp"
#include "audio/SoundManager.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/Model.hpp"
#include "rendering/ShaderManager.hpp"
#include "rendering/Skybox.hpp"

int main(void)
{
	Logger_WriteConsole("Hello, GLFW 3.3.8!", LogLevel::INFO);

	ShaderManager::RegisterShader(ShaderObject::Register("shaders/default", "defaultShader"));
	ShaderManager::RegisterShader(ShaderObject::Register("shaders/transparent", "transparentShader"));
	ShaderManager::RegisterShader(ShaderObject::Register("shaders/light", "lightShader"));
	ShaderManager::RegisterShader(ShaderObject::Register("shaders/skybox", "skyboxShader"));
	
	SoundManager::Init();
	SoundManager::RegisterSound(SoundEffect::Register("sounds/explode1", "explosion", true, 1.0f));

	std::shared_ptr<Window> window(new Window());
	std::shared_ptr<Model> renderer(new Model());
	//RenderableObject object;
	std::shared_ptr<Camera> camera(new Camera());
	std::shared_ptr<PointLight> light(new PointLight());
	std::shared_ptr<DirectionalLight> directionalLight(new DirectionalLight());

	window->GenerateWindow("BoulderSmash* 0.1.4", 780, 450);
	window->SetBackgroundColor(glm::vec3{ 0.0f, 0.42f, 0.68f });
	Input::Init(window);

	directionalLight->Register(glm::vec3{ -0.2f, -1.0f, -0.3f }, glm::vec3{ 0.05f, 0.05f, 0.05f }, glm::vec3{ 0.4f, 0.4f, 0.4f }, glm::vec3{ 0.05f, 0.05f, 0.05f });
	Renderer::directionalLight = directionalLight;

	light->Register("light1", glm::vec3{ 0.05f, 0.05f, 0.05f }, glm::vec3{ 0.4f, 0.4f, 0.4f }, glm::vec3{ 0.05f, 0.05f, 0.05f });
	Renderer::RegisterLight(light);

	camera->InitCamera(window, glm::vec3{ 0, 0, -1.5 });

	Skybox::GenerateSkybox(DEFAULT_CUBEMAP);

	//object.GenerateTestObject("mm2", glm::vec3{0.0f, 0.0f, 0.0f});
	//Renderer::RegisterRenderableObject(object);
	renderer->GenerateModel("assets/characters/asteroid/model/model.obj", "modelthing", glm::vec3{0.0f, 1.0f, 0.0f});

	//SoundManager::PlayEffect("explosion");

	while (!window->ShouldClose())
	{
		window->UpdateColors();

		light->transform.position = camera->transform.position;

		camera->Update(window);
		
		Renderer::RenderObjects(camera);
		Skybox::Render(camera);

		window->UpdateBuffers();
	}

	window->CleanUp();
	SoundManager::CleanUp();

	return 0;
}