#include "GameDemo_pch.h"
#include "HideInteractionScript.h"

#include "InputManager.h"
#include "SoundManager.h"

#include "Collider.h"
#include "Entity.h"
#include "Transform.h"
#include "PlayerFSM.h"
#include "PlayerMoveScript.h"


void HideInteractionScript::Update(double dt)
{
	// 숨기 쿨타임에 걸렸다면 
	// n 초후에 다시 숨기 가능
	if (!m_canPlayerHide)
	{
		static float hideCoolTimeElapsedTime = 0.f;
		hideCoolTimeElapsedTime += dt;

		if (hideCoolTimeElapsedTime > m_hideCoolTime)
		{
			hideCoolTimeElapsedTime = 0.f;
			m_canPlayerHide = true;
		}
	}
}

void HideInteractionScript::OnTriggerEnter(Collider* collider)
{
	if (!collider) return;

	if (collider->GetOwner().lock()->GetName() == "Player")
		m_isPlayerHidden = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayerHiddenPointer();
}

void HideInteractionScript::OnTriggerStay(Collider* collider)
{
	if (!collider) return;

	if (collider->GetOwner().lock()->GetName() == "Player")
	{
		// 플레이어가 숨을 수 있는 상태이고, 숨지 않은 상태라면
		if (m_canPlayerHide && !(*m_isPlayerHidden))
		{
			if (m_canRenderUi)
			{
				// 숨기 가능 UI 활성화
				GetOwner().lock()->GetComponent<Transform>()->GetChild(0).lock()->GetOwner().lock()->SetRender(true);
			}

			// 이 때 F키를 누르면 숨고, UI도 꺼짐
			if (InputManager::GetInstance()->IsKeyDown('F'))
			{
				// 플레이어 숨기 true
				*m_isPlayerHidden = true;

				// 플레이어 숨기 가능 false
				m_canPlayerHide = false;

				// UI 끄기
				m_canRenderUi = false;
				GetOwner().lock()->GetComponent<Transform>()->GetChild(0).lock()->GetOwner().lock()->SetRender(false);

				// 실질적인 레이어 숨기 로직
				// 상태 숨기로 변환 + 렌더 false
				if (collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
				{
					collider->GetOwner().lock()->SetRender(false);
					collider->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Hide);

					SoundManager::GetInstance()->PlaySFX("Hide_wood.wav");
				}
			}
		}
	}
}

void HideInteractionScript::OnTriggerExit(Collider* collider)
{
	if (!collider) return;

	if (collider->GetOwner().lock()->GetName() == "Player")
	{
		// 플레이어 숨기 false
		*m_isPlayerHidden = false;	
		m_isPlayerHidden = nullptr;

		// UI 렌더 가능
		m_canRenderUi = true;

		// 숨기 가능 UI 끄기
		GetOwner().lock()->GetComponent<Transform>()->GetChild(0).lock()->GetOwner().lock()->SetRender(false);

		// 플레이어 움직임 상태로 변환 + 렌더 true
		if (collider)
		{
			if (collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
			{
				collider->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
				collider->GetOwner().lock()->SetRender(true);
			}
		}
	}
}

std::shared_ptr<Component> HideInteractionScript::Clone() const
{
	std::shared_ptr<HideInteractionScript> clone = std::make_shared<HideInteractionScript>();

	clone->m_canPlayerHide;
	clone->m_hideCoolTime;

	return clone;
}
