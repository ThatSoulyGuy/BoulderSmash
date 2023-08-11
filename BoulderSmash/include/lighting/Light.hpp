#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "rendering/Camera.hpp"
 
class Light
{

public:
	
	void GenerateLight()
	{
		//if(DrawOutline())
			//renderer.GenerateTestObject("lightObject", transform.position);

		transform.position = glm::vec3{ 0, 0, 0 };
		transform.rotation = glm::vec3{ 0, 0, 0 };

		//Renderer::RegisterRenderableObject(renderer);
	}

	virtual bool DrawOutline() = 0;

	Transform GetTransform() const
	{
		return transform;
	}

	std::string name;
	Transform transform;

private:

	//RenderableObject renderer;

};

#endif // !LIGHT_HPP