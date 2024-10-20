#include "GameDemo_pch.h"
#include "FishRangeScript.h"
#include "Entity.h"
#include "GuardFSM.h"
#include "Collider.h"

#include "EventManager.h"

void FishRangeScript::Serialize(nlohmann::json& json)
{

}

void FishRangeScript::Deserialize(const nlohmann::json& json)
{

}

FishRangeScript::~FishRangeScript()
{

}

void FishRangeScript::Update(double dt)
{
	if (GetOwner().lock() != nullptr)
	{
		static double time = 0;
		time += dt;

		if (m_duration < time)
		{
			time = 0;
			EventManager::GetInstance()->AddEvent(GetOwner().lock(), EventType::Destory);
		}
	}



}

void FishRangeScript::Render(double dt)
{

}

void FishRangeScript::Finalize()
{

}

void FishRangeScript::OnCollisionEnter(const Collision& collision)
{

}

void FishRangeScript::OnCollisionStay(const Collision& collision)
{

}

void FishRangeScript::OnCollisionExit(const Collision& collision)
{

}

void FishRangeScript::OnTriggerEnter(Collider* collider)
{
	if (collider->GetOwner().lock()->GetComponent<GuardFSM>() != nullptr)
	{
		collider->GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::fishinged);
	}
}

void FishRangeScript::OnTriggerStay(Collider* collider)
{
	
}

void FishRangeScript::OnTriggerExit(Collider* collider)
{

}

std::shared_ptr<Component> FishRangeScript::Clone() const
{
	std::shared_ptr<FishRangeScript> clone = std::make_shared<FishRangeScript>();

	return clone;
}
