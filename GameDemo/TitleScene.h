#pragma once
#include <Scene.h>
class TitleScene :
    public Scene
{
public:
	TitleScene(std::wstring sceneName);

	void Start();
	void Update(double dt);

private:
};

