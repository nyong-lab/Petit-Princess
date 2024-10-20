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
	if (child == nullptr) assert(false && L"�÷��̾��� �ڽ����� ScoreUI�� �ʿ��մϴ�.");
	m_scoreUI = m_owner.lock()->GetSmartChild("ScoreUI").lock()->GetComponent<ScoreUI>();
	if (m_scoreUI == nullptr) assert(false && L"�÷��̾��� �ڽ����� ScoreUI�� �ʿ��մϴ�.");

	// �ʱ� �����̼��� �ʱ� m_headVector�� �Ȱ��� ��������� �Ѵ�
	GetTransform()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
}

void PlayerMoveScript::Update(double dt)
{
	/// �׽�Ʈ-------------------------------------------------------
// 	if (InputManager::GetInstance()->IsKeyDown('0'))
// 	{
// 		BasicScoreIncrease();
// 	}
// 
// 	if (InputManager::GetInstance()->IsKeyDown('9'))
// 	{
// 		DeliciousScoreIncrease();
// 	}

	/// �׽�Ʈ-------------------------------------------------------
	

	// ��� ������ ����
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

	//���� �ӵ�
	float curSpeed = 0;


	// ���� �̼��� �ð�
	static float notEatingElapsedTime = 0.f;

	// ��� ���� �ð�
	static float hungerDecreaseElapsedTime = 0.f;

	notEatingElapsedTime += dt;

	// n�ʵ��� ���� �̼��� ��
	if (notEatingElapsedTime > m_notEatenTime)
	{
		hungerDecreaseElapsedTime += dt;

		// n�ʸ��� �������� ����
		if (hungerDecreaseElapsedTime > m_hugerDecreaseTime)
		{
			hungerDecreaseElapsedTime = 0.f;
			m_hunger -= m_decreaseHunger;

			if (m_hunger < 0.f)
				m_hunger = 0.f;
		}
	}

	// �������� ��ġ�� �´� �÷��̾� �̵� �ӵ� ����
	// ���� ����
	if (m_hunger >= m_hungerSatisfactionState)
		m_moveSpeed = m_satisfactionMoveSpeed;

	// ���� ����
	else if (m_hunger >= m_hungerNormalState)
		m_moveSpeed = m_normalMoveSpeed;

	// �Ҹ��� ����
	else
		m_moveSpeed = m_dissatisfactionMoveSpeed;

	// ������ ���� �� n�ʰ� ����
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

	// �̴ϰ����� �����ߴٸ� n�ʰ� ���� �Ա� (�� �ð� ���� �ൿ����)
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

			// ��湮 ��Ÿ�� ����
			if (m_foodTableScript)
				m_foodTableScript->SetAteFood(true);

			GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);
		}
	}
	// ��� ������Ʈ�� ��ȣ�ۿ��Ͽ��ٸ� n�� �ൿ ����
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
		// ������ ���� �ߴٸ� n�ʰ� �̵��ӵ� 1.5�� ����
		if (m_isFoodIntake)
		{
			curSpeed = m_moveSpeed * 1.5f;
		}
		// ���� ���� �ʾҴٸ� �⺻ �ӵ�
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
			if (m_targetVector.LengthSquared() > 1.f) // <-�� ���� ������ Ű ���������� ������ȯ��.. �̰� ���θ� ������
			{
				m_targetVector.Normalize();

				// �ε巴�� ���� ��ȯ�ϱ�
				//m_headVector = DirectX::SimpleMath::Vector3::Lerp(m_headVector, m_targetVector, dt * m_turnSpeed); // lerp�� �ƴ϶� slerp�� �����
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

			// �ε巴�� ���� ��ȯ�ϱ�
			//m_headVector = DirectX::SimpleMath::Vector3::Lerp(m_headVector, m_targetVector, dt * m_turnSpeed); // lerp�� �ƴ϶� slerp�� �����
			DirectX::SimpleMath::Quaternion targetQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_targetVector.x, m_targetVector.z), 0.f, 0.f);
			DirectX::SimpleMath::Quaternion headQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(atan2f(m_headVector.x, m_headVector.z), 0.f, 0.f);
			DirectX::SimpleMath::Quaternion resultQuat = DirectX::SimpleMath::Quaternion::Slerp(headQuat, targetQuat, dt * m_turnSpeed);
			m_headVector = DirectX::SimpleMath::Vector3::Transform(Vector3::UnitZ, resultQuat);
			GetTransform()->SetLocalRotation(Vector3(0.f, DirectX::XMConvertToDegrees(atan2f(m_headVector.x, m_headVector.z)), 0.f));
		}

		// ������
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

	// UI�� ���������� ������ �����Ѵ�. 
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
				//�÷��̾� ü�� ���·� �����
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
		// �÷��̾ �̴ϰ����� �÷��������� ���������� ���� && �ùķ��̼� off �϶� Addforce�ϸ� �����⿡ ����ó��
		if (!m_isPlayMiniGame && m_owner.lock()->GetSceneManager().lock()->GetCurScene()->IsSimulating())
		{
			if (GetOwner().lock()->GetComponent<PlayerFSM>()->GetState() == PlayerFSM::Player::Lifting)
			{
				if (InputManager::GetInstance()->IsKeyUp(VK_LBUTTON))
				{
					/// ���ذ����� ��, �߻�

					Vector3 bulletposition = GetOwner().lock()->GetComponent<Transform>()->GetLocalPosition();
					bulletposition.y += 3.f; // ��ġ�� �ݶ��̴����� ƨ��ϱ� ���� ������ ����

					auto item = std::make_shared<Entity>("");
					item->SetName("item");
					item->GetComponent<Transform>()->SetLocalPosition(bulletposition);
					EventManager::GetInstance()->AddEvent(item, EventType::Create);
					item->AddComponent<SphereCollider>();					
					item->AddComponent<ThrowItemScript>();

					float m_gaugePower = power;

					///���콺 ��ġ�� ���缭 ������
					// �÷��̾��� ȭ�� ��ġ�� ����غ���
					Vector3 playerPos = GetTransform()->GetWorldPosition();
					Matrix viewProj = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetViewProjectionMatrix();
					Vector2 screenSize = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetScreenSize();
					Vector2 playerScreenPos = MathUtils::WorldToScreenPixel(playerPos, viewProj, screenSize.x, screenSize.y);

					// ���콺 ��ġ�� �����´�.
					Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();

					// �� ������ ���� ���͸� ���Ѵ�.
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
