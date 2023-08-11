#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <deque>
#include <vector>
#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/Logger.hpp"
#include "components/Transform.hpp"
#include "lighting/DirectionalLight.hpp"
#include "lighting/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/ShaderManager.hpp"
#include "rendering/Texture.hpp"

enum class GLPointerType
{
	D,
	I
};

struct GLPointerCall
{
	int from, to, stride, index;
	unsigned int type;
	bool normalized;
	void* pointer;
	std::string name;
	GLPointerType pointerType;

	static GLPointerCall Register(int from, int to, unsigned int type, bool normalized, int stride, void* pointer, int index, const std::string& name, GLPointerType pointerType)
	{
		GLPointerCall out;

		out.from = from;
		out.to = to;
		out.stride = stride;
		out.index = index;
		out.type = type;
		out.normalized = normalized;
		out.pointer = pointer;
		out.name = name;
		out.pointerType = pointerType;

		return out;
	}
};

struct GLBufferCall
{
	std::string bind;
	std::string name;
	unsigned int type, size, draw;
	void* pointer;

	static GLBufferCall Register(unsigned int type, unsigned int size, void* pointer, unsigned int draw, const std::string& bind, const std::string& name)
	{
		GLBufferCall out;

		out.bind = bind;
		out.type = type;
		out.size = size;
		out.draw = draw;
		out.pointer = pointer;
		out.name = name;

		return out;
	}
};

struct ModelTexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

struct RenderableData
{
	std::string name;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::deque<GLPointerCall> pointerCalls;
	std::deque<GLBufferCall> bufferCalls;
	std::vector<Texture> textures;

	TextureProperties texturePorperties;
	ShaderObject shader;
	Transform transform;
	bool doDefaultLighting;
	bool completelyReplaceDefaultGLPointerCalls;
};

struct RenderableObject
{
	RenderableData data;

	unsigned int VBO, VAO, EBO;

	void GenerateTestObject(const std::string& name, const glm::vec3& position)
	{
		float vertices[] =
		{
			//FRONT
			0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
			0.0f, 0.5f, 0.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
			0.5f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,

			//BACK
			0.0f, 0.0f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
			0.0f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			0.5f, 0.0f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

			//RIGHT
			0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			0.0f, 0.5f, 0.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			0.0f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			0.0f, 0.0f, 0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

			//LEFT
			0.5f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			0.5f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			0.5f, 0.0f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

			//TOP
			0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			0.0f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
			0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
			0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,

			//BOTTOM
			0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
			0.0f, 0.0f, 0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
			0.5f, 0.0f, 0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
			0.5f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f
		};

		unsigned int indices[] =
		{
			0, 1, 3,
			1, 2, 3,

			7, 6, 5,
			7, 5, 4,

			11, 10, 9,
			11, 9, 8,

			12, 13, 15,
			13, 14, 15,

			16, 17, 19,
			17, 18, 19,

			23, 22, 21,
			23, 21, 20
		};

		data.textures.push_back(Texture::LoadModernTextureStatic("stone.png", "assets/scenes", TextureProperties::Register(GL_REPEAT, GL_NEAREST, true)));
		data.textures[0].data.type = "texture_diffuse";
		RegisterValues(false, true, name, position, vertices, sizeof(vertices), indices, sizeof(indices));
		GenerateRawObject();
	}

	void RegisterTexture(const Texture& texture)
	{
		data.textures.push_back(texture);
	}

	void RequestGLPointerCall(GLPointerCall call)
	{
		data.pointerCalls.push_back(call);
	}

	void PostGLPointerCalls()
	{
		if (!data.completelyReplaceDefaultGLPointerCalls)
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

			//printf("glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0)\n");
			//printf("glEnableVertexAttribArray(0)\n");

			//printf("glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)))\n");
			//printf("glEnableVertexAttribArray(1)\n");

			//printf("glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)))\n");
			//printf("glEnableVertexAttribArray(2)\n");
		}

		while (!data.pointerCalls.empty())
		{
			auto& call = data.pointerCalls.front();

			switch (call.pointerType)
			{

			case GLPointerType::D:

				glVertexAttribPointer(call.from, call.to, call.type, call.normalized, call.stride, call.pointer);
				//printf("glVertexAttribPointer(%d, %d, %d, %d, %d, void)\n", call.from, call.to, call.type, (int)call.normalized, call.stride);
				break;

			case GLPointerType::I:

				glVertexAttribIPointer(call.from, call.to, call.type, call.stride, call.pointer);
				//printf("glVertexAttribIPointer(%d, %d, %d, %d, void)\n", call.from, call.to, call.type, call.stride);
				break;

			default:
				break;

			}
			
			glEnableVertexAttribArray(call.index);
			//printf("glEnableVertexAttribArray(%d)\n\n", call.index);

			data.pointerCalls.pop_front();
		}
	}

	void RemoveGLPointerCall(const std::string& name)
	{
		for (int r = 0; r < data.pointerCalls.size(); ++r)
		{
			if (data.pointerCalls[r].name == name)
			{
				data.pointerCalls.erase(data.pointerCalls.begin() + r);
				return;
			}
		}
	}

	void RequestGLBufferCall(GLBufferCall call)
	{
		data.bufferCalls.push_back(call);
	}

	void PostGLBufferCalls()
	{
		if (data.bufferCalls.size() <= 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

			//printf("glBindBuffer(GL_ARRAY_BUFFER, VBO);\n");
			//printf("glBufferData(GL_ARRAY_BUFFER, data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);\n");

			//printf("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);\n");
			//printf("glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size(), data.indices.data(), GL_STATIC_DRAW);\n");

			return;
		}

		while (!data.bufferCalls.empty())
		{
			auto& call = data.bufferCalls.front();

			if (call.bind == "VBO")
				glBindBuffer(call.type, VBO);
			else if (call.bind == "EBO")
				glBindBuffer(call.type, EBO);
			else
				throw new std::exception("call.bind is f*cked");
			
			glBufferData(call.type, call.size, call.pointer, call.draw);

			//printf("glBindBuffer(%d, %s);\n", call.type, call.bind.c_str());
			//printf("glBufferData(%d, %d, %s, %d);\n\n", call.type, call.size, "void", call.draw);

			data.bufferCalls.pop_front();
		}
	}

	void RemoveGLBufferCall(const std::string& name)
	{
		for (int r = 0; r < data.bufferCalls.size(); ++r)
		{
			if (data.bufferCalls[r].name == name)
			{
				data.bufferCalls.erase(data.bufferCalls.begin() + r);
				return;
			}
		}
	}

	void RegisterValues(const bool& completelyReplaceDefaultGLPointerCalls, const bool& doDefaultLighting, const std::string& name, const glm::vec3& position, float vertices[], size_t verticesSize, unsigned int indices[], size_t indicesSize, const std::string& shaderName = "defaultShader")
	{
		this->data.name = name;
		this->data.transform.position = position;
		this->data.doDefaultLighting = doDefaultLighting;
		data.completelyReplaceDefaultGLPointerCalls = completelyReplaceDefaultGLPointerCalls;

		data.shader = ShaderManager::GetShader(shaderName);
		data.shader.BindShader();

		for (int v = 0; v < verticesSize; ++v)
		{
			this->data.vertices.push_back(vertices[v]);
		}

		for (int i = 0; i < indicesSize; ++i)
		{
			this->data.indices.push_back(indices[i]);
		}
	}

	void RegisterValues(const bool& completelyReplaceDefaultGLPointerCalls, const bool& doDefaultLighting, const std::string& name, const glm::vec3& position, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::string& shaderName = "defaultShader")
	{
		this->data.name = name;
		this->data.transform.position = position;
		this->data.doDefaultLighting = doDefaultLighting;
		data.completelyReplaceDefaultGLPointerCalls = completelyReplaceDefaultGLPointerCalls;

		data.shader = ShaderManager::GetShader(shaderName);
		data.shader.BindShader();

		this->data.vertices = vertices;
		this->data.indices = indices;
	}

	void GenerateRawObject()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		PostGLBufferCalls();
		PostGLPointerCalls();

		glBindVertexArray(0);
	}
};

struct ShaderCall
{
	std::string objectName;
	std::string variableName;
	std::string type;
	void* value;

	static ShaderCall Register(const std::string& objectName, const std::string& variableName, const std::string& type, void* value)
	{
		ShaderCall out;

		out.objectName = objectName;
		out.variableName = variableName;
		out.type = type;
		out.value = value;

		return out;
	}
};

namespace Renderer
{
	extern std::vector<RenderableObject> registeredObjects;
	extern std::vector<std::shared_ptr<PointLight>> lights;
	extern std::deque<ShaderCall> shaderCalls;
	extern std::shared_ptr<DirectionalLight> directionalLight;

	void RegisterRenderableObject(const RenderableObject& object)
	{
		registeredObjects.push_back(object);
	}

	void RemoveRenderableObject(const std::string& name)
	{
		for (int r = 0; r < registeredObjects.size(); ++r)
		{
			if (registeredObjects[r].data.name == name)
			{
				registeredObjects.erase(registeredObjects.begin() + r);
				return;
			}
		}
	}

	RenderableObject& GetRegisteredObject(const std::string& name)
	{
		for (auto& object : registeredObjects)
		{
			if (object.data.name == name)
				return object;
		}
	}

	void RegisterLight(const std::shared_ptr<PointLight>& light)
	{
		lights.push_back(light);
	}

	void RemoveLight(const std::string& name)
	{
		for (int l = 0; l < lights.size(); ++l)
		{
			if (lights[l]->name == name)
			{
				lights.erase(lights.begin() + l);
				return;
			}
		}
	}

	std::shared_ptr<PointLight>& GetLight(const std::string& name)
	{
		for (auto& light : lights)
		{
			if (light->name == name)
				return light;
		}
	}

	template<typename T>
	void RequestShaderCall(const std::string& objectName, const std::string& variableName, const T& value)
	{
		ShaderCall call = ShaderCall::Register(objectName, variableName, typeid(T).name(), (void*)&value);

		for (auto& scall : shaderCalls)
		{
			if (scall.objectName == objectName)
			{
				scall = call;
				return;
			}
		}

		shaderCalls.push_back(call);
	}

	void PostShaderCalls()
	{
		while (!shaderCalls.empty())
		{
			for (auto& object : registeredObjects)
			{
				auto& call = shaderCalls.front();

				if (object.data.name == call.objectName)
				{
					if (call.type == "int")
						object.data.shader.SetInt(call.variableName, *(int*)call.value);
					else if (call.type == "float")
						object.data.shader.SetFloat(call.variableName, *(float*)call.value);
					else if (call.type == "bool")
						object.data.shader.SetBool(call.variableName, *(bool*)call.value);

					else if (call.type == "struct glm::vec<2,float,0>")
						object.data.shader.SetVec2(call.variableName, *(glm::vec2*)call.value);
					else if (call.type == "struct glm::vec<3,float,0>")
						object.data.shader.SetVec3(call.variableName, *(glm::vec3*)call.value);
					else if (call.type == "struct glm::vec<4,float,0>")
						object.data.shader.SetVec4(call.variableName, *(glm::vec4*)call.value);

					else if (call.type == "struct glm::mat<2,2,float,0>")
						object.data.shader.SetMat2(call.variableName, *(glm::mat2*)call.value);
					else if (call.type == "struct glm::mat<3,3,float,0>")
						object.data.shader.SetMat3(call.variableName, *(glm::mat3*)call.value);
					else if (call.type == "struct glm::mat<4,4,float,0>")
						object.data.shader.SetMat4(call.variableName, *(glm::mat4*)call.value);
					else
						Logger_ThrowError("Invalid type", "Graphical issues are imminent", false);

					shaderCalls.pop_front();
				}
			}
		}
	}

	void RenderObjects(std::shared_ptr<Camera>& camera)
	{
		for (auto& object : registeredObjects)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			for (unsigned int t = 0; t < object.data.textures.size(); t++)
			{
				glActiveTexture(GL_TEXTURE0 + t);

				std::string number;
				std::string name = object.data.textures[t].data.type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);

				glUniform1i(glGetUniformLocation(object.data.shader.ID, (name + number).c_str()), t);
				glBindTexture(GL_TEXTURE_2D, object.data.textures[t].data.ID);
			}
		
			glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::translate(transform, object.data.transform.position);

			if (object.data.transform.rotation.x <= 0)
				object.data.transform.rotation.x = 0.001f;

			if (object.data.transform.rotation.y <= 0)
				object.data.transform.rotation.y = 0.001f;

			if (object.data.transform.rotation.z <= 0)
				object.data.transform.rotation.z = 0.001f;

			object.data.shader.Use();
			object.data.shader.SetMat4("projection", camera->GetProjection().projection);
			object.data.shader.SetMat4("view", camera->GetProjection().view);
			object.data.shader.SetMat4("model", transform);

			if (object.data.doDefaultLighting)
			{
				for (int l = 0; l < lights.size(); ++l)
				{
					object.data.shader.SetVec3("viewPos", camera->transform->position);
					object.data.shader.SetFloat("material.shininess", 32.0f);

					object.data.shader.SetVec3("dirLight.direction", directionalLight->direction);
					object.data.shader.SetVec3("dirLight.ambient", directionalLight->ambient);
					object.data.shader.SetVec3("dirLight.diffuse", directionalLight->diffuse);
					object.data.shader.SetVec3("dirLight.specular", directionalLight->specular);

					object.data.shader.SetVec3("pointLights[" + std::to_string(l) + "].position", lights[l]->transform.position);
					object.data.shader.SetVec3("pointLights[" + std::to_string(l) + "].ambient", lights[l]->ambient);
					object.data.shader.SetVec3("pointLights[" + std::to_string(l) + "].diffuse", lights[l]->diffuse);
					object.data.shader.SetVec3("pointLights[" + std::to_string(l) + "].specular", lights[l]->specular);
					object.data.shader.SetFloat("pointLights[" + std::to_string(l) + "].constant", 1.0f);
					object.data.shader.SetFloat("pointLights[" + std::to_string(l) + "].linear", 0.09f);
					object.data.shader.SetFloat("pointLights[" + std::to_string(l) + "].quadratic", 0.032f);

					object.data.shader.SetVec3("spotLight.position", camera->transform->position);
					object.data.shader.SetVec3("spotLight.direction", camera->transform->rotation);
					object.data.shader.SetVec3("spotLight.ambient", glm::vec3{ 0.0f, 0.0f, 0.0f });
					object.data.shader.SetVec3("spotLight.diffuse", glm::vec3{ 0.0f, 0.0f, 0.0f });
					object.data.shader.SetVec3("spotLight.specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
					object.data.shader.SetFloat("spotLight.constant", 1.0f);
					object.data.shader.SetFloat("spotLight.linear", 0.09f);
					object.data.shader.SetFloat("spotLight.quadratic", 0.032f);
					object.data.shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
					object.data.shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.0f)));
				}
			}

			PostShaderCalls();
 
			glBindVertexArray(object.VAO);
			glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
		}
	}
};

std::vector<RenderableObject> Renderer::registeredObjects;
std::vector<std::shared_ptr<PointLight>> Renderer::lights;
std::deque<ShaderCall> Renderer::shaderCalls;
std::shared_ptr<DirectionalLight> Renderer::directionalLight;

#endif // !RENDERER_HPP