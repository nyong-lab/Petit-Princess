#include "GameDemo_pch.h"
#include "InterKeyScript.h"

#include "FoodTableScript.h"
#include "TechniquesScript.h"

#include "Entity.h"

void InterKeyScript::Update(double dt)
{

}

void InterKeyScript::Render(double dt)
{

}

std::shared_ptr<Component> InterKeyScript::Clone() const
{
	// 음식 가게를 클론 할지언정 그와 관련된 세트 친구들은 따로 클론시키지 않는다.
	// 음식 가게의 필터만 고르면 관련된 친구들을 모두 만들어주기 때문
	std::shared_ptr<InterKeyScript> clone = std::make_shared<InterKeyScript>();

	return clone;
}

bool InterKeyScript::GetInteractive()
{
	if (m_foodTableScript)
		return m_foodTableScript->GetInteractive();
	else if (m_techniquesScript)
		return m_techniquesScript->GetInteractive();
	else 
		return false;
}

DirectX::SimpleMath::Vector3 InterKeyScript::GetObjectPosition()
{
	return GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();
}
