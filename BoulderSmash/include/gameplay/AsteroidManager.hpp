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

		gameObject.AddComponent<EntityAsteroid>();
		gameObject.GetComponent<EntityAsteroid>().Start();
		gameObject.GetComponent<EntityAsteroid>().transform = transform;
		gameObject.GetComponent<EntityAsteroid>().name = name;
		gameObject.GetComponent<EntityAsteroid>().collider = collider;

		spawnedAsteroids.push_back(gameObject.GetComponent<EntityAsteroid>());
		EntityManager::RegisterEntity(gameObject.GetComponent<EntityAsteroid>());
	}
}

std::vector<EntityAsteroid> AsteroidManager::spawnedAsteroids;

#endif // !ASTEROID_MANAGER_HPP