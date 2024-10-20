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
	// ���� ��Ÿ�ӿ� �ɷȴٸ� 
	// n ���Ŀ� �ٽ� ���� ����
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
		// �÷��̾ ���� �� �ִ� �����̰�, ���� ���� ���¶��
		if (m_canPlayerHide && !(*m_isPlayerHidden))
		{
			if (m_canRenderUi)
			{
				// ���� ���� UI Ȱ��ȭ
				GetOwner().lock()->GetComponent<Transform>()->GetChild(0).lock()->GetOwner().lock()->SetRender(true);
			}

			// �� �� FŰ�� ������ ����, UI�� ����
			if (InputManager::GetInstance()->IsKeyDown('F'))
			{
				// �÷��̾� ���� true
				*m_isPlayerHidden = true;

				// �÷��̾� ���� ���� false
				m_canPlayerHide = false;

				// UI ����
				m_canRenderUi = false;
				GetOwner().lock()->GetComponent<Transform>()->GetChild(0).lock()->GetOwner().lock()->SetRender(false);

				// �������� ���̾� ���� ����
				// ���� ����� ��ȯ + ���� false
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
		// �÷��̾� ���� false
		*m_isPlayerHidden = false;	
		m_isPlayerHidden = nullptr;

		// UI ���� ����
		m_canRenderUi = true;

		// ���� ���� UI ����
		GetOwner().lock()->GetComponent<Transform>()->GetChild(0).lock()->GetOwner().lock()->SetRender(false);

		// �÷��̾� ������ ���·� ��ȯ + ���� true
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
