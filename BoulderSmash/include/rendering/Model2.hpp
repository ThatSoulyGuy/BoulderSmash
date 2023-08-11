#ifndef MODEL_HPP
#define MODEL_HPP

#include <type_traits>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "components/Transform.hpp"
#include "core/ECS.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/IRenderableContainer.hpp"
#include "rendering/Shader.hpp"
#include "rendering/ShaderManager.hpp"
#include "rendering/Texture.hpp"

unsigned int TextureFromFile(const std::string& path, const std::string& directory, const bool& gamma, const bool& repeat = true)
{
    Logger_WriteConsole("Attempting to load up a Texture...", LogLevel::INFO);

    std::string filename = directory + '/' + path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (repeat)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
            
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        Logger_ThrowError("NULL", "Texture failed to load at path: '" + path + "'.", false);
        stbi_image_free(data);
    }

    Logger_WriteConsole("Successfully loaded up a Texture!", LogLevel::INFO);
    return textureID;
}

class Model : public IRenderableContainer<std::string, glm::vec3, ShaderObject, bool, std::vector<std::shared_ptr<Mesh>>>
{

public:
	
    void InternalInit() override
    {
        Logger_WriteConsole("Attempting to init internal stuff...", LogLevel::INFO);

        transform = std::make_shared<Transform>();

        Logger_WriteConsole("Successfully initalized internal stuff!", LogLevel::INFO);
    }

    void OnInitCalled(const std::string& path, const glm::vec3& position, const bool& repeatingTexture = true, const ShaderObject& shaderType = ShaderManager::GetShader("defaultShader")) override
	{ 
        Logger_WriteConsole("Attempting to init a Model...", LogLevel::INFO);

        InternalInit();

        this->repeatingTexture = repeatingTexture;
        transform->position = position;
        this->shaderType = shaderType;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            Logger_WriteConsole(importer.GetErrorString(), LogLevel::ERROR);
            return;
        }
        
        directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);

        Logger_WriteConsole("Successfully initalized a Model!", LogLevel::INFO);
	}

	void OnRenderCalled(const std::shared_ptr<Camera>& camera) override
	{
        for (unsigned int o = 0; o < otherData.size(); o++)
        {
            otherData[o]->transform = transform;
            otherData[o]->Render(camera);
        }
	}

private:

    void ProcessNode(aiNode* node, const aiScene* scene)
    {
        Logger_WriteConsole("Attempting to process a Node...", LogLevel::INFO);

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            otherData.push_back(ProcessMesh(mesh, scene));
        }
        
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }

        Logger_WriteConsole("Successfully processed a Node!", LogLevel::INFO);
    }

    std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        Logger_WriteConsole("Attempting to process a Mesh...", LogLevel::INFO);

        //std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<ModelTexture> textures;
        /*for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            //Vertex vertex;
            
            vertex.position = glm::vec3{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            
            if (mesh->HasNormals())
                vertex.normal = glm::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            
            if (mesh->mTextureCoords[0])
            {
                vertex.texCoords = glm::vec2{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
                
                vertex.tangent = glm::vec3{mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                
                vertex.bitangent = glm::vec3{mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
            }
            else
                vertex.texCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }*/
        
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<ModelTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        std::vector<ModelTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        
        std::vector<ModelTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        
        std::vector<ModelTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        std::shared_ptr<Mesh> mmesh(new Mesh());
       // mmesh->GenerateMesh(vertices, indices, textures, shaderType, transform->position);

        Logger_WriteConsole("Successfully processed a Mesh!", LogLevel::INFO);
        return mmesh;
    }

    std::vector<ModelTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
    {
        Logger_WriteConsole("Attempting to load Material Textures...", LogLevel::INFO);

        std::vector<ModelTexture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < texturesLoaded.size(); j++)
            {
                if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(texturesLoaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {   
                ModelTexture texture;
                
                texture.id = TextureFromFile(str.C_Str(), this->directory, false, repeatingTexture);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);

                texturesLoaded.push_back(texture);
            }
        }

        Logger_WriteConsole("Successfully loaded Material Textures!", LogLevel::INFO);
        return textures;
    }

    bool repeatingTexture;
    std::string directory;
    ShaderObject shaderType;
    std::vector<ModelTexture> texturesLoaded;
	
};

#endif // !MODEL_HPP