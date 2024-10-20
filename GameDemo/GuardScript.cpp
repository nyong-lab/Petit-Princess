#include "GameDemo_pch.h"
#include "GuardScript.h"

#include "InputManager.h"
#include "Transform.h"

#include "Entity.h"
#include "GuardFSM.h"
#include "PlayerFSM.h"


#include "Collider.h"
#include "CollisionManager.h"
#include "RayCaster.h"


///------------
#include "ShopKeeperFSM.h"


GuardScript::GuardScript()
{

}

GuardScript::~GuardScript()
{

}

void GuardScript::Update(double dt)
{
	//cheat 숫자 2
	if (InputManager::GetInstance()->IsKeyDown(0x32) && !InputManager::GetInstance()->IsKeyPress(0x32))
	{

		if (GetOwner().lock()->GetComponent<GuardFSM>() != nullptr)
		{
			//GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::called);
			GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::fishinged);
		}
	}


	if (GetOwner().lock()->GetComponent<RayCaster>() != nullptr)
	{
		//magicnumber raycaster의 갯수 5
		for (int i = 0; i < 5; i++)
		{

			//RayCastInfo ray = GetOwner().lock()->GetComponent<RayCaster>(i)->RaycastClosest();
			std::vector<RayCastInfo> rayList = GetOwner().lock()->GetComponent<RayCaster>(i)->RaycastMultiple();

			RayCastInfo closest;
			double min = 10000;

			//물리가 적용되는 애들만 검출 부쉬같은건 트리거여서 건너뜀
			for (auto& ray : rayList)
			{
				if (ray.distance < min)
				{
					min = ray.distance;
					closest = ray;
				}
			}

			if (closest.collider != nullptr)
			{
				//가장가까운게 플레이어다
				if (closest.collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
				{
					////경비병이 쫓아가는중에 숨으면 들킨다
					//if (closest.collider->GetOwner().lock()->GetComponent<PlayerFSM>()->GetState() == PlayerFSM::Player::Hide)
					//{
					//	GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::chasing);
					//}

					//경비병이 이미 순찰중이면 astar 갱신을 위한 상태 chasing 갱신
					if (GetOwner().lock()->GetComponent<GuardFSM>()->GetCurState() == GuardFSM::Guard::chasing)
					{
						GetOwner().lock()->GetComponent<GuardFSM>()->SetDestination(closest.collider->GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition());
						GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::chasing);
					}
					else
					{
						GetOwner().lock()->GetComponent<GuardFSM>()->SetDestination(closest.collider->GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition());
						GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::set);
					}
				}
				else

				break;
			}
		}
	}




}

void GuardScript::Render(double dt)
{

}

void GuardScript::Finalize()
{

}

void GuardScript::OnCollisionEnter(const Collision& collision)
{

}

void GuardScript::OnTriggerEnter(Collider* collider)
{

}

std::shared_ptr<Component> GuardScript::Clone() const
{
	std::shared_ptr<GuardScript> clone = std::make_shared<GuardScript>();

	return clone;
}
