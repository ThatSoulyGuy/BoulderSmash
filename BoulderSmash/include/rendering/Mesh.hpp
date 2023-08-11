#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "rendering/IRenderable.hpp"
#include "rendering/Shader.hpp"

#define MAX_BONE_INFLUENCE 4

struct Vertexa
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct ModelTexturea
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh : public IRenderable
{
public:

    void GenerateMesh(const std::vector<Vertexa>& vertices, const std::vector<unsigned int>& indices, const std::vector<ModelTexturea>& textures, ShaderObject type, const glm::vec3 & position = { 0, 0, 0 }, const glm::vec3 & rotation = { 0, 0, 0 })
    {
        Logger_WriteConsole("Attempting to generate a Mesh...", LogLevel::INFO);

        Init(InitalizeData::Register(position, rotation, type));

        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->shaderType = shaderType;

        SetupMesh();

        Logger_WriteConsole("Successfully generated a Mesh!", LogLevel::INFO);
    }

    ShaderObject GetShaderType() const
    {
        return shaderType;
    }

    bool InFront() override
    {
        return false;
    }

    void OnRenderCalled() override
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);

            glUniform1i(glGetUniformLocation(shader->GetID(), (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }

    std::vector<Vertexa> vertices;
    std::vector<unsigned int> indices;
    std::vector<ModelTexturea> textures;

private:

    void SetupMesh()
    {
        Logger_WriteConsole("Attempting to setup a Mesh...", LogLevel::INFO);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertexa), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertexa), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertexa), (void*)offsetof(Vertexa, normal));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertexa), (void*)offsetof(Vertexa, texCoords));
        
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertexa), (void*)offsetof(Vertexa, tangent));
        
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertexa), (void*)offsetof(Vertexa, bitangent));
        
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertexa), (void*)offsetof(Vertexa, boneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertexa), (void*)offsetof(Vertexa, weights));
        glBindVertexArray(0);

        Logger_WriteConsole("Successfully settedup a Mesh!", LogLevel::INFO);
    }

    ShaderObject shaderType;
};

#endif // !MESH_HPP