#include "GameDemo_pch.h"
#include "PlayerMoveScript.h"

#include "CameraManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "CameraModule.h"
#include "MathUtils.h"

#include "PlayerFSM.h"

#include "TextureBox.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MeshRenderer.h"

#include "ThrowItemScript.h"
#include "UITestScript.h"
#include "FoodTableScript.h"
#include "GuardFSM.h"
#include "ScoreUI.h"
#include "SoundManager.h"

void PlayerMoveScript::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_targetVector"] =
	{
		{"x", m_targetVector.x},
		{"y", m_targetVector.y},
		{"z", m_targetVector.z}
	};

	json["m_headVector"] =
	{
		{"x", m_headVector.x},
		{"y", m_headVector.y},
		{"z", m_headVector.z},
	};
}

void PlayerMoveScript::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null()) from_json(json, *this);

	if (json.contains("m_targetVector"))
	{
		m_targetVector.x = json["m_targetVector"]["x"];
		m_targetVector.y = json["m_targetVector"]["y"];
		m_targetVector.z = json["m_targetVector"]["z"];
	}

	if (json.contains("m_headVector"))
	{
		m_headVector.x = json["m_headVector"]["x"];
		m_headVector.y = json["m_headVector"]["y"];
		m_headVector.z = json["m_headVector"]["z"];
	}
}

void PlayerMoveScript::Start()
{
	std::shared_ptr<Entity> child = m_owner.lock()->GetSmartChild("ScoreUI").lock();
	if (child == nullptr) assert(false && L"플레이어의 자식으로 ScoreUI가 필요합니다.");
	m_scoreUI = m_owner.lock()->GetSmartChild("ScoreUI").lock()->GetComponent<ScoreUI>();
	if (m_scoreUI == nullptr) assert(false && L"플레이어의 자식으로 ScoreUI가 필요합니다.");

	// 초기 로테이션을 초기 m_headVector와 똑같이 설정해줘야 한다
	GetTransform()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
}

void PlayerMoveScript::Update(double dt)
{
	/// 테스트-------------------------------------------------------
// 	if (InputManager::GetInstance()->IsKeyDown('0'))
// 	{
// 		BasicScoreIncrease();
// 	}
// 
// 	if (InputManager::GetInstance()->IsKeyDown('9'))
// 	{
// 		DeliciousScoreIncrease();
// 	}

	/// 테스트-------------------------------------------------------
	

	// 허기 게이지 사운드
	if (m_hunger <= 0.3)
	{
		if (m_hunger <= 0.14)
		{	
			if (!m_hungerSecondSound)
			{
				SoundManager::GetInstance()->PlaySFX("Hungry_Warning.wav", false);
				m_hungerSecondSound = true;
			}
		}
		
		else if (!m_hungerSound)
		{
			SoundManager::GetInstance()->PlaySFX("Hungry_Warning.wav", false);
			m_hungerSound = true;
		}
	}
	else if (m_hunger > 0.3)
	{
		if (m_hungerSound)
		{
			m_hungerSound = false;
			m_hungerSecondSound = false;
		}
	}

	//현재 속도
	float curSpeed = 0;


	// 음식 미섭취 시간
	static float notEatingElapsedTime = 0.f;

	// 허기 감소 시간
	static float hungerDecreaseElapsedTime = 0.f;

	notEatingElapsedTime += dt;

	// n초동안 음식 미섭취 시
	if (notEatingElapsedTime > m_notEatenTime)
	{
		hungerDecreaseElapsedTime += dt;

		// n초마다 허기게이지 감소
		if (hungerDecreaseElapsedTime > m_hugerDecreaseTime)
		{
			hungerDecreaseElapsedTime = 0.f;
			m_hunger -= m_decreaseHunger;

			if (m_hunger < 0.f)
				m_hunger = 0.f;
		}
	}

	// 허기게이지 수치에 맞는 플레이어 이동 속도 조절
	// 만족 상태
	if (m_hunger >= m_hungerSatisfactionState)
		m_moveSpeed = m_satisfactionMoveSpeed;

	// 보통 상태
	else if (m_hunger >= m_hungerNormalState)
		m_moveSpeed = m_normalMoveSpeed;

	// 불만족 상태
	else
		m_moveSpeed = m_dissatisfactionMoveSpeed;

	// 음식을 섭취 후 n초간 버프
	if (m_isFoodIntake)
	{
		static float speedUpElapsedTime = 0.f;
		speedUpElapsedTime += dt;

		if (speedUpElapsedTime > m_speedUpBuffTime)
		{
			speedUpElapsedTime = 0.f;
			m_isFoodIntake = false;
		}
	}

	// 미니게임을 성공했다면 n초간 음식 먹기 (이 시간 동안 행동봉쇄)
	if (m_iSuccessedMiniGame)
	{
		static float blockadeElapsedTime = 0.f;
		blockadeElapsedTime += dt;

		GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Eat);


		if (blockadeElapsedTime > m_blockadeActionTime)
		{
			blockadeElapsedTime = 0.f;
			m_iSuccessedMiniGame = false;
			m_isFoodIntake = true;
			notEatingElapsedTime = 0.f;
			m_isPlayMiniGame = false;

			// 재방문 쿨타임 시작
			if (m_foodTableScript)
				m_foodTableScript->SetAteFood(true);

			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);
		}
	}
	// 기술 오브젝트와 상호작용하였다면 n초 행동 봉쇄
	else if (m_isBlockadeAction)
	{
		static float blockadeElapsedTime = 0.f;
		blockadeElapsedTime += dt;

		if (blockadeElapsedTime > m_blockadeActionTime)
		{
			blockadeElapsedTime = 0.f;
			m_isBlockadeAction = false;
			SoundManager::GetInstance()->PlaySFX("Potion_Get.mp3");
		}
	}
	else
	{
		// 음식을 섭취 했다면 n초간 이동속도 1.5배 버프
		if (m_isFoodIntake)
		{
			curSpeed = m_moveSpeed * 1.5f;
		}
		// 섭취 하지 않았다면 기본 속도
		else
		{
			curSpeed = m_moveSpeed;
		}

		if (InputManager::GetInstance()->IsKeyPress('A'))
		{
			GetTransform()->AddPosition(Vector3(-dt * curSpeed, 0, 0));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
			if (m_rotationLockTime <= 0.f) { m_targetVector += Vector3(-1.f, 0, 0); }
		}
		if (InputManager::GetInstance()->IsKeyPress('D'))
		{
			GetTransform()->AddPosition(Vector3(dt * curSpeed, 0, 0));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
			if (m_rotationLockTime <= 0.f) m_targetVector += Vector3(1.f, 0, 0);
		}
		if (InputManager::GetInstance()->IsKeyPress('W'))
		{
			GetTransform()->AddPosition(Vector3(0, 0, dt * curSpeed));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
			if (m_rotationLockTime <= 0.f) m_targetVector += Vector3(0, 0, 1.f);
		}
		if (InputManager::GetInstance()->IsKeyPress('S'))
		{
			GetTransform()->AddPosition(Vector3(0, 0, -dt * curSpeed));
			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
			if (m_rotationLockTime <= 0.f) m_targetVector += Vector3(0, 0, -1.f);
		}

		if (m_rotationLockTime <= 0.0f)
		{
			if (m_targetVector.LengthSquared() > 1.f) // <-이 조건 넣으면 키 누를때에만 방향전환함.. 이게 별로면 빼세요
			{
				m_targetVector.Normalize();

				// 부드럽게 방향 전환하기
				//m_headVector = DirectX::SimpleMath::Vector3::Lerp(m_headVector, m_targetVector, dt * m_turnSpeed); // lerp가 아니라 slerp를 써야함
				DirectX::SimpleMath::Quaternion targetQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_targetVector.x, m_targetVector.z), 0.f, 0.f);
				DirectX::SimpleMath::Quaternion headQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_headVector.x, m_headVector.z), 0.f, 0.f);
				DirectX::SimpleMath::Quaternion resultQuat = DirectX::SimpleMath::Quaternion::Slerp(headQuat, targetQuat, dt * m_turnSpeed);
				m_headVector = DirectX::SimpleMath::Vector3::Transform(Vector3::UnitZ, resultQuat);
				GetTransform()->SetLocalRotation(Vector3(0.f, DirectX::XMConvertToDegrees(atan2f(m_headVector.x, m_headVector.z)), 0.f));
			}
		}
		else
		{
			m_targetVector.Normalize();

			// 부드럽게 방향 전환하기
			//m_headVector = DirectX::SimpleMath::Vector3::Lerp(m_headVector, m_targetVector, dt * m_turnSpeed); // lerp가 아니라 slerp를 써야함
			DirectX::SimpleMath::Quaternion targetQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_targetVector.x, m_targetVector.z), 0.f, 0.f);
			DirectX::SimpleMath::Quaternion headQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_headVector.x, m_headVector.z), 0.f, 0.f);
			DirectX::SimpleMath::Quaternion resultQuat = DirectX::SimpleMath::Quaternion::Slerp(headQuat, targetQuat, dt * m_turnSpeed);
			m_headVector = DirectX::SimpleMath::Vector3::Transform(Vector3::UnitZ, resultQuat);
			GetTransform()->SetLocalRotation(Vector3(0.f, DirectX::XMConvertToDegrees(atan2f(m_headVector.x, m_headVector.z)), 0.f));
		}

		// 던지기
		if (!m_techItemList.empty())
		{
			for (auto item : m_techItemList)
			{
				GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Throw);

				//item->GetComponent<SphereCollider>()->SetRadius(m_itemRange);
				item->GetComponent<SphereCollider>()->AddForce(m_bulletforcevector, ForceMode::Impulse);
				item->AddComponent<MeshRenderer>();
				item->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Potion);
				m_techItemList.pop_back();
			}
		}


	}

	if (!m_ableSkill)
	{
		static double checkSkill = 0;
		checkSkill += dt;
		if (checkSkill >= m_skillCoolTime)
		{
			m_ableSkill = (m_skillCount > 0);

			checkSkill = 0;
		}
	}

	// UI로 허기게이지와 점수를 전달한다. 
	m_scoreUI->TransferInfo(m_hunger, m_score);

	if (m_rotationLockTime > 0)
	{
		m_rotationLockTime -= dt;
	}
}

std::shared_ptr<Component> PlayerMoveScript::Clone() const
{
	std::shared_ptr<PlayerMoveScript> clone = std::make_shared<PlayerMoveScript>();

	clone->m_targetVector = m_targetVector;
	clone->m_headVector = m_headVector;
	clone->m_moveSpeed = m_moveSpeed;
	clone->m_turnSpeed = m_turnSpeed;
	clone->m_hunger = m_hunger;

	return clone;
}

void PlayerMoveScript::OnCollisionEnter(const Collision& collision)
{
	if (collision.otherCollider)
	{
		if (collision.otherCollider->GetOwner().lock()->GetComponent<GuardFSM>() != nullptr)
		{
			if (GetOwner().lock()->GetComponent<PlayerFSM>()->GetState() != PlayerFSM::Player::Hide)
			{
				//플레이어 체포 상태로 만들것
				GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Arrest);
			}
		}
	}
}

void PlayerMoveScript::OnCollisionStay(const Collision& collision)
{

}

void PlayerMoveScript::OnCollisionExit(const Collision& collision)
{

}

void PlayerMoveScript::OnTriggerEnter(Collider* collider)
{
	if (collider)
	{
		if (collider->GetOwner().lock()->HasComponent<FoodTableScript>())
		{
			m_foodTableScript = collider->GetOwner().lock()->GetComponent<FoodTableScript>();
		}
	}
}

void PlayerMoveScript::OnTriggerStay(Collider* collider)
{
}

void PlayerMoveScript::OnTriggerExit(Collider* collider)
{
	if (collider)
	{
		if (collider->GetOwner().lock()->HasComponent<FoodTableScript>())
		{
			if (m_foodTableScript)
				m_foodTableScript.reset();
		}
	}
}

void PlayerMoveScript::ThrowItem(float power)
{
	//if (m_skillCount > 0)
	{
		// 플레이어가 미니게임을 플레이중이지 않을때에만 가능 && 시뮬레이션 off 일때 Addforce하면 터지기에 예외처리
		if (!m_isPlayMiniGame && m_owner.lock()->GetSceneManager().lock()->GetCurScene()->IsSimulating())
		{
			if (GetOwner().lock()->GetComponent<PlayerFSM>()->GetState() == PlayerFSM::Player::Lifting)
			{
				if (InputManager::GetInstance()->IsKeyUp(VK_LBUTTON))
				{
					/// 조준게이지 끝, 발사

					Vector3 bulletposition = GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition();
					bulletposition.y += 3.f; // 겹치면 콜라이더끼리 튕기니까 조금 위에서 생성

					auto item = std::make_shared<Entity>("");
					item->SetName("item");
					item->GetComponent<Transform>()->SetLocalPosition(bulletposition);
					EventManager::GetInstance()->AddEvent(item, EventType::Create);
					item->AddComponent<SphereCollider>();					
					item->AddComponent<ThrowItemScript>();

					float m_gaugePower = power;

					///마우스 위치에 맞춰서 던지기
					// 플레이어의 화면 위치를 계산해보자
					Vector3 playerPos = GetTransform()->GetWorldPosition();
					Matrix viewProj = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetViewProjectionMatrix();
					Vector2 screenSize = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetScreenSize();
					Vector2 playerScreenPos = MathUtils::WorldToScreenPixel(playerPos, viewProj, screenSize.x, screenSize.y);

					// 마우스 위치를 가져온다.
					Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();

					// 두 벡터의 방향 벡터를 구한다.
					Vector2 direction = mousePos - playerScreenPos;
					direction.Normalize();
					Vector3 dir3 = Vector3(direction.x, 0, -direction.y);
					m_rotationLockTime = 1.f;
					m_targetVector = dir3;
					// 	m_bulletforcevector = m_headVector * 50.f * m_gaugePower;
					// 	m_bulletforcevector.y += 50.f;

					m_bulletforcevector = dir3 * 50.f * m_gaugePower;
					m_bulletforcevector.y += 20.f;

					m_techItemList.emplace_back(item);
					//item->GetComponent<BoxCollider>()->AddForce(bulletforcevector, ForceMode::Impulse);

					m_skillCount--;
					m_ableSkill = false;
				}
			}
		}
	}
}

void PlayerMoveScript::BasicScoreIncrease()
{
	m_score += m_basicScoreIncrease;
	m_scoreUI->ScoreIncrease(m_basicScoreIncrease);
	m_scoreUI->TransferInfo(m_hunger, m_score);
}

void PlayerMoveScript::DeliciousScoreIncrease()
{
	m_score += m_deliciousScoreIncrease;
	m_scoreUI->ScoreIncrease(m_deliciousScoreIncrease);
	m_scoreUI->TransferInfo(m_hunger, m_score);
}
