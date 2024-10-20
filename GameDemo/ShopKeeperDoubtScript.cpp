#include "GameDemo_pch.h"
#include "ShopKeeperDoubtScript.h"

#include "InputManager.h"
#include "Transform.h"

#include "Entity.h"

#include "GuardFSM.h"
#include "ShopKeeperFSM.h"

#include "Collider.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "RayCaster.h"
///----
#include "PlayerFSM.h"

ShopKeeperDoubtScript::ShopKeeperDoubtScript()
{

}

ShopKeeperDoubtScript::~ShopKeeperDoubtScript()
{

}

void ShopKeeperDoubtScript::Update(double dt)
{
	// ��ġ ������ ���尡 �︰ ���¶�� 
	// ��ġ �������� 0���� ���������� üũ�ؼ� ���� �ٽ� ������ �Ѵ�.
	if (m_SenseOnSound)
	{
		double senseCurrentValue = m_owner.lock()->GetComponent<ShopKeeperFSM>()->GetSense();
		if (senseCurrentValue == 0)
			m_SenseOnSound = false;
		if (senseCurrentValue != 100)
			m_SenseLookingSound = false;
	}


	auto shopkeeper = GetOwner().lock();

	//cheat ���� 3��
	if (InputManager::GetInstance()->IsKeyDown(0x33) && !InputManager::GetInstance()->IsKeyPress(0x33))
	{
		if (shopkeeper->GetComponent<ShopKeeperFSM>() != nullptr)
		{
			shopkeeper->GetComponent<ShopKeeperFSM>()->ChangeState(ShopKeeperFSM::ShopKeeper::calling);
		}
	}


	if (GetOwner().lock()->GetComponent<RayCaster>() != nullptr)
	{
		bool check[5] = { false,false,false,false,false };


		//magicnumber raycaster�� ���� 5
		for (int i = 0; i < 5; i++)
		{
			//ray�� ������ ��ü���� ���
			std::vector<RayCastInfo> rayVector = GetOwner().lock()->GetComponent<RayCaster>(i)->RaycastMultiple();


			std::vector<RayCastInfo>::iterator itr = rayVector.begin();

			//����� ��ȭ�ϸ鼭 �ش� ��ü�� �÷��̾����� Ȯ���ϰڴ�
			for (auto ray : rayVector)
			{
				if (ray.collider != nullptr)
				{
					//�÷��̾�� ���� ��ȯ!
					if (ray.collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
					{
						// ���� ������ ��� ����
						if (!m_SenseOnSound)
						{
							m_SenseOnSound = true;
							SoundManager::GetInstance()->PlaySFX("MerChantSense_On.mp3", false);
						}

						// ��ġ �������� 100�̶�� ����
						if (!m_SenseLookingSound)
						{
							if (m_owner.lock()->GetComponent<ShopKeeperFSM>()->GetSense() == 100)
							{
								m_SenseLookingSound = true;
								SoundManager::GetInstance()->PlaySFX("MerChantSense_Looking.mp3", false);
							}
						}
						

						//������ ȣ�� ����?
						if (GetOwner().lock()->GetComponent<ShopKeeperFSM>()->GetEnumState() != ShopKeeperFSM::ShopKeeper::calling)
						{
							check[i] = true;
							break;
						}
					}
				}
			}
		}

		//��� ���̿��� �÷��̾ ����Ǿ���?
		if (check[0] || check[1] || check[2] || check[3] || check[4])
		{
			shopkeeper->GetComponent<ShopKeeperFSM>()->ChangeState(ShopKeeperFSM::ShopKeeper::doubt);
		}
		else
		{
			if (GetOwner().lock()->GetComponent<ShopKeeperFSM>()->GetEnumState() == ShopKeeperFSM::ShopKeeper::doubt)
			{
				shopkeeper->GetComponent<ShopKeeperFSM>()->ChangeState(ShopKeeperFSM::ShopKeeper::idle);
			}
		}
	}
}

void ShopKeeperDoubtScript::Render(double dt)
{

}

void ShopKeeperDoubtScript::Finalize()
{

}

void ShopKeeperDoubtScript::OnCollisionEnter(const Collision& collision)
{

}

void ShopKeeperDoubtScript::OnCollisionStay(const Collision& collision)
{

}

void ShopKeeperDoubtScript::OnTriggerEnter(Collider* collider)
{

}

void ShopKeeperDoubtScript::OnTriggerStay(Collider* collider)
{
	//auto shopkeeper = GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock();

	////������ ��ġ������ �ִ� 
	//if (shopkeeper != nullptr)
	//{
	//	//�÷��̾ ���ĸ԰� ������ ������ �ǽ��ϴ� ���°� �ƴϸ� �ǽɻ��·� �ٲ���
	//	if (shopkeeper->GetComponent<ShopKeeperFSM>()->GetEnumState() != ShopKeeperFSM::ShopKeeper::doubt)
	//	{
	//		//�ǽɻ��´� ������ �ǽ� ������ �÷��̾ ������ ������ ��ġ�������� ����Ѵ�
	//		std::string name = collider->GetOwner().lock()->GetName();
	//		if (collider->GetOwner().lock()->GetName() == "Player")
	//		{
	//			shopkeeper->GetComponent<ShopKeeperFSM>()->ChangeState(ShopKeeperFSM::ShopKeeper::doubt);
	//		}
	//	}

	//	//�ϴ� ����
	//	//��� ȣ�� �����϶� ����� �ش� ������ �ֺ��� �����ϸ� �ֺ��� �θ����Ÿ���
	//	//if (shopkeeper->GetComponent<ShopKeeperFSM>()->GetEnumState() == ShopKeeperFSM::ShopKeeper::calling)
	//	//{
	//	//	//����� ��ó�� ������ �θ��� �Ÿ��� �ֺ��� ã�ƾ��Ѵ�
	//	//	std::string name = collider->GetOwner().lock()->GetName();
	//	//	if (collider->GetOwner().lock()->GetName() == "Guard")
	//	//	{
	//	//		collider->GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::search);
	//	//	}
	//	//}
	//}
}

std::shared_ptr<Component> ShopKeeperDoubtScript::Clone() const
{
	std::shared_ptr<ShopKeeperDoubtScript> clone = std::make_shared<ShopKeeperDoubtScript>();

	return clone;
}