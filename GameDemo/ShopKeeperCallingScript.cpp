#include "GameDemo_pch.h"
#include "ShopKeeperCallingScript.h"
#include "InputManager.h"
#include "Transform.h"

#include "Entity.h"

#include "GuardFSM.h"
#include "ShopKeeperFSM.h"

#include "Collider.h"
#include "CollisionManager.h"
#include "RayCaster.h"

ShopKeeperCallingScript::ShopKeeperCallingScript()
{

}

ShopKeeperCallingScript::~ShopKeeperCallingScript()
{

}

void ShopKeeperCallingScript::Update(double dt)
{
	auto shopkeeper = GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock();
	if (shopkeeper != nullptr)
	{
		if (shopkeeper->GetComponent<ShopKeeperFSM>()->GetEnumState() == ShopKeeperFSM::ShopKeeper::idle)
		{
			m_isCalled = false;
		}
	}

}

void ShopKeeperCallingScript::Render(double dt)
{

}

void ShopKeeperCallingScript::Finalize()
{

}

void ShopKeeperCallingScript::OnCollisionEnter(const Collision& collision)
{

}

void ShopKeeperCallingScript::OnCollisionStay(const Collision& collision)
{

}

void ShopKeeperCallingScript::OnTriggerEnter(Collider* collider)
{

}

void ShopKeeperCallingScript::OnTriggerStay(Collider* collider)
{

	if (!m_isCalled)
	{
		//������ ȣ������� ����� ȣ���ϱ�
		auto shopkeeper = GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock();

		if (shopkeeper != nullptr)
		{
			if (shopkeeper->GetComponent<ShopKeeperFSM>()->GetEnumState() == ShopKeeperFSM::ShopKeeper::calling)
			{
				//������ ȣ������� ����� �����ϸ�
				std::string name = collider->GetOwner().lock()->GetName();
				if (collider->GetOwner().lock()->GetName() == "Guard")
				{
					if (collider->GetOwner().lock()->GetComponent<GuardFSM>() != nullptr)
					{
						//TODO::������ ���̺� ������ ������ ������ �ʿ���
						collider->GetOwner().lock()->GetComponent<GuardFSM>()->SetDestination(GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition());
						collider->GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::called);
						m_isCalled = true;
					}
				}
			}
		}
	}
}

std::shared_ptr<Component> ShopKeeperCallingScript::Clone() const
{
	std::shared_ptr<ShopKeeperCallingScript> clone = std::make_shared<ShopKeeperCallingScript>();

	return clone;
}
