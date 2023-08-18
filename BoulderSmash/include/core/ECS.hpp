#ifndef ECS_HPP
#define ECS_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "components/Transform.hpp"

#define maxComponents 32

class Component;
class GameObject;

typedef std::size_t ComponentID;

inline ComponentID GetComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

template<typename T> 
inline ComponentID GetComponentTypeID() noexcept
{
	static ComponentID typeID = GetComponentTypeID();
	return typeID;
}

using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
	
public:

	GameObject* gameObject;

	virtual void Start() {}
	virtual void Update() {}

	virtual ~Component() {}
};

class GameObject
{

public:

	void Update()
	{
		for (auto& component : components)
		{
			component->Update();
		}
	}

	template<typename T>
	bool HasComponent()
	{
		return bitset[GetComponentTypeID<T>()];
	}

	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args)
	{
		T* component(new T(std::forward<TArgs>(args)...));
		component->gameObject = this;

		std::unique_ptr<Component> componentPtr{ component };
		components.emplace_back(std::move(componentPtr));

		array[GetComponentTypeID<T>()] = component;
		bitset[GetComponentTypeID<T>()] = true;

		component->Start();

		return *component;
	}

	template<typename T>
	T& GetComponent()
	{
		auto componentPtr(array[GetComponentTypeID<T>()]);

		return *static_cast<T*>(componentPtr);
	}

	bool active = true;
	Transform transform;

private:

	std::vector<std::unique_ptr<Component>> components;

	ComponentArray array;
	ComponentBitset bitset;
};

class ECSManager
{

public:

	static GameObject& AddGameObject()
	{
		GameObject* gameObject = new GameObject();
		std::unique_ptr<GameObject> gameObjectPtr{ gameObject };

		gameObjects.emplace_back(std::move(gameObjectPtr));

		return *gameObject;
	}

	static void UpdateGameObjects()
	{
		gameObjects.erase(std::remove_if(std::begin(gameObjects), std::end(gameObjects), [](const std::unique_ptr<GameObject>& gameobject) { return !gameobject->active; }), std::end(gameObjects));
		
		for (auto& gameObject : gameObjects)
		{
			gameObject->Update();
		}
	}

private:

	static std::vector<std::unique_ptr<GameObject>> gameObjects;

};

std::vector<std::unique_ptr<GameObject>> ECSManager::gameObjects;

#endif // !ECS_HPP