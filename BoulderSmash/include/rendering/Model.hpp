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
#include "rendering/Renderer.hpp"
#include "rendering/Shader.hpp"
#include "rendering/ShaderManager.hpp"
#include "rendering/Texture.hpp"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct ModelData
{
    std::vector<Texture> texturesLoaded;
    std::vector<RenderableObject> meshes;
    std::string directory;
    
    ShaderObject shaderType;
    Transform transform;
    
    bool repeatingTexture;

    std::string name;
};

class Model
{

public:

    void GenerateModel(const std::string& path, const std::string& name, const glm::vec3& position, const bool& repeatingTexture = true, const ShaderObject& shaderType = ShaderManager::GetShader("defaultShader"))
	{ 
        Logger_WriteConsole("Attempting to init a Model...", LogLevel::INFO);

        data.name = name;
        data.repeatingTexture = repeatingTexture;
        data.transform.position = position;
        data.shaderType = shaderType;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            Logger_WriteConsole(importer.GetErrorString(), LogLevel::ERROR);
            return;
        }
        
        data.directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);

        Logger_WriteConsole("Successfully initalized a Model!", LogLevel::INFO);
	}

    ModelData data;

private:

    void ProcessNode(aiNode* node, const aiScene* scene)
    {
        Logger_WriteConsole("Attempting to process a Node...", LogLevel::INFO);

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);
        }
        
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }

        Logger_WriteConsole("Successfully processed a Node!", LogLevel::INFO);
    }

    void ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        Logger_WriteConsole("Attempting to process a Mesh...", LogLevel::INFO);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            
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
        }
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        
        std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        
        std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        RenderableObject object;
        
        for (auto& texture : diffuseMaps)
            object.RegisterTexture(texture);
        
        for (auto& texture : specularMaps)
            object.RegisterTexture(texture);

        for (auto& texture : normalMaps)
            object.RegisterTexture(texture);

        for (auto& texture : heightMaps)
            object.RegisterTexture(texture);

        object.RegisterValues(true, true, data.name, data.transform.position, {}, indices);
        
        object.RequestGLBufferCall(GLBufferCall::Register(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW, "VBO", "verticesCall"));
        object.RequestGLBufferCall(GLBufferCall::Register(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW, "EBO", "indicesCall"));

        object.RequestGLPointerCall(GLPointerCall::Register(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0, 0, "firstCall", GLPointerType::D));
        object.RequestGLPointerCall(GLPointerCall::Register(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal), 1, "normalCall", GLPointerType::D));
        object.RequestGLPointerCall(GLPointerCall::Register(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords), 2, "texCoordsCall", GLPointerType::D));
        object.RequestGLPointerCall(GLPointerCall::Register(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent), 3, "tangentCall", GLPointerType::D));
        object.RequestGLPointerCall(GLPointerCall::Register(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent), 4, "bitangentCall", GLPointerType::D));
        object.RequestGLPointerCall(GLPointerCall::Register(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs), 5, "boneIDsCall", GLPointerType::I));
        object.RequestGLPointerCall(GLPointerCall::Register(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights), 6, "weightsCall", GLPointerType::D));

        object.GenerateRawObject();
        Renderer::RegisterRenderableObject(object);

        Logger_WriteConsole("Successfully processed a Mesh!", LogLevel::INFO);
    }

    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
    {
        Logger_WriteConsole("Attempting to load Material Textures...", LogLevel::INFO);

        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < data.texturesLoaded.size(); j++)
            {
                if (std::strcmp(data.texturesLoaded[j].data.path.c_str(), str.C_Str()) == 0)
                {
                    textures.push_back(data.texturesLoaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {   
                Texture texture = Texture::LoadTextureStatic(std::string(str.C_Str()), TextureProperties::Register(GL_REPEAT, GL_LINEAR, false));

                texture.data.type = typeName;

                textures.push_back(texture);
                data.texturesLoaded.push_back(texture);
            }
        }

        Logger_WriteConsole("Successfully loaded Material Textures!", LogLevel::INFO);
        return textures;
    }
};

#endif // !MODEL_HPP