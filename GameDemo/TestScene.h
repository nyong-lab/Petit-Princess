#pragma once
#include "Scene.h"

class IGraphics;
class CameraModule;
class CollisionManager;

class TestScene : public Scene
{
public:
	TestScene(std::wstring sceneName);

	virtual void Start() override; //�ش� ���� �ʿ��� ��ƼƼ�� ������̴�

	void Update(double dt);



private:
};

