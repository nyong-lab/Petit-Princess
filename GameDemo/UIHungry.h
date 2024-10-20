#pragma once
#include "Entity.h"
#include "IGraphics.h"

/// <summary>
/// UI hungry
/// 
/// 24.02.19. ±è¼±¿õ
/// </summary>

class CameraModule;

class UIHungry : public Entity
{
public:
	UIHungry();
	UIHungry(const std::shared_ptr<UIHungry>& other);
	~UIHungry();
public:
	virtual void Start() override;
};
