#ifndef ECS_HPP
#define ECS_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class GameObject;

using ComponentID = std::size_t;

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

constexpr std::size_t maxComponents = 128;

using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{

public:
	
	GameObject* gameObject;

	virtual void Start() { };
	virtual void Update() { };
	virtual void Render() { };

	virtual ~Component() { };

private:



};

class GameObject
{

public:
	
	void Update()
	{
		for (auto& c : components) c->Update();
	}

	void Render()
	{
		for (auto& c : components) c->Render();
	}

	bool IsActive() const
	{
		return active;
	}

	void Destroy()
	{
		active = false;
	}

	template<typename T>
	bool HasComponent() const
	{
		return componentBitset[GetComponentTypeID<T>()];
	}

	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->gameObject = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[GetComponentTypeID<T>()] = c;
		componentBitset[GetComponentTypeID<T>()] = true;
		
		c->Start();

		return *c;
	}

	template<typename T>
	T& GetComponent()
	{
		auto ptr(componentArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

private:

	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitset componentBitset;

};

class ECSManager
{

public:
	
	void Update()
	{
		for (auto& g : gameObjects) g->Update();
	}

	void Render()
	{
		for (auto& g : gameObjects) g->Render();
	}

	void Refresh()
	{
		gameObjects.erase(std::remove_if(std::begin(gameObjects), std::end(gameObjects), [](const std::unique_ptr<GameObject>& mGameObject) { return !mGameObject->IsActive(); }), std::end(gameObjects));
	}

	GameObject& AddGameObject()
	{
		GameObject* g = new GameObject();
		std::unique_ptr<GameObject> uPtr{ g };
		gameObjects.emplace_back(std::move(uPtr));

		return *g;
	}

private:

	std::vector<std::unique_ptr<GameObject>> gameObjects;

};

#endif // !ECS_HPP