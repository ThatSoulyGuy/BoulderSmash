#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "components/Transform.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Cubemap.hpp"
#include "rendering/ShaderManager.hpp"

unsigned int VAO, VBO;

std::shared_ptr<Cubemap> cubemap;
ShaderObject shader;
Transform transform;

namespace Skybox
{
    void InternalInit(ShaderObject type)
    {
        cubemap = std::make_shared<Cubemap>();
        shader = type;
        shader.BindShader();
    }

	void GenerateSkybox(CubemapContainer container, ShaderObject type = ShaderManager::GetShader("skyboxShader"))
	{
		InternalInit(type);
		cubemap->LoadFromFile(container.ToMapped());

        float skyboxVertices[] = 
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        shader.Use();
        shader.SetInt("skybox", 0);
	}

	void Render(const std::shared_ptr<Camera>& camera)
	{
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);

        shader.Use();

        shader.SetMat4("view", glm::mat4(glm::mat3(camera->GetProjection().view)));
        shader.SetMat4("projection", camera->GetProjection().projection);

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->GetTextureID());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

    void CleanUp()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif // !SKYBOX_HPP