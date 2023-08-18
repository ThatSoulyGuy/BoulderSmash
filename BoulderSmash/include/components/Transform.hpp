#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TRANSFORM_DEFAULT Transform::Register(glm::vec3{0.0f, 0.0f, 0.0f}, glm::quat{1.0f, 0.0f, 0.0f, 0.0f})
#define TRANSFORM_POSITION(x, y, z) Transform::Register(glm::vec3{x, y, z}, glm::quat{1.0f, 0.0f, 0.0f, 0.0f})
#define TRANSFORM_ROTATION(x, y, z, w) Transform::Register(glm::vec3{0.0f, 0.0f, 0.0f}, glm::quat{w, x, y, z})

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

	static Transform Register(const glm::vec3& position, const glm::quat& rotation)
	{
		Transform transform;

		transform.position = position;
		transform.rotation = rotation;

		return transform;
	}

	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 up;
};

#endif // !TRANSFORM_HPP