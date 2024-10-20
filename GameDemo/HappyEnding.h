#pragma once
#include <Scene.h>
class HappyEnding :
	public Scene
{
public:
	HappyEnding(std::wstring sceneName);

	void Start();
	void Update(double dt);
};

