#include "GameDemo_pch.h"
#include "GameManager.h"

#include "MiniGameManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "SoundManager.h"

#include "Entity.h"
#include "PlayerFSM.h"
#include "Collider.h"

GameManager::GameManager() : 
	m_timer(0.0),
	m_gameTime(1000.0), 
	m_gameOver(false), 
	m_isReturn(false), 
	m_isArrest(false), 
	m_allClear(false)
{

}

GameManager::~GameManager()
{
	m_player.reset();
}

void GameManager::Update(double dt)
{
	static bool timeWariningSound = true;

	m_timer += dt;
	
	if (!m_gameOver)
	{
		///case 1 : �ð� ����
		//�⺻���� ������ �ð��� �� �Ǿ��°�?
		if (m_gameTime > 0)
		{
			m_gameTime -= dt;
			
			// �ð� ���� ����
			if (timeWariningSound && m_gameTime < 30)
			{
				timeWariningSound = false;
				SoundManager::GetInstance()->PlaySFX("TimeWarning.wav", false);
			}

			if (m_gameTime <= 0)
			{
				m_gameOver = true;
			}
		}

		///case 2 : ��񺴿��� ���� - ����� ���� ->��� ������ �˰��ΰ�
		if (!m_isArrest)
		{
			if (m_player)
			{
				if (m_player->HasComponent<PlayerFSM>())
				{
					if (m_player->GetComponent<PlayerFSM>()->GetState() == PlayerFSM::Player::Arrest)
					{
						m_isArrest = true;
						m_gameOver = true;
					}
				}
			}
		}

		///case 3 : �÷��̾ ���� �Ѵ� -> ��� �÷��̾��� ���͸� �˰��ΰ�
		//GameManager �ؿ� ���Ϳ� collider�� Ʈ���ŷ� �ߵ��ϸ� bool ������ �츮�ʿ��� �����ع�����
		if (m_isReturn)
		{
			//���� �� Ŭ���� ����? - ������ minigamemanager���� �޾ƿ���
			if (MiniGameManager::GetInstance()->IsClearMiniGame())
			{
				m_allClear = true;
			}

			m_gameOver = true;
			//������ �� ���� �ʾ����� - ����� ����
		}
	}
	else
	{
		//TODO:: scene ��ȯ?
		if (m_isArrest || !m_allClear || !m_isReturn)
		{
			//����� ����
			EventManager::GetInstance()->SceneChange(GetOwner().lock()->GetSceneManager().lock()->GetScene("Hungry"));

		}

		//��θ� ����
		if (m_allClear == true && m_isReturn == true)
		{
			EventManager::GetInstance()->SceneChange(GetOwner().lock()->GetSceneManager().lock()->GetScene("Happy"));

		}
	}

}

void GameManager::Render(double dt)
{

}

void GameManager::Finalize()
{

}

void GameManager::OnTriggerEnter(Collider* collider)
{
	auto gamemanager = GetOwner().lock();

	if (gamemanager != nullptr)
	{
		if (collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{
			m_isReturn = true;
		}
	}
}

void GameManager::OnTriggerStay(Collider* collider)
{
	
}

void GameManager::OnTriggerExit(Collider* collider)
{

}

std::shared_ptr<Component> GameManager::Clone() const
{
	std::shared_ptr<GameManager> clone = std::make_shared<GameManager>();

	return clone;
}

void GameManager::UpdateTimeInfo()
{

}
