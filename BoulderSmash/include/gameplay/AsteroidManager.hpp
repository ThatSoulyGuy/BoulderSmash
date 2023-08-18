#ifndef ASTEROID_MANAGER_HPP
#define ASTEROID_MANAGER_HPP

#include <vector>
#include "gameplay/EntityAsteroid.hpp"

namespace AsteroidManager
{
	extern std::vector<EntityAsteroid> spawnedAsteroids;

	void SpawnAsteroid(const Transform& transform, const BoxCollider& collider, const std::string& name)
	{
		GameObject& gameObject = ECSManager::AddGameObject();

		EntityAsteroid& asteroid = gameObject.AddComponent<EntityAsteroid>();
		asteroid.Start();
		asteroid.gameObject->transform = transform;
		asteroid.name = name;

		BoxCollider& boxCollider = gameObject.AddComponent<BoxCollider>();
		boxCollider = collider;

		spawnedAsteroids.push_back(gameObject.GetComponent<EntityAsteroid>());
		EntityManager::RegisterEntity(gameObject.GetComponent<EntityAsteroid>());
	}
}

std::vector<EntityAsteroid> AsteroidManager::spawnedAsteroids;

#endif // !ASTEROID_MANAGER_HPP