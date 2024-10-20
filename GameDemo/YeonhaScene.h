#pragma once

#include <Scene.h>

class IGraphics;

class YeonhaScene : public Scene
{
public:
	YeonhaScene(std::wstring scenename);

	void Start();
	void Update(double dt);

public:
	std::wstring testKey;
};

