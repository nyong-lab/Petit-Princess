#include "GameDemo_pch.h"
#include "ThrowItemScript.h"
#include "EventManager.h"
#include "Entity.h"
#include "Collider.h"
#include "GuardFSM.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "FishRangeScript.h"
#include "SoundManager.h"


void ThrowItemScript::Serialize(nlohmann::json& json)
{

}

void ThrowItemScript::Deserialize(const nlohmann::json& json)
{

}

ThrowItemScript::~ThrowItemScript()
{

}

void ThrowItemScript::Update(double dt)
{

}

void ThrowItemScript::Render(double dt)
{

}

void ThrowItemScript::Finalize()
{

}

void ThrowItemScript::OnCollisionEnter(const Collision& collision)
{
	if (collision.otherCollider->GetOwner().lock()->GetComponent<GuardFSM>() != nullptr)
	{
		//collision.otherCollider->GetOwner().lock()->GetComponent<GuardFSM>()->ChangeState(GuardFSM::Guard::fishinged);


		auto flooring = std::make_shared<Entity>("");
		flooring->SetName("itemRange");
		flooring->AddComponent<SphereCollider>();
		flooring->AddComponent<FishRangeScript>();
		flooring->GetComponent<SphereCollider>()->SetRadius(3.0f);
		flooring->GetComponent<SphereCollider>()->SetDisableGravity(true);
		flooring->GetComponent<SphereCollider>()->SetIsTrigger(true);
		flooring->GetComponent<Transform>()->SetLocalPosition(GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition());
		EventManager::GetInstance()->AddEvent(flooring, EventType::Create);

		EventManager::GetInstance()->AddEvent(GetOwner().lock(), EventType::Destory);
		//SoundManager::GetInstance()->PlaySFX("Postion_Broken.wav");
	}


	//TODO::판정 일단 바닥만했는데 더 들어갈 수도 있음
	if (collision.otherCollider->GetOwner().lock()->GetComponent<PlaneCollider>() != nullptr)
	{
		auto flooring = std::make_shared<Entity>("");
		flooring->SetName("itemRange");
		flooring->AddComponent<SphereCollider>();
		flooring->AddComponent<FishRangeScript>();
		flooring->GetComponent<SphereCollider>()->SetRadius(3.0f);
		flooring->GetComponent<SphereCollider>()->SetDisableGravity(true);
		flooring->GetComponent<SphereCollider>()->SetIsTrigger(true);
		flooring->GetComponent<SphereCollider>()->SetBodyType(BodyType::Static);
		flooring->GetComponent<Transform>()->SetLocalPosition(GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition());
		EventManager::GetInstance()->AddEvent(flooring, EventType::Create);

		EventManager::GetInstance()->AddEvent(GetOwner().lock(), EventType::Destory);
		SoundManager::GetInstance()->PlaySFX("Postion_Broken.wav");
	}


	///아이템의 효과 영역 - 생성은 했는데 삭제는 어떻게? -> 스크립트 만들어서 일정 시간 지나면 없애거나 특정효과 부여하면 가능할듯
	/*auto flooring = std::make_shared<Entity>("");
	flooring->SetName("itemRange");
	flooring->AddComponent<SphereCollider>();
	flooring->GetComponent<SphereCollider>()->SetRadius(3.0f);
	flooring->GetComponent<SphereCollider>()->SetDisableGravity(true);
	flooring->GetComponent<SphereCollider>()->SetIsTrigger(true);
	flooring->GetComponent<Transform>()->SetLocalPosition(GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition());

	EventManager::GetInstance()->AddEvent(flooring, EventType::Create);*/

}

void ThrowItemScript::OnTriggerEnter(Collider* collider)
{

}

void ThrowItemScript::OnTriggerStay(Collider* collider)
{

}

void ThrowItemScript::OnTriggerExit(Collider* collider)
{

}

std::shared_ptr<Component> ThrowItemScript::Clone() const
{
	std::shared_ptr<ThrowItemScript> clone = std::make_shared<ThrowItemScript>();

	return clone;
}
