#include "GameDemo_pch.h"
#include "UIDataSyncScript.h"

#include "Entity.h"
#include "Transform.h"
#include "UITestScript.h"

void UIDataSyncScript::Start()
{
	/*m_player = GetOwner().lock()->GetComponent<Transform>()->GetChild(1).lock()->GetOwner().lock();
	m_minimapCurPos = GetOwner().lock()->GetComponent<Transform>()->GetChild(0).lock()->GetOwner().lock();*/

	for (const auto& child : GetOwner().lock()->GetComponent<Transform>()->GetChildren())
	{
		if (child->GetOwner().lock()->GetName() == "Player")
			m_player = child->GetOwner().lock();
		else
			m_minimapCurPos = child->GetOwner().lock();
	}
}

void UIDataSyncScript::Update(double dt)
{	
	

	if (m_minimapCurPos->HasComponent<UITestScript>())
	{
		if (m_minimapCurPos->GetComponent<UITestScript>()->GetIsMinimapOn())
		{
			float x_flat = m_player->GetComponent<Transform>()->GetWorldPosition().x;
			float y_flat = m_player->GetComponent<Transform>()->GetWorldPosition().z;

			// 맵상 좌표로 변환
			x_flat = (450.f / 150.f) * x_flat - (450.f / 2.f); // 450 462 이 숫자는 UIRect에서 설정해준 맵 이미지크기
			y_flat = (462.f / 150.f) * y_flat - (462.f / 2.f);

			m_minimapCurPos->GetComponent<Transform>()->SetWorldPosition(Vector3(x_flat, y_flat, 0.f));
		}
	}
}

std::shared_ptr<Component> UIDataSyncScript::Clone() const
{
	std::shared_ptr<UIDataSyncScript> clone = std::make_shared<UIDataSyncScript>();

	return clone;
}

void UIDataSyncScript::OnTriggerEnter(Collider* collider)
{
}

void UIDataSyncScript::OnTriggerStay(Collider* collider)
{
}

void UIDataSyncScript::OnTriggerExit(Collider* collider)
{
}
