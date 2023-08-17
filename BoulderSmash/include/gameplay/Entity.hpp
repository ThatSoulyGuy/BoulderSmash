#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <memory>
#include "rendering/Model.hpp"
#include "components/BoxCollider.hpp"

struct Entity : public Component
{

public:

	virtual void StartEntity() { }

	virtual void UpdateEntity() { }

	void Start() override
	{
		StartEntity();
		currentHealth = maxHealth;
	}

	void Update() override
	{
		UpdateEntity();

		if (gameObject->HasComponent<Model>())
		{
			gameObject->GetComponent<Model>().data.transform.position = transform.position;
			gameObject->GetComponent<Model>().data.transform.rotation = transform.rotation;
		}
		
		collider.transform = transform;
	}

	float maxHealth = 20.0f;
	float currentHealth;

	BoxCollider collider;
	Transform transform;
	std::string name;

private:



};

namespace EntityManager
{
	extern std::vector<Entity> registeredEntities;
	
	void RegisterEntity(Entity& entity)
	{
		registeredEntities.push_back(entity);
	}
	 
	Entity& GetEntity(const std::string& name)
	{
		for (Entity& entity : registeredEntities)
		{
			if (entity.name == name)
				return entity;
		}

		Logger_ThrowError("NULL", std::format("Entity: '{}' was not found.", name), false);
	}
}

std::vector<Entity> EntityManager::registeredEntities;

#endif // !ENTITY_HPP