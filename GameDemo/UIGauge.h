#pragma once
#include "Entity.h"
#include "IGraphics.h"

/// <summary>
/// UI Gauge
/// 
/// 24.02.05. ±è¼±¿õ
/// </summary>

class CameraModule;

class UIGauge : public Entity
{
public:
	UIGauge();
	UIGauge(const std::shared_ptr<UIGauge>& other);
	~UIGauge();
public:
	virtual void Start() override;
};

