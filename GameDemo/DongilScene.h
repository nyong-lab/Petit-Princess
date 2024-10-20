#pragma once

#include <Scene.h>

class IGraphics;

class DongilScene : public Scene
{
public:
	DongilScene(std::wstring sceneName);

	virtual void Start() override;
	void Update(double dt);

	// �ִϸ��̼� �׽�Ʈ��
	std::shared_ptr<Entity> player;

private:
};

