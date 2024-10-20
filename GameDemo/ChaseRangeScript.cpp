#include "GameDemo_pch.h"
#include "ChaseRangeScript.h"

#include "Entity.h"
#include "Collider.h"
#include "PlayerFSM.h"
#include "GuardFSM.h"

ChaseRangeScript::ChaseRangeScript()
{

}

ChaseRangeScript::~ChaseRangeScript()
{

}

void ChaseRangeScript::Update(double dt)
{

}

void ChaseRangeScript::Render(double dt)
{

}

void ChaseRangeScript::Finalize()
{

}

void ChaseRangeScript::OnCollisionEnter(const Collision& collision)
{

}

void ChaseRangeScript::OnTriggerEnter(Collider* collider)
{
	if (!collider) return;

	if (GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GuardFSM>()->GetCurState()
		== GuardFSM::Guard::chasing)
	{
		// 추격중인 상태에서 플레이어가 접근 했다면 
		if (collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{

			if (GetOwner().lock()->GetParent() != nullptr)
			{
				GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GuardFSM>()->SetDestination(collider->GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition());
				GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::chasing);
			}
		}
	}
}

void ChaseRangeScript::OnTriggerStay(Collider* collider)
{
	if (!collider) return;

	if (GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GuardFSM>()->GetCurState()
		== GuardFSM::Guard::chasing)
	{
		// 추격중인 상태에서 플레이어가 접근 했다면 
		if (collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{

			if (GetOwner().lock()->GetParent() != nullptr)
			{
				GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GuardFSM>()->SetDestination(collider->GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition());
				GetOwner().lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::chasing);
			}
		}
	}
}

std::shared_ptr<Component> ChaseRangeScript::Clone() const
{
	std::shared_ptr<ChaseRangeScript> clone = std::make_shared<ChaseRangeScript>();
	return clone;
}
