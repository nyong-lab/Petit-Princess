#include "GameDemo_pch.h"
#include "FoodTableScript.h"

// �Ŵ���
#include "IGraphics.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "CollisionManager.h"
#include "MiniGameManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "SoundManager.h"

// ��ƼƼ
#include "Entity.h"

// ������Ʈ
#include "Collider.h"
#include "UIRenderer.h"
#include "MeshFilter.h"

// ��ũ��Ʈ
#include "QuickTimeEventScript.h"
#include "PlayerMoveScript.h"
#include "GuardFSM.h"

// ����ƽ ����
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
			// ��ȣ�ۿ� Ʈ���� ��
			m_isInteractiveTrigger = true;

			// �̴ϰ��� ���� ����
			m_currentMiniGameInfo = std::make_shared<MiniGameInfo>(MiniGameManager::GetInstance()->GetStageInfo(m_foodType, 1));

			// �÷��̾� ��� ������ ����
			m_playerHunger = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetHungerPointer();

			// �÷��̾� �̴ϰ��� ���� ���� 
			m_isPlayerSuccessedMiniGame = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetSuccessedMiniGameBoolPointer();

			// �÷����̾� �̴ϰ��� �÷��� ����
			m_isPlayerPlayMiniGame = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayMiniGameBoolPointer();

			// �÷��̾� ��ũ��Ʈ (�� �ʰ� ��� �ְ� �Ѵ�. (ui���� �۾� �� ��, �Լ� ȣ���� �̿��ϱ� ����)
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

	// ����� �����ߴٸ� ��湮 ��Ÿ�� ����
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

	// �÷��̾ ������ �Ծ���, ������ �ƴ϶�� ��湮 ��Ÿ�� ����
	if (m_isPlayerAteFood && !m_isDeliciousFood)
	{
		m_reVisitElapsedTime += dt;

		if (m_reVisitElapsedTime > m_reVisitEndTime)
		{
			// ���� ���� Ȱ��ȭ
			if (GetTransform()->GetChild(0).lock())
				GetTransform()->GetChild(0).lock()->GetOwner().lock()->SetRender(true);

			m_reVisitElapsedTime = 0.f;
			m_isPlayerAteFood = false;
			m_isClear = false;
		}
	}

	// �÷��̾ ������ �Ծ���, �����̶��
	// N�� �� ���� ��ġ ������ 100% ����
	else if (m_isPlayerAteFood && m_isDeliciousFood)
	{
		m_senseElapsedTime += dt;

		if (m_senseElapsedTime > m_merchantSenseTime)
		{
			m_senseElapsedTime = 0.f;
			*m_merchantSense = 100;
		}
	}

	// Ʈ���� Ȱ��ȭ ���� üũ
	if (m_isInteractiveTrigger)
	{
		// FŰ�� �����ٸ� �̴ϰ��� ����
		if (InputManager::GetInstance()->IsKeyDown('F'))
		{
			// �̴ϰ��� �÷��̸� ���ϰ� ���� ���� ����
			{
				if (!m_isMiniGamePlay)
					m_isMiniGamePlay = true;

				if (m_isPlayerPlayMiniGame)
					*m_isPlayerPlayMiniGame = true;

				// �̴ϰ��� ��ũ��Ʈ�� ����ִ� ��ƼƼ ����
				// �̴ϰ��� ��ƼƼ�� ���� ������� �ʰ� FoodTable�� �����Ѵ�.
				m_miniGameEntity = std::make_shared<Entity>("MiniGame");
				m_miniGameEntity->SetGraphics(m_owner.lock()->GetGraphics());
				m_miniGameEntity->SetManager(m_owner.lock()->GetCameraManager());
				const auto& qteScript = m_miniGameEntity->AddComponent<QuickTimeEventScript>();
				qteScript->Initialize(m_currentMiniGameInfo, m_foodType, m_merchantSense);
				auto owner = GetOwner().lock();
				m_miniGameEntity->AddComponent<UIRenderer>("MiniGameRenderer");
				m_miniGameEntity->AddComponent<MeshFilter>("MiniGameMesh")->SetModelKey(ModelKey::QTE_MiniGame);

				// ���� ������ ������ ������ ������ �ٽ� ���� �Ұ�
				m_isInteractiveTrigger = false;

				SoundManager::GetInstance()->PlaySFX("Click.mp3");
			}
		}
	}

	// �̴ϰ��� �÷��� ���� üũ
	if (m_isMiniGamePlay)
	{
		// �̴ϰ����� ������Ʈ ������ Ǫ�� ���̺��� �����Ѵ�.
		if (m_miniGameEntity)
			m_miniGameEntity->Update(dt);

		// �̴ϰ��� ���߿� ����Ű�� �����ٸ� 
		// �̴ϰ��� ��ƼƼ ���� + �̴ϰ��� �ʱ�ȭ
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

		// �̴ϰ��� ������ �ܰ谡 �����ٸ� ���� ��������
		// �ܰ� ������ �̴ϰ��ӿ��� ���� �����Ѵ�.
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

				// ���������� �����ٸ� �̴ϰ��� Ŭ����
				if (m_currentMiniGameInfo->stage > m_currentMiniGameInfo->lastStage)
				{
					// ����
					SoundManager::GetInstance()->PlaySFX("QTE_Success.wav", false);

					// ���� ���� ��Ȱ��ȭ
					if (GetTransform()->GetChild(0).lock())
						GetTransform()->GetChild(0).lock()->GetOwner().lock()->SetRender(false);

					// �÷��̾��� �������� ������ ���� true
					if (m_isPlayerSuccessedMiniGame)
						*m_isPlayerSuccessedMiniGame = true;

					// �÷��̾� ��� ������ ����
					if (m_playerHunger)
					{
						*m_playerHunger += 0.067f;
						if (*m_playerHunger > 1.f)
							*m_playerHunger = 1.f;
					}

					//// �̴ϰ��� �Ŵ����� �ش� �̴ϰ���ID �� ���Ͽ� Ŭ���� üũ�� �Ѵ�.
					//MiniGameManager::GetInstance()->SetClearMiniGame(m_miniGameID);

					// �����̶�� Ŭ���� ī��Ʈ ����
					// �����̶�� ���� 2õ��
					if (m_isDeliciousFood)
					{
						m_playerScript.lock()->DeliciousScoreIncrease();
						MiniGameManager::GetInstance()->ClearEatSpot(m_foodType);
					}
					else
						m_playerScript.lock()->BasicScoreIncrease();

					// Ŭ��� ���� ��ƼƼ �Ҹ� �� Ŭ���� üũ
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
	// ���� ���Ը� Ŭ�� �������� �׿� ���õ� ��Ʈ ģ������ ���� Ŭ�н�Ű�� �ʴ´�.
	// ���� ������ ���͸� ���� ���õ� ģ������ ��� ������ֱ� ����
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