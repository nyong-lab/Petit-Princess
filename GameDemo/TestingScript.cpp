#include "GameDemo_pch.h"
#include "TestingScript.h"

#include "InputManager.h"
#include "Transform.h"


#include "Entity.h"


#include "PlayerFSM.h"
#include "GuardFSM.h"
#include "ShopKeeperFSM.h"

#include "Collider.h"
#include "CollisionManager.h"
#include "RayCaster.h"

TestingScript::TestingScript()
{

}

TestingScript::~TestingScript()
{

}

void TestingScript::Update(double dt)
{
	float speed = 3.0f;


	if (InputManager::GetInstance()->IsKeyPress(VK_LEFT))
		//|| InputManager::GetInstance()->IsKeyDown(VK_LEFT))
	{
		if (GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{
			GetTransform()->AddPosition(Vector3(dt * -speed, 0, 0));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
		}
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_RIGHT))
		//|| InputManager::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		if (GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{
			GetTransform()->AddPosition(Vector3(dt * speed, 0, 0));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
		}
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_UP))
		//|| InputManager::GetInstance()->IsKeyDown(VK_LEFT))
	{
		if (GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{
			GetTransform()->AddPosition(Vector3(0, 0, dt * speed));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
		}
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_DOWN))
		//|| InputManager::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		if (GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{
			GetTransform()->AddPosition(Vector3(0, 0, dt * -speed));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
		}
	}



	if (InputManager::GetInstance()->IsKeyDown('I') && !InputManager::GetInstance()->IsKeyPress('I'))
	{

	}

	if (InputManager::GetInstance()->IsKeyDown('U') && !InputManager::GetInstance()->IsKeyPress('U'))
	{
	}


}

void TestingScript::Render(double dt)
{

}

void TestingScript::Finalize()
{

}

void TestingScript::OnCollisionEnter(const Collision& collision)
{

}

void TestingScript::OnTriggerEnter(Collider* collider)
{

}


std::shared_ptr<Component> TestingScript::Clone() const
{
	std::shared_ptr<TestingScript> clone = std::make_shared<TestingScript>();

	return clone;
}
