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

	//TODO:: ������Ʈ�� ���� �ִ� ��ƼƼ�� �����ΰ�?
	//virtual Entity* InEntity();

};

