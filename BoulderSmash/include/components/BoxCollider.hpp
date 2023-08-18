#ifndef BOX_COLLIDER_HPP
#define BOX_COLLIDER_HPP

#include "components/Transform.hpp"
#include "core/ECS.hpp"

struct BoxCollider : public Component
{
	bool useGameObject;
	Transform transformThis;
	glm::vec3 size;

	bool IsCollidingWith(BoxCollider other)
	{
		glm::vec3 position;
		glm::vec3 otherPosition;

		if (useGameObject)
			position = gameObject->transform.position;
		else
			position = transformThis.position;

		otherPosition = other.transformThis.position;

		return (position.x < otherPosition.x + other.size.x) &&
			(position.x + size.x > otherPosition.x) &&
			(position.y < otherPosition.y + other.size.y) &&
			(position.y + size.y > otherPosition.y) &&
			(position.z < otherPosition.z + other.size.z) &&
			(position.z + size.z > otherPosition.z);
	}

	BoxCollider operator==(BoxCollider other)
	{
		BoxCollider out;

		if (other.useGameObject)
			out.gameObject->transform = other.transformThis;
		else
			out.transformThis = other.transformThis;
		
		return out;
	}

	static BoxCollider Register(const Transform& transform, const glm::vec3& size, const bool& useGameObject)
	{
		BoxCollider collider;

		collider.useGameObject = useGameObject;

		collider.transformThis = transform;

		collider.size = size;

		return collider;
	}
};

#endif // !BOX_COLLIDER_HPP