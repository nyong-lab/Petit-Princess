#pragma once
#include "Entity.h"
#include "IGraphics.h"

class CubeMap : public Entity
{
public:
	CubeMap();
	~CubeMap();

public:
	virtual void Start() override;
};

