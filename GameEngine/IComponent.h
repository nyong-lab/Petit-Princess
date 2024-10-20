#pragma once
#include "IComponent.h"

class Entity;
class IGraphics;
class CameraModule;

class IComponent
{
public:
	virtual ~IComponent() {}

	virtual void Start() abstract;
	virtual void Update(double dt) abstract;
	virtual void Render(double dt) abstract;
	virtual void DebugRender() abstract;
	virtual void Finalize() abstract;

	//TODO:: 컴포넌트를 갖고 있는 엔티티가 무엇인가?
	//virtual Entity* InEntity();

};

