#ifndef IRENDERABLE_CONTAINER_HPP
#define IRENDERABLE_CONTAINER_HPP

#include <vector>
#include <memory>
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"

template<typename T, typename A, typename D, typename X, typename F>
class IRenderableContainer
{

public:
	
	virtual void InternalInit() { };
	virtual void OnInitCalled(const T& value1 = std::nullptr_t, const A& value2 = std::nullptr_t, const X& value3 = std::nullptr_t, const D& value4 = std::nullptr_t) = 0;
	virtual void OnRenderCalled(const std::shared_ptr<Camera>& camera) = 0;

	F GetOtherData()
	{
		return otherData;
	}

	std::shared_ptr<Transform> transform;

protected:
	
	F otherData;

};

#endif // !IRENDERABLE_CONTAINER_HPP