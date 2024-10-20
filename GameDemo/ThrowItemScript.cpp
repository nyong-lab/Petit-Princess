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


	//TODO::���� �ϴ� �ٴڸ��ߴµ� �� �� ���� ����
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


	///�������� ȿ�� ���� - ������ �ߴµ� ������ ���? -> ��ũ��Ʈ ���� ���� �ð� ������ ���ְų� Ư��ȿ�� �ο��ϸ� �����ҵ�
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
