#ifndef ENTITY_ASTEROID_HPP
#define ENTITY_ASTEROID_HPP

#include "gameplay/Entity.hpp"

struct EntityAsteroid : public Entity
{
	
public:

	void Start() override
	{
		gameObject->AddComponent<Model>();
		gameObject->GetComponent<Model>().GenerateModel("assets/characters/asteroid/model/model.obj", "asteroid", glm::vec3{ 0.0f, 0.0f, 0.0f });
	}

	void Update() override
	{
		transform.Rotate(glm::quat{5, 0, 0, 1});
	}

private:



};

#endif // !ENTITY_ASTEROID_HPP