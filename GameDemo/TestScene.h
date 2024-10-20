#pragma once
#include "Scene.h"

class IGraphics;
class CameraModule;
class CollisionManager;

class TestScene : public Scene
{
public:
	TestScene(std::wstring sceneName);

	virtual void Start() override; //해당 씬에 필요한 엔티티를 만들것이다

	void Update(double dt);



private:
};

