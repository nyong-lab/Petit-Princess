#include "GameDemo_pch.h"
#include "TechniquesScript.h"

#include "Entity.h"
#include "Collider.h"
#include "PlayerMoveScript.h"
#include "InputManager.h"
#include "SoundManager.h"

void TechniquesScript::Update(double dt)
{
	// nullptr �� �ƴ϶�� ���� ���� ��� ������Ʈ�� �÷��̾�� ��ȣ�ۿ� �����ϴٴ� ����
	// && �÷��̾ �������� �� ���̶� ����ٸ� ��ȣ�ۿ� �Ұ���
	if (m_isPlayerBlockadeAction != nullptr && !m_isPlayerGetItem)
	{
		// FŰ�� ������ ��� ������Ʈ�� ��ȣ�ۿ�
		if (InputManager::GetInstance()->IsKeyDown('F'))
		{
			// ��ȣ�ۿ� �Ѵٸ� �÷��̾� n�� �ൿ����
			*m_isPlayerBlockadeAction = true;

			// ���� ��� ������ ȹ�� (����� ������ �ϳ��̹Ƿ� ī��Ʈ�� �÷���)
			++(*m_playerSkillCount);

			// �÷��̾ �������� �����.
			// �� ���� ȹ�� �����ϱ� ���� ����
			m_isPlayerGetItem = true;

			// ���ͷ�Ƽ�� Ű UI ����
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
		// ��ȣ�ۿ� Ʈ���� ��
		if(!m_isPlayerGetItem)
			m_isInteractiveTrigger = true;

		// �÷��̾��� ��� ������ ���� �ϱ����� ������ ����
		m_isPlayerBlockadeAction = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayerBlockadeActionPointer();
		m_playerSkillCount = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayerSkillCountPointer();
	}
}

void TechniquesScript::OnTriggerExit(Collider* collider)
{
	if (!collider) return;

	if (collider->GetOwner().lock()->GetName() == "Player")
	{
		// ��ȣ�ۿ� Ʈ���� ����
		if (!m_isPlayerGetItem)
			m_isInteractiveTrigger = false;

		// ������ ���� ����
		m_isPlayerBlockadeAction = nullptr;
		m_playerSkillCount = nullptr;
	}
}

std::shared_ptr<Component> TechniquesScript::Clone() const
{
	std::shared_ptr<TechniquesScript> clone = std::make_shared<TechniquesScript>();

	return clone;
}
