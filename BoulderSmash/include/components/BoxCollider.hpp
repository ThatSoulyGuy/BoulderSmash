#ifndef BOX_COLLIDER_HPP
#define BOX_COLLIDER_HPP

#include "components/Transform.hpp"
#include "core/ECS.hpp"

struct BoxCollider : public Component
{
	Transform transform;
	glm::vec3 size;

	bool IsCollidingWith(BoxCollider other)
	{
		//FORMAT: v = Vertex, <x, y, z> Axis, <p, n> Positive/Negitive, <t, o> This/Other
		float vxpt = size.x / 2;
		float vxnt = -(size.x / 2);

		float vxpo = other.size.x / 2;
		float vxno = -(other.size.x / 2);


		float vypt = size.y / 2;
		float vynt = -(size.y / 2);

		float vypo = other.size.y / 2;
		float vyno = -(other.size.y / 2);


		float vzpt = size.z / 2;
		float vznt = -(size.z / 2);

		float vzpo = other.size.z / 2;
		float vzno = -(other.size.z / 2);

		if ((vxpt <= vxpo || vxnt >= vxno) && (vypt <= vypo || vynt >= vyno) && (vzpt <= vzpo || vznt >= vzno))
			return true;

		return false;
	}

	static BoxCollider Register(const Transform& transform, const glm::vec3& size)
	{
		BoxCollider collider;

		collider.transform = transform;
		collider.size = size;

		return collider;
	}
};

#endif // !BOX_COLLIDER_HPP