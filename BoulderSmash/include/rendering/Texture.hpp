#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <STBI/stb_image.h>
#include "core/Logger.hpp"

#define DEFAULT_TEXTURE_PROPERTIES TextureProperties::Register(GL_REPEAT, GL_NEAREST, true)

#include <string>
#include <glm/vec2.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STBI/stb_image.h>
#include "core/Logger.hpp"

struct TextureProperties
{
    GLint wrapping;
    GLint filter;

    bool flip;

    static TextureProperties Register(GLint wrapping, GLint filter, bool flip)
    {
        TextureProperties out;

        out.wrapping = wrapping;
        out.filter = filter;
        out.flip = flip;

        return out;
    }
};

struct TextureData
{
    TextureProperties properties;
    std::string path;
    std::string type;
    glm::ivec2 size;

    int channels;
    unsigned int ID;
};

struct Texture
{
    void LoadTexture(const std::string& localPath, const TextureProperties& properties)
    {
        data.properties = properties;
        std::string path = localPath;
        glGenTextures(1, &this->data.ID);
        glBindTexture(GL_TEXTURE_2D, this->data.ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, properties.wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, properties.wrapping);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, properties.filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, properties.filter);

        stbi_set_flip_vertically_on_load(properties.flip);

        unsigned char* data = stbi_load(path.c_str(), &this->data.size.x, &this->data.size.y, &this->data.channels, 0);
        if (data)
        {
            GLenum format;
            if (this->data.channels == 1)
                format = GL_RED;
            else if (this->data.channels == 3)
                format = GL_RGB;
            else if (this->data.channels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->data.size.x, this->data.size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            Logger_ThrowError("NULL", "Texture failed to load at path: '" + path + "'.", false);

        stbi_image_free(data);
    }

    static Texture LoadTextureStatic(const std::string& localPath, const TextureProperties& properties)
    {
        Texture out;

        out.LoadTexture(localPath, properties);

        return out;
    }

    static Texture LoadModernTextureStatic(const std::string& filename, const std::string& directory, const TextureProperties& properties)
    {
        Texture out;

        Logger_WriteConsole("Attempting to load up a Texture...", LogLevel::INFO);

        const std::string path = directory + '/' + filename;

        glGenTextures(1, &out.data.ID);

        unsigned char* data = stbi_load(path.c_str(), &out.data.size.x, &out.data.size.y, &out.data.channels , 0);

        stbi_set_flip_vertically_on_load(properties.flip);

        if (data)
        {
            GLenum format;
            if (out.data.channels == 1)
                format = GL_RED;
            else if (out.data.channels == 3)
                format = GL_RGB;
            else if (out.data.channels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, out.data.ID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, out.data.size.x, out.data.size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, properties.wrapping);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, properties.wrapping);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, properties.filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, properties.filter);

            stbi_image_free(data);
        }
        else
        {
            Logger_ThrowError("NULL", "Texture failed to load at path: '" + path + "'.", false);
            stbi_image_free(data);
        }

        Logger_WriteConsole("Successfully loaded up a Texture!", LogLevel::INFO);
        return out;
    }

    TextureData data;
};

#endif // !TEXTURE_HPP