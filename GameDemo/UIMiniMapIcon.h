#pragma once
#include "Entity.h"
#include "IGraphics.h"

/// <summary>
/// UI MiniMap Icon
/// 
/// 24.02.21. ±è¼±¿õ
/// </summary>

class CameraModule;

class UIMiniMapIcon : public Entity
{
public:
	UIMiniMapIcon();
	UIMiniMapIcon(const std::shared_ptr<UIMiniMapIcon>& other);
	~UIMiniMapIcon();
public:
	virtual void Start() override;
};

