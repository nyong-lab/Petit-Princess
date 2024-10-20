#pragma once
#include <Scene.h>

class IGraphics;

class SunWoongScene : public Scene
{
public:
	SunWoongScene(std::wstring sceneName);
	virtual void Start() override;
	void Update(double dt);
private:
};

