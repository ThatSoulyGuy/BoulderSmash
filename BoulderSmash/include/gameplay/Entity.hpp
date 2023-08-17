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

private:



};

#endif // !ENTITY_HPP