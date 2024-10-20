#include "GameDemo_pch.h"
#include "HideInterKeyScript.h"
#include "Entity.h"

#include "HideInteractionScript.h"

void HideInterKeyScript::Update(double dt)
{
}

void HideInterKeyScript::Render(double dt)
{
}

std::shared_ptr<Component> HideInterKeyScript::Clone() const
{
	// 음식 가게를 클론 할지언정 그와 관련된 세트 친구들은 따로 클론시키지 않는다.
// 음식 가게의 필터만 고르면 관련된 친구들을 모두 만들어주기 때문 <지원: 이해는 안가지만 복사해왔다
	std::shared_ptr<HideInterKeyScript> clone = std::make_shared<HideInterKeyScript>();

	return clone;
}

bool HideInterKeyScript::GetInteractive()
{
	if (m_hideInteractionScript)
		return m_hideInteractionScript->GetInteractive();
	//else if (m_techniquesScript)
	//	return m_techniquesScript->GetInteractive();
	else
		return false;
}

Vector3 HideInterKeyScript::GetObjectPosition()
{
	return GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();
}
