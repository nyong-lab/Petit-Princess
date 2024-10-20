#include "GameDemo_pch.h"
#include "PetitCamera.h"
#include "Entity.h"
#include "CameraModule.h"
#include "CameraManager.h"
#include "MathUtils.h"

void PetitCamera::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_posOffset"] =
	{
		{"x", m_posOffset.x},
		{"y", m_posOffset.y},
		{"z", m_posOffset.z}
	};
}

void PetitCamera::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
		from_json(json, *this);

	if (json.contains("m_posOffset"))
	{
		m_posOffset.x = json["m_posOffset"]["x"];
		m_posOffset.y = json["m_posOffset"]["y"];
		m_posOffset.z = json["m_posOffset"]["z"];
	}
}

PetitCamera::PetitCamera()
	:m_mainCamera(nullptr), m_target(nullptr),
	m_topBoundary(120), m_bottomBoundary(10), m_rightBoundary(135), m_leftBoundary(10)
	, m_posOffset(Vector3(0,20,-17.5))
	, m_yaw(0), m_pitch(-0.85), m_cameraChaseSpeed(2.5f)
	, m_chasingStartDistance(5), m_chasingStopDistance(0.2f), m_isChasing(true)
{

}

PetitCamera::~PetitCamera()
{

}

void PetitCamera::Update(double dt)
{
	if (m_target)
	{
		// �þ� ���� �׻� ������Ų��.
		m_mainCamera = m_owner.lock()->GetCameraManager().lock()->GetMainCamera();
		m_mainCamera->SetLookRadian(Vector2(m_yaw, m_pitch));
		
		// ��ǥ ��ƼƼ�� ��ġ�� �����´�.
		Vector3 entityPos = m_target->GetComponent<Transform>()->GetWorldPosition();
		
		
		// �������� ����� ��ǥ ī�޶� ��ġ�� ����Ѵ�.
		Vector3 targetPos = entityPos + m_posOffset;
		float height = m_posOffset.y + entityPos.y;

		// �ٿ������ �Ѿ�� ���̻� ������ �ʴ´�.
		targetPos = MathUtils::Clamp(targetPos
			, Vector3(m_leftBoundary, height, m_bottomBoundary)
			, Vector3(m_rightBoundary, height, m_topBoundary));

		Vector3 curCameraPos = m_mainCamera->GetPosition();
				
		// ���� ī�޶� ��ġ�� ��ǥ ��ġ�� �Ÿ��� ���Ѵ�.
		float distance;
		DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(DirectX::XMVectorSubtract(targetPos, curCameraPos)));
		
		// �Ÿ��� �ּ� �Ÿ����� �ָ� ������ �����Ѵ�.
		if (m_chasingStartDistance < distance)
			m_isChasing = true;

		if (m_isChasing)
		{
			// ���� ������ �Ÿ��� �����ϸ� �������·� ����
			if (distance < m_chasingStopDistance)
				m_isChasing = false;

			Vector3 now = MathUtils::Lerp(curCameraPos, targetPos, dt * m_cameraChaseSpeed);
			m_mainCamera->SetPosition(now);
		}
	}
}

void PetitCamera::Render(double dt)
{

}

void PetitCamera::Finalize()
{

}

std::shared_ptr<Component> PetitCamera::Clone() const
{
	auto clone = std::make_shared<PetitCamera>();
	clone->m_topBoundary = m_topBoundary;
	clone->m_bottomBoundary = m_bottomBoundary;
	clone->m_rightBoundary = m_rightBoundary;
	clone->m_leftBoundary = m_leftBoundary;
	clone->m_posOffset = m_posOffset;
	clone->m_yaw = m_yaw;
	clone->m_pitch = m_pitch;
	clone->m_cameraChaseSpeed = m_cameraChaseSpeed;

	return clone;
}
