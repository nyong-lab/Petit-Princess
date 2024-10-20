#pragma once
#include "Entity.h"
#include "IGraphics.h"

/// <summary>
/// UI hungry
/// 
/// 24.02.19. ±è¼±¿õ
/// </summary>

class CameraModule;

class UISkill : public Entity
{
public:
	UISkill();
	UISkill(const std::shared_ptr<UISkill>& other);
	~UISkill();
public:
	virtual void Start() override;
};
