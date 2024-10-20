#pragma once

#include <Scene.h>

class IGraphics;

class DongilScene : public Scene
{
public:
	DongilScene(std::wstring sceneName);

	virtual void Start() override;
	void Update(double dt);

	// 애니메이션 테스트용
	std::shared_ptr<Entity> player;

private:
};

