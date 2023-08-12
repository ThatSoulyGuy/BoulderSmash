#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3& qtov(glm::quat& q)
{
	return (glm::vec3&)q;
}

struct Transform
{
	void Translate(const glm::vec3& translation)
	{
		position += translation;
	}

	void Rotate(const glm::quat& rotation)
	{
		this->rotation = glm::quat{ rotation.w, this->rotation.x + rotation.x, this->rotation.y + rotation.y, this->rotation.z + rotation.z };
	}

	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 up;
};

#endif // !TRANSFORM_HPP