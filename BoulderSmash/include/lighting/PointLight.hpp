#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "lighting/Light.hpp"

class PointLight : public Light
{

public:
	
	void Register(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	glm::vec3 ambient = glm::vec3{ 0.05f, 0.05f, 0.05f };
	glm::vec3 diffuse = glm::vec3{ 0.8f, 0.8f, 0.8f };
	glm::vec3 specular = glm::vec3{ 1.0f, 1.0f, 1.0f };

	bool DrawOutline() override
	{
		return false;
	}

private:



};

#endif // !POINT_LIGHT_HPP