#pragma once
#include "Entity.h"
#include "IGraphics.h"

/// <summary>
/// UI test
/// 
/// 24.01.31. ±è¼±¿õ
/// </summary>

class CameraModule;

class UI : public Entity
{
public:
	UI();
	UI(const std::shared_ptr<UI>& other);
	~UI();
public:
	virtual void Start() override;
};

