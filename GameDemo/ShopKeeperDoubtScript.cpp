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
	// 눈치 게이지 사운드가 울린 상태라면 
	// 눈치 게이지가 0으로 내려갔는지 체크해서 사운드 다시 나도록 한다.
	if (m_SenseOnSound)
	{
		double senseCurrentValue = m_owner.lock()->GetComponent<ShopKeeperFSM>()->GetSense();
		if (senseCurrentValue == 0)
			m_SenseOnSound = false;
		if (senseCurrentValue != 100)
			m_SenseLookingSound = false;
	}


	auto shopkeeper = GetOwner().lock();

	//cheat 숫자 3번
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


		//magicnumber raycaster의 갯수 5
		for (int i = 0; i < 5; i++)
		{
			//ray가 감지한 물체들의 목록
			std::vector<RayCastInfo> rayVector = GetOwner().lock()->GetComponent<RayCaster>(i)->RaycastMultiple();


			std::vector<RayCastInfo>::iterator itr = rayVector.begin();

			//목록을 순화하면서 해당 물체가 플레이어인지 확인하겠다
			for (auto ray : rayVector)
			{
				if (ray.collider != nullptr)
				{
					//플레이어면 상태 변환!
					if (ray.collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
					{
						// 최초 조우한 경우 사운드
						if (!m_SenseOnSound)
						{
							m_SenseOnSound = true;
							SoundManager::GetInstance()->PlaySFX("MerChantSense_On.mp3", false);
						}

						// 눈치 게이지가 100이라면 사운드
						if (!m_SenseLookingSound)
						{
							if (m_owner.lock()->GetComponent<ShopKeeperFSM>()->GetSense() == 100)
							{
								m_SenseLookingSound = true;
								SoundManager::GetInstance()->PlaySFX("MerChantSense_Looking.mp3", false);
							}
						}
						

						//상인이 호출 가능?
						if (GetOwner().lock()->GetComponent<ShopKeeperFSM>()->GetEnumState() != ShopKeeperFSM::ShopKeeper::calling)
						{
							check[i] = true;
							break;
						}
					}
				}
			}
		}

		//모든 레이에서 플레이어가 검출되었나?
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

	////상인의 눈치범위에 있는 
	//if (shopkeeper != nullptr)
	//{
	//	//플레이어가 훔쳐먹고 있을때 상인이 의심하는 상태가 아니면 의심상태로 바꾸자
	//	if (shopkeeper->GetComponent<ShopKeeperFSM>()->GetEnumState() != ShopKeeperFSM::ShopKeeper::doubt)
	//	{
	//		//의심상태는 상인의 의심 범위에 플레이어가 있으면 상인의 눈치게이지가 상승한다
	//		std::string name = collider->GetOwner().lock()->GetName();
	//		if (collider->GetOwner().lock()->GetName() == "Player")
	//		{
	//			shopkeeper->GetComponent<ShopKeeperFSM>()->ChangeState(ShopKeeperFSM::ShopKeeper::doubt);
	//		}
	//	}

	//	//일단 보류
	//	//경비병 호출 상태일때 경비병이 해당 상인의 주변에 도착하면 주변을 두리번거리자
	//	//if (shopkeeper->GetComponent<ShopKeeperFSM>()->GetEnumState() == ShopKeeperFSM::ShopKeeper::calling)
	//	//{
	//	//	//경비병이 근처에 왔으면 두리번 거리며 주변을 찾아야한다
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