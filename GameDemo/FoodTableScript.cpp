#include "GameDemo_pch.h"
#include "FoodTableScript.h"

// 매니저
#include "IGraphics.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "CollisionManager.h"
#include "MiniGameManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "SoundManager.h"

// 엔티티
#include "Entity.h"

// 컴포넌트
#include "Collider.h"
#include "UIRenderer.h"
#include "MeshFilter.h"

// 스크립트
#include "QuickTimeEventScript.h"
#include "PlayerMoveScript.h"
#include "GuardFSM.h"

// 스태틱 변수
int FoodTableScript::m_miniGameLastID = 1;


FoodTableScript::FoodTableScript()
	: m_foodType(FoodType::None)
	, m_miniGameID(m_miniGameLastID++)
{
	//MiniGameManager::GetInstance()->AddMiniGame(m_miniGameID);
}

FoodTableScript::FoodTableScript(FoodType type)
	: m_foodType(type)
	, m_miniGameID(m_miniGameLastID++)
{
	m_miniGameInformations = MiniGameManager::GetInstance()->GetMiniGameInfo(m_foodType);
	//MiniGameManager::GetInstance()->AddMiniGame(m_miniGameID);
}

FoodTableScript::~FoodTableScript()
{
	//MiniGameManager::GetInstance()->DestoryMiniGame(m_miniGameID);
	m_miniGameInformations.clear();
}

void FoodTableScript::OnTriggerEnter(Collider* collider)
{
	if (collider)
	{
		auto name = collider->GetOwner().lock()->GetName();
		if (name == "Player" && m_isClear == false)
		{
			// 상호작용 트리거 온
			m_isInteractiveTrigger = true;

			// 미니게임 정보 생성
			m_currentMiniGameInfo = std::make_shared<MiniGameInfo>(MiniGameManager::GetInstance()->GetStageInfo(m_foodType, 1));

			// 플레이어 허기 게이지 변수
			m_playerHunger = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetHungerPointer();

			// 플레이어 미니게임 성공 여부 
			m_isPlayerSuccessedMiniGame = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetSuccessedMiniGameBoolPointer();

			// 플레이이어 미니게임 플레이 여부
			m_isPlayerPlayMiniGame = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayMiniGameBoolPointer();

			// 플레이어 스크립트 (뒤 늦게 들고 있게 한다. (ui에서 작업 할 때, 함수 호출을 이용하기 위함)
			m_playerScript = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>();
		}

		if (collider->GetOwner().lock()->HasComponent<GuardFSM>())
			m_isGuardArrive = true;
	}
}

void FoodTableScript::OnTriggerStay(Collider* collider)
{

}

void FoodTableScript::OnTriggerExit(Collider* collider)
{
	if (collider)
	{
		auto name = collider->GetOwner().lock()->GetName();
		if (name == "Player" && m_isClear == false)
		{
			m_isInteractiveTrigger = false;
			m_currentMiniGameInfo.reset();
			m_playerHunger = nullptr;
			m_isPlayerPlayMiniGame = nullptr;
		}
	}
}

void FoodTableScript::Update(double dt)
{
	if (*m_merchantSense > 0)
		int a = 0;

	// 경비병이 도착했다면 재방문 쿨타임 시작
	if (m_isGuardArrive)
	{
		m_reVisitElapsedTime += dt;

		if (m_reVisitElapsedTime > m_reVisitEndTime)
		{
			m_reVisitElapsedTime = 0.f;
			m_isClear = false;
			m_isGuardArrive = false;
		}
	}

	// 플레이어가 음식을 먹었고, 맛집이 아니라면 재방문 쿨타임 시작
	if (m_isPlayerAteFood && !m_isDeliciousFood)
	{
		m_reVisitElapsedTime += dt;

		if (m_reVisitElapsedTime > m_reVisitEndTime)
		{
			// 음식 렌더 활성화
			if (GetTransform()->GetChild(0).lock())
				GetTransform()->GetChild(0).lock()->GetOwner().lock()->SetRender(true);

			m_reVisitElapsedTime = 0.f;
			m_isPlayerAteFood = false;
			m_isClear = false;
		}
	}

	// 플레이어가 음식을 먹었고, 맛집이라면
	// N초 뒤 상인 눈치 게이지 100% 적용
	else if (m_isPlayerAteFood && m_isDeliciousFood)
	{
		m_senseElapsedTime += dt;

		if (m_senseElapsedTime > m_merchantSenseTime)
		{
			m_senseElapsedTime = 0.f;
			*m_merchantSense = 100;
		}
	}

	// 트리거 활성화 여부 체크
	if (m_isInteractiveTrigger)
	{
		// F키를 누른다면 미니게임 시작
		if (InputManager::GetInstance()->IsKeyDown('F'))
		{
			// 미니게임 플레이를 안하고 있을 때만 가능
			{
				if (!m_isMiniGamePlay)
					m_isMiniGamePlay = true;

				if (m_isPlayerPlayMiniGame)
					*m_isPlayerPlayMiniGame = true;

				// 미니게임 스크립트를 들고있는 엔티티 생성
				// 미니게임 엔티티는 씬에 등록하지 않고 FoodTable이 관리한다.
				m_miniGameEntity = std::make_shared<Entity>("MiniGame");
				m_miniGameEntity->SetGraphics(m_owner.lock()->GetGraphics());
				m_miniGameEntity->SetManager(m_owner.lock()->GetCameraManager());
				const auto& qteScript = m_miniGameEntity->AddComponent<QuickTimeEventScript>();
				qteScript->Initialize(m_currentMiniGameInfo, m_foodType, m_merchantSense);
				auto owner = GetOwner().lock();
				m_miniGameEntity->AddComponent<UIRenderer>("MiniGameRenderer");
				m_miniGameEntity->AddComponent<MeshFilter>("MiniGameMesh")->SetModelKey(ModelKey::QTE_MiniGame);

				// 범위 밖으로 나갔다 들어오기 전까진 다시 생성 불가
				m_isInteractiveTrigger = false;

				SoundManager::GetInstance()->PlaySFX("Click.mp3");
			}
		}
	}

	// 미니게임 플레이 여부 체크
	if (m_isMiniGamePlay)
	{
		// 미니게임의 업데이트 렌더는 푸드 테이블이 관리한다.
		if (m_miniGameEntity)
			m_miniGameEntity->Update(dt);

		// 미니게임 도중에 방향키를 누른다면 
		// 미니게임 엔티티 삭제 + 미니게임 초기화
		if (InputManager::GetInstance()->IsKeyPress('W') ||
			InputManager::GetInstance()->IsKeyPress('A') ||
			InputManager::GetInstance()->IsKeyPress('S') ||
			InputManager::GetInstance()->IsKeyPress('D'))
		{
			if (m_isPlayerPlayMiniGame)
				*m_isPlayerPlayMiniGame = false;

			m_isInteractiveTrigger = true;
			m_isMiniGamePlay = false;
			m_miniGameEntity->Finalize();
			m_miniGameEntity.reset();
			m_currentMiniGameInfo.reset();
			m_currentMiniGameInfo = std::make_shared<MiniGameInfo>(MiniGameManager::GetInstance()->GetStageInfo(m_foodType, 1));
		}

		// 미니게임 마지막 단계가 끝났다면 다음 스테이지
		// 단계 관리는 미니게임에서 직접 진행한다.
		if (m_currentMiniGameInfo)
		{
			if (m_currentMiniGameInfo->progress > m_currentMiniGameInfo->lastProgress)
			{
				++m_currentMiniGameInfo->stage;
				if (m_currentMiniGameInfo->stage <= m_currentMiniGameInfo->lastStage)
				{
					m_currentMiniGameInfo = std::make_shared<MiniGameInfo>(MiniGameManager::GetInstance()->GetStageInfo(m_foodType, m_currentMiniGameInfo->stage));
					m_miniGameEntity->GetComponent<QuickTimeEventScript>()->SetMiniGameInfo(m_currentMiniGameInfo);
				}

				// 스테이지가 끝났다면 미니게임 클리어
				if (m_currentMiniGameInfo->stage > m_currentMiniGameInfo->lastStage)
				{
					// 사운드
					SoundManager::GetInstance()->PlaySFX("QTE_Success.wav", false);

					// 음식 렌더 비활성화
					if (GetTransform()->GetChild(0).lock())
						GetTransform()->GetChild(0).lock()->GetOwner().lock()->SetRender(false);

					// 플레이어의 성공여부 포인터 변수 true
					if (m_isPlayerSuccessedMiniGame)
						*m_isPlayerSuccessedMiniGame = true;

					// 플레이어 허기 게이지 증가
					if (m_playerHunger)
					{
						*m_playerHunger += 0.067f;
						if (*m_playerHunger > 1.f)
							*m_playerHunger = 1.f;
					}

					//// 미니게임 매니저에 해당 미니게임ID 에 대하여 클리어 체크를 한다.
					//MiniGameManager::GetInstance()->SetClearMiniGame(m_miniGameID);

					// 맛집이라면 클리어 카운트 증가
					// 맛집이라면 점수 2천점
					if (m_isDeliciousFood)
					{
						m_playerScript.lock()->DeliciousScoreIncrease();
						MiniGameManager::GetInstance()->ClearEatSpot(m_foodType);
					}
					else
						m_playerScript.lock()->BasicScoreIncrease();

					// 클리어에 따른 엔티티 소멸 및 클리어 체크
					m_isMiniGamePlay = false;
					m_isClear = true;
					m_miniGameEntity->Finalize();
					m_miniGameEntity.reset();
				}
			}
		}
	}
}

void FoodTableScript::Render(double dt)
{
	if (m_isMiniGamePlay)
		m_miniGameEntity->Render(dt);
}

std::shared_ptr<Component> FoodTableScript::Clone() const
{
	// 음식 가게를 클론 할지언정 그와 관련된 세트 친구들은 따로 클론시키지 않는다.
	// 음식 가게의 필터만 고르면 관련된 친구들을 모두 만들어주기 때문
	std::shared_ptr<FoodTableScript> clone = std::make_shared<FoodTableScript>();

	return clone;

	/*std::shared_ptr<FoodTableScript> clone = std::make_shared<FoodTableScript>();

	clone->m_isInteractiveTrigger = m_isInteractiveTrigger;
	clone->m_isClear = m_isClear;
	clone->m_isMiniGamePlay = false;
	clone->m_foodType = m_foodType;
	clone->m_miniGameInformations = m_miniGameInformations;

	if (m_currentMiniGameInfo)
		clone->m_currentMiniGameInfo = std::make_shared<MiniGameInfo>(*m_currentMiniGameInfo.get());
	else
		clone->m_currentMiniGameInfo = nullptr;

	clone->m_miniGameEntity = {};

	clone->m_merchantSense = nullptr;

	return clone;*/
}

void FoodTableScript::SetFoodType(FoodType foodType)
{
	m_foodType = foodType;
	m_miniGameInformations = MiniGameManager::GetInstance()->GetMiniGameInfo(m_foodType);
}