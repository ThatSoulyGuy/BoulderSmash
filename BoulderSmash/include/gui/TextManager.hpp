#ifndef TEXT_MANAGER_HPP
#define TEXT_MANAGER_HPP

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include "core/Logger.hpp"
#include "core/Window.hpp"
#include "rendering/ShaderManager.hpp"

struct Character 
{
    glm::ivec2 size;
    glm::ivec2 bearing;

    unsigned int textureID;
    unsigned int advance;
};

namespace TextManager
{
    extern unsigned int VAO, VBO;
    extern ShaderObject shader;
    extern std::map<GLchar, Character> characters;
	extern std::string fontPath;

    void InitRendering()
    {
        shader = ShaderManager::GetShader("textShader");
        shader.BindShader();

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window::GetDimensions().x), 0.0f, static_cast<float>(Window::GetDimensions().y));

        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    }

	void InitText()
	{
        InitRendering();

        fontPath = "assets/fonts/arial.ttf";

		FT_Library freeType;
		FT_Face face;

		if (FT_Init_FreeType(&freeType))
			Logger_ThrowError("NULL", "Unable to initalize FreeType library", true);

        if (FT_New_Face(freeType, fontPath.c_str(), 0, &face))
            Logger_ThrowError("NULL", "Unable to initalize new FreeType Font Face", false);
        else 
        {
            FT_Set_Pixel_Sizes(face, 0, 48);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            for (unsigned char c = 0; c < 128; c++)
            {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    Logger_ThrowError("NULL", "Unable to initalize find FreeType Glyph", false);
                    continue;
                }
                
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                
                Character character = 
                {
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),

                    texture,
                    static_cast<unsigned int>(face->glyph->advance.x)
                };

                characters.insert(std::pair<char, Character>(c, character));
            }

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(freeType);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}

    void RenderText(const std::string& text, glm::vec2 position, const float& scale, const glm::vec3& color)
    {
        shader.Use();
        glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        std::string::const_iterator c;

        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = characters[*c];

            float xpos = position.x + ch.bearing.x * scale;
            float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;
            
            float vertices[6][4] = 
            {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            position.x += (ch.advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

unsigned int TextManager::VAO;
unsigned int TextManager::VBO;
ShaderObject TextManager::shader;
std::map<GLchar, Character> TextManager::characters;
std::string TextManager::fontPath;

#endif // !TEXT_MANAGER_HPP