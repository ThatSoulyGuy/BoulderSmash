#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform
{
	void Translate(const glm::vec3& translation)
	{
		position += translation;
	}

	void Rotate(const glm::vec3& rotation)
	{
		this->rotation += rotation;
	}

	glm::vec3 rotation;
	glm::vec3 position;
	glm::vec3 up;
};

#endif // !TRANSFORM_HPP