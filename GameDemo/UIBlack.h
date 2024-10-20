#pragma once
#include "Entity.h"
#include "IGraphics.h"

/// <summary>
/// UI Gauge
/// 
/// 24.02.05. ±è¼±¿õ
/// </summary>

class CameraModule;

class UIBlack : public Entity
{
public:
	UIBlack();
	UIBlack(const std::shared_ptr<UIBlack>& other);
	~UIBlack();
public:
	virtual void Start() override;
};

