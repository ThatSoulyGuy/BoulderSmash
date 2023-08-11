#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#define DEFAULT_CUBEMAP CubemapContainer::Register("assets/skybox/default/top.png", "assets/skybox/default/bottom.png", "assets/skybox/default/front.png", "assets/skybox/default/back.png", "assets/skybox/default/right.png", "assets/skybox/default/left.png")
#define DEFAULT_CUBEMAP_MAPPED CubemapContainer::Register("assets/skybox/default/top.png", "assets/skybox/default/bottom.png", "assets/skybox/default/front.png", "assets/skybox/default/back.png", "assets/skybox/default/right.png", "assets/skybox/default/left.png").ToMapped()

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STBI/stb_image.h>
#include "core/Logger.hpp"

struct CubemapContainer
{
    std::string top;
    std::string bottom;

    std::string front;
    std::string back;

    std::string right;
    std::string left;

    std::vector<std::string> ToMapped()
    {
        std::vector<std::string> out;

        out.push_back(right);
        out.push_back(left);

        out.push_back(top);
        out.push_back(bottom);

        out.push_back(front);
        out.push_back(back);

        return out;
    }

    static CubemapContainer Register(std::string top, std::string bottom, std::string front, std::string back, std::string right, std::string left)
    {
        CubemapContainer container;

        container.top = top;
        container.bottom = bottom;

        container.front = front;
        container.back = back;

        container.right = right;
        container.left = left;

        return container;
    }
};

class Cubemap
{

public:
	
	void LoadFromFile(const std::vector<std::string>& faces)
	{
        
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, channel;

        for (unsigned int f = 0; f < faces.size(); ++f)
        {
            unsigned char* data = stbi_load(faces[f].c_str(), &width, &height, &channel, 0);

            GLenum format = GL_RGB;
            //if (channel == 1)
            //    format = GL_RED;
            //else if (channel == 3)
            //    format = GL_RGB;
            //else if (channel == 4)
            //    format = GL_RGBA;

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + f, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                Logger_ThrowError("NULL", "Cubemap texture failed to load at path: '" + faces[f] + "'.", "Cubemap", false);
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

    unsigned int GetTextureID() const
    {
        return textureID;
    }

private:

    unsigned int textureID;

};

#endif // !CUBEMAP_HPP