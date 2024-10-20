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
		///case 1 : 시간 종료
		//기본적인 게임의 시간이 다 되었는가?
		if (m_gameTime > 0)
		{
			m_gameTime -= dt;
			
			// 시간 부족 사운드
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

		///case 2 : 경비병에게 잡힘 - 배고픈 엔딩 ->어떻게 잡힌걸 알것인가
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

		///case 3 : 플레이어가 복귀 한다 -> 어떻게 플레이어의 복귀를 알것인가
		//GameManager 밑에 복귀용 collider가 트리거로 발동하면 bool 값ㄷ을 우리쪽에서 셋팅해버린다
		if (m_isReturn)
		{
			//맛집 다 클리어 했음? - 동일이 minigamemanager에서 받아오자
			if (MiniGameManager::GetInstance()->IsClearMiniGame())
			{
				m_allClear = true;
			}

			m_gameOver = true;
			//맛집을 다 돌지 않았으면 - 배고픈 엔딩
		}
	}
	else
	{
		//TODO:: scene 전환?
		if (m_isArrest || !m_allClear || !m_isReturn)
		{
			//배고픈 엔딩
			EventManager::GetInstance()->SceneChange(GetOwner().lock()->GetSceneManager().lock()->GetScene("Hungry"));

		}

		//배부른 엔딩
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
