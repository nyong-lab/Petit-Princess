#pragma once
#include <Scene.h>
class IGraphics;

class SeoheeScene : public Scene
{
public:
	SeoheeScene(std::wstring scenename);

	virtual void Start() override;
	void Update(double dt) override;
};

