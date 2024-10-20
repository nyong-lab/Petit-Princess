#include "GameDemo_pch.h"
#include "DongilScript.h"

#include "../GameEngine/InputManager.h"
#include "Transform.h"
#include "Entity.h"

void DongilScript::Serialize(nlohmann::json& json)
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

void DongilScript::Deserialize(const nlohmann::json& json)
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

void DongilScript::Update(double dt)
{
	if (InputManager::GetInstance()->IsKeyPress(VK_UP))
	{
		GetTransform()->AddPosition(Vector3(0, 0, dt * m_moveSpeed));
		m_targetVector += Vector3(0, 0, 1.f);
	}
	if (InputManager::GetInstance()->IsKeyPress(VK_LEFT))
	{
		GetTransform()->AddPosition(Vector3(-dt * m_moveSpeed, 0, 0));
		m_targetVector += Vector3(-1.f, 0, 0);
	}
	if (InputManager::GetInstance()->IsKeyPress(VK_DOWN))
	{
		GetTransform()->AddPosition(Vector3(0, 0, -dt * m_moveSpeed));
		m_targetVector += Vector3(0, 0, -1.f);
	}
	if (InputManager::GetInstance()->IsKeyPress(VK_RIGHT))
	{
		GetTransform()->AddPosition(Vector3(dt * m_moveSpeed, 0, 0));
		m_targetVector += Vector3(1.f, 0, 0);
	}
	
	m_targetVector.Normalize();

	// 부드럽게 방향 전환하기
	Quaternion targetQuat = Quaternion::CreateFromYawPitchRoll(atan2f(m_targetVector.x, m_targetVector.z), 0.f, 0.f);
	Quaternion headQuat = Quaternion::CreateFromYawPitchRoll(atan2f(m_headVector.x, m_headVector.z), 0.f, 0.f);
	Quaternion resultQuat = Quaternion::Slerp(headQuat, targetQuat, dt * m_turnSpeed);
	m_headVector = DirectX::SimpleMath::Vector3::Transform(Vector3::UnitZ, resultQuat);
	GetTransform()->SetLocalRotation(Vector3(0.f, DirectX::XMConvertToDegrees(atan2f(m_headVector.x, m_headVector.z)), 0.f));

	if (InputManager::GetInstance()->GetWheelState() == WheelState::Up)
	{
		static int a = 0;
		++a;
	}
	if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
	{
		static int a = 0;
		++a;
	}
}

void DongilScript::Render(double dt)
{

}

std::shared_ptr<Component> DongilScript::Clone() const
{
    std::shared_ptr<DongilScript> clone = std::make_shared<DongilScript>();

	clone->m_moveSpeed = m_moveSpeed;
	clone->m_turnSpeed = m_turnSpeed;
	clone->m_targetVector = m_targetVector;
	clone->m_headVector = m_headVector;

    return clone;
}
