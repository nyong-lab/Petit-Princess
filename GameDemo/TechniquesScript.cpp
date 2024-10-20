#include "GameDemo_pch.h"
#include "TechniquesScript.h"

#include "Entity.h"
#include "Collider.h"
#include "PlayerMoveScript.h"
#include "InputManager.h"
#include "SoundManager.h"

void TechniquesScript::Update(double dt)
{
	// nullptr 이 아니라는 뜻은 현재 기술 오브젝트와 플레이어는 상호작용 가능하다는 상태
	// && 플레이어가 아이템을 한 번이라도 얻었다면 상호작용 불가능
	if (m_isPlayerBlockadeAction != nullptr && !m_isPlayerGetItem)
	{
		// F키를 누르면 기술 오브젝트와 상호작용
		if (InputManager::GetInstance()->IsKeyDown('F'))
		{
			// 상호작용 한다면 플레이어 n초 행동봉쇄
			*m_isPlayerBlockadeAction = true;

			// 랜덤 기술 아이템 획득 (현재는 아이템 하나이므로 카운트만 올려줌)
			++(*m_playerSkillCount);

			// 플레이어가 아이템을 얻었다.
			// 한 번만 획득 가능하기 위한 변수
			m_isPlayerGetItem = true;

			// 인터랙티브 키 UI 끄기
			m_isInteractiveTrigger = false;

			SoundManager::GetInstance()->PlaySFX("Click.mp3");
		}
	}
}

void TechniquesScript::OnTriggerEnter(Collider* collider)
{
	if (!collider) return;

	if (collider->GetOwner().lock()->GetName() == "Player")
	{
		// 상호작용 트리거 온
		if(!m_isPlayerGetItem)
			m_isInteractiveTrigger = true;

		// 플레이어의 멤버 변수를 조작 하기위한 포인터 연결
		m_isPlayerBlockadeAction = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayerBlockadeActionPointer();
		m_playerSkillCount = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayerSkillCountPointer();
	}
}

void TechniquesScript::OnTriggerExit(Collider* collider)
{
	if (!collider) return;

	if (collider->GetOwner().lock()->GetName() == "Player")
	{
		// 상호작용 트리거 오프
		if (!m_isPlayerGetItem)
			m_isInteractiveTrigger = false;

		// 포인터 연결 해제
		m_isPlayerBlockadeAction = nullptr;
		m_playerSkillCount = nullptr;
	}
}

std::shared_ptr<Component> TechniquesScript::Clone() const
{
	std::shared_ptr<TechniquesScript> clone = std::make_shared<TechniquesScript>();

	return clone;
}
