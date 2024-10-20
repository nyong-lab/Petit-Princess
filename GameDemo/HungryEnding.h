#pragma once
#include <Scene.h>
class HungryEnding :
    public Scene
{
public:
	HungryEnding(std::wstring sceneName);

	void Start();
	void Update(double dt);
};

