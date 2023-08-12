#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <memory>
#include "rendering/Model.hpp"

struct Entity
{

public:

	void Init()
	{
		model = std::make_shared<Model>();
		currentHealth = maxHealth;
	}

	void EntityUpdate()
	{
		model->data.transform.position = transform.position;
		model->data.transform.rotation = transform.rotation;
	}

	virtual void Start() { }

	virtual void Update() { }

	float maxHealth = 20.0f;
	float currentHealth;
	Transform transform;
	std::string name;
	std::shared_ptr<Model> model;

private:



};

namespace EntityManager
{
	extern std::vector<Entity> registeredEntities;
	
	void RegisterEntity(Entity& entity)
	{
		registeredEntities.push_back(entity);
	}

	void UpdateEntities()
	{
		for (Entity& entity : registeredEntities)
		{
			entity.EntityUpdate();
			entity.Update();
		}
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