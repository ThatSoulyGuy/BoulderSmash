#ifndef IRENDERABLE_HPP
#define IRENDERABLE_HPP

#define DEFAULT_SHADER_PATHS {"assets/shaders/defaultVertex.glsl", "assets/shaders/defaultFragment.glsl"}

#include <deque>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Logger.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Shader.hpp"
#include "rendering/ShaderManager.hpp"

struct ShaderCalla
{
    std::string name;
    std::string type;
    void* value;
};

std::vector<std::string> MakeStringArray(int size, const char* format ...)
{
    Logger_WriteConsole("Attempting to make a String array...", LogLevel::INFO);

    std::vector<std::string> out;

    for (int f = 0; f < size; ++f)
    {
        out.push_back(std::string(&format[f]));
    }
    
    Logger_WriteConsole("Successfully make a String array!", LogLevel::INFO);
    return out;
}

struct InitalizeData
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);

    std::string shaderPaths[2];

    static InitalizeData Register(const glm::vec3& position, const glm::vec3& rotation, ShaderObject type)
    {
        Logger_WriteConsole("Attempting to register an InitalizeData...", LogLevel::INFO);

        InitalizeData out;

        out.position = position;
        out.rotation = rotation;

        out.shaderPaths[0] = type.vertexPath;
        out.shaderPaths[1] = type.fragmentPath;

        Logger_WriteConsole("Successfully registered an InitalizeData!", LogLevel::INFO);
        return out;
    }
};

class IRenderable
{

public:
	
    void Init(const InitalizeData& data)
    {
        Logger_WriteConsole("Attempting to init internal stuff...", LogLevel::INFO);

        transform = std::make_shared<Transform>();
        transform->position = data.position;
        transform->rotation = data.rotation;

        shader = std::make_shared<Shader>();
        shader->LoadFromFile(data.shaderPaths[0], data.shaderPaths[1]);

        Logger_WriteConsole("Successfully initalized internal stuff!", LogLevel::INFO);
    }

    template<typename T>
    void RequestShaderCall(const std::string& name, const T& value)
    {
        ShaderCalla call;

        call.name = name;
        call.type = typeid(T).name();
        call.value = (void*)&value;

        for (auto& scall : shaderCalls)
        {
            if (scall.name == name)
            {
                scall = call;
                return;
            }
        }

        shaderCalls.push_back(call);
    }

    virtual bool InFront() = 0;

    virtual void OnRenderCalled() { }

    void Render(const std::shared_ptr<Camera>& camera)
    {
        if (InFront())
        {
            glDepthFunc(GL_LEQUAL);
            glDisable(GL_CULL_FACE);
        }

        OnRenderCalled();

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, this->transform->position);
        if (this->transform->rotation.x != 0.0f || this->transform->rotation.y != 0.0f || this->transform->rotation.z != 0.0f)
            transform = glm::rotate(transform, glm::radians(this->transform->rotation.y), this->transform->rotation);

        shader->Use();
        shader->SetMat4("model", transform);
        shader->SetMat4("view", camera->GetProjection().view);
        shader->SetMat4("projection", camera->GetProjection().projection);

        PostShaderCalls();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 30000, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
    }
    
    std::shared_ptr<Transform> transform;

protected:

    void PostShaderCalls()
    {
        while (!shaderCalls.empty())
        {
            auto& call = shaderCalls.front();

            if (call.type == "int")
                shader->SetInt(call.name, *(int*)call.value);
            else if (call.type == "float")
                shader->SetFloat(call.name, *(float*)call.value);
            else if (call.type == "bool")
                shader->SetBool(call.name, *(bool*)call.value);
            else if (call.type == "struct glm::vec<2,float,0>")
                shader->SetVec2(call.name, *(glm::vec2*)call.value);
            else if (call.type == "struct glm::vec<3,float,0>")
                shader->SetVec3(call.name, *(glm::vec3*)call.value);
            else if (call.type == "struct glm::vec<4,float,0>")
                shader->SetVec4(call.name, *(glm::vec4*)call.value);

            else if (call.type == "struct glm::mat<2,2,float,0>")
                shader->SetMat2(call.name, *(glm::mat2*)call.value);
            else if (call.type == "struct glm::mat<3,3,float,0>")
                shader->SetMat3(call.name, *(glm::mat3*)call.value);
            else if (call.type == "struct glm::mat<4,4,float,0>")
                shader->SetMat4(call.name, *(glm::mat4*)call.value);
            else
                Logger_ThrowError("Invalid type", "Graphical issues are imminent", false);

            shaderCalls.pop_front();
        }
    }

    
    std::shared_ptr<Shader> shader;
    std::deque<ShaderCalla> shaderCalls;

    unsigned int VAO = 0, VBO = 0, EBO = 0;
};

#endif // !RENDERABLE_OBJECT_HPP