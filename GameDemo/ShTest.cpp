#include "GameDemo_pch.h"
#include "ShTest.h"
#include "InputManager.h"
#include "Transform.h"

ShTest::ShTest()
{

}

ShTest::~ShTest()
{

}

void ShTest::Update(double dt)
{
	auto tr = GetTransform();
	float speed = 0.05f;

	if (InputManager::GetInstance()->IsKeyPress(VK_UP))
	{
		tr->AddPosition(Vector3(0,0,speed));
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_DOWN))
	{
		tr->AddPosition(Vector3(0, 0, -speed));
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_LEFT))
	{
		tr->AddPosition(Vector3(-speed, 0, 0));
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_RIGHT))
	{
		tr->AddPosition(Vector3(speed, 0, 0));
	}
}

void ShTest::Render(double dt)
{

}

void ShTest::Finalize()
{

}
