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
	//cheat ���� 2
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
		//magicnumber raycaster�� ���� 5
		for (int i = 0; i < 5; i++)
		{

			//RayCastInfo ray = GetOwner().lock()->GetComponent<RayCaster>(i)->RaycastClosest();
			std::vector<RayCastInfo> rayList = GetOwner().lock()->GetComponent<RayCaster>(i)->RaycastMultiple();

			RayCastInfo closest;
			double min = 10000;

			//������ ����Ǵ� �ֵ鸸 ���� �ν������� Ʈ���ſ��� �ǳʶ�
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
				//���尡���� �÷��̾��
				if (closest.collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
				{
					////����� �Ѿư����߿� ������ ��Ų��
					//if (closest.collider->GetOwner().lock()->GetComponent<PlayerFSM>()->GetState() == PlayerFSM::Player::Hide)
					//{
					//	GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::chasing);
					//}

					//����� �̹� �������̸� astar ������ ���� ���� chasing ����
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
