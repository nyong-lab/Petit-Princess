#include "GameDemo_pch.h"
#include "UserScript.h"

#include "InputManager.h"
#include "Transform.h"

void UserScript::Update(double dt)
{
	if (InputManager::GetInstance()->IsKeyPress(VK_LEFT))
		//|| InputManager::GetInstance()->IsKeyDown(VK_LEFT))
	{
		GetTransform()->AddLocalPosition(Vector3(dt*-1.0f, 0, 0));
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_RIGHT))
		//|| InputManager::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		GetTransform()->AddLocalPosition(Vector3(dt * 1.0f, 0, 0));
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_UP))
		//|| InputManager::GetInstance()->IsKeyDown(VK_LEFT))
	{
		GetTransform()->AddLocalPosition(Vector3(0,0,dt * 1.0f));
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_DOWN))
		//|| InputManager::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		GetTransform()->AddLocalPosition(Vector3(0,0,dt * -1.0f));
	}
}

void UserScript::Render()
{

}

void UserScript::Finalize()
{

}

std::shared_ptr<Component> UserScript::Clone() const
{
	std::shared_ptr<UserScript> clone = std::make_shared<UserScript>();

	return clone;
}