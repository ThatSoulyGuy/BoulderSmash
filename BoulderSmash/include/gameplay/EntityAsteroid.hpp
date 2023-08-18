#ifndef ENTITY_ASTEROID_HPP
#define ENTITY_ASTEROID_HPP

#include "gameplay/Entity.hpp"
#include "rendering/Model.hpp"

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
		gameObject->transform.Rotate(glm::quat{5, 0, 0, 1});

		gameObject->GetComponent<Model>().data.transform.position = gameObject->transform.position;
		gameObject->GetComponent<Model>().data.transform.rotation = gameObject->transform.rotation;
	}

private:



};

#endif // !ENTITY_ASTEROID_HPP