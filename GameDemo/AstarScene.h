#pragma once
#include <Scene.h>

class IGraphics;
class CameraModule;
class CollisionManager;

class AstarScene :  public Scene
{
public:
	AstarScene(std::wstring sceneName);

	void Start();
	void Update(double dt);

private:
};
