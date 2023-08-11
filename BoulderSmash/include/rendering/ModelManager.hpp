#ifndef MODEL_MANAGER_HPP
#define MODEL_MANAGER_HPP

#include <vector>
#include "lighting/DirectionalLight.hpp"
#include "lighting/PointLight.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/Model2.hpp"

std::shared_ptr<DirectionalLight> directionalLight;
std::vector<std::shared_ptr<PointLight>> lights;
std::vector<std::shared_ptr<IRenderableContainer<void*, void*, void*, void*, std::vector<std::shared_ptr<Mesh>>>>> objects;

namespace ModelManager
{
	void RenderObjects(const std::shared_ptr<Camera>& camera)
	{
		for (int o = 0; o < objects.size(); ++o)
		{
			for (int l = 0; l < lights.size(); ++l)
			{
				for (int m = 0; m < objects[o]->GetOtherData().size(); ++m)
				{
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("viewPos", camera->transform->position);
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("material.shininess", 32.0f);

					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("dirLight.direction", directionalLight->direction);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("dirLight.ambient", directionalLight->ambient);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("dirLight.diffuse", directionalLight->diffuse);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("dirLight.specular", directionalLight->specular);

					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("pointLights[" + std::to_string(o) + "].position", camera->transform->position);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("pointLights[" + std::to_string(o) + "].ambient", lights[l]->ambient);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("pointLights[" + std::to_string(o) + "].diffuse", lights[l]->diffuse);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("pointLights[" + std::to_string(o) + "].specular", lights[l]->specular);
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("pointLights[" + std::to_string(o) + "].constant", 1.0f);
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("pointLights[" + std::to_string(o) + "].linear", 0.09f);
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("pointLights[" + std::to_string(o) + "].quadratic", 0.032f);

					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("spotLight.position", camera->transform->position);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("spotLight.direction", camera->transform->rotation);
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("spotLight.ambient", glm::vec3{ 0.0f, 0.0f, 0.0f });
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("spotLight.diffuse", glm::vec3{ 0.0f, 0.0f, 0.0f });
					objects[o]->GetOtherData()[m]->RequestShaderCall<glm::vec3>("spotLight.specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("spotLight.constant", 1.0f);
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("spotLight.linear", 0.09f);
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("spotLight.quadratic", 0.032f);
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
					objects[o]->GetOtherData()[m]->RequestShaderCall<float>("spotLight.outerCutOff", glm::cos(glm::radians(17.0f)));
					
					objects[o]->OnRenderCalled(camera);
				}

				//lights[l]->Render(camera);
			}
		}
	}

	void SetDirectionalLight(const std::shared_ptr<DirectionalLight>& light)
	{
		directionalLight = light;
	}

	void RegisterObject(void* object)
	{
		objects.push_back(*(std::shared_ptr<IRenderableContainer<void*, void*, void*, void*, std::vector<std::shared_ptr<Mesh>>>>*)object);
	}

	void RegisterLight(const std::shared_ptr<PointLight>& light)
	{
		lights.push_back(light);
	}

	void RemoveObject(int index)
	{

	}
}

#endif // !LIGHTING_MANAGER_HPP