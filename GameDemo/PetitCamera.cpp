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
		// 시야 각은 항상 고정시킨다.
		m_mainCamera = m_owner.lock()->GetCameraManager().lock()->GetMainCamera();
		m_mainCamera->SetLookRadian(Vector2(m_yaw, m_pitch));
		
		// 목표 엔티티의 위치를 가져온다.
		Vector3 entityPos = m_target->GetComponent<Transform>()->GetWorldPosition();
		
		
		// 오프셋을 계산한 목표 카메라 위치를 계산한다.
		Vector3 targetPos = entityPos + m_posOffset;
		float height = m_posOffset.y + entityPos.y;

		// 바운더리를 넘어가면 더이상 따라가지 않는다.
		targetPos = MathUtils::Clamp(targetPos
			, Vector3(m_leftBoundary, height, m_bottomBoundary)
			, Vector3(m_rightBoundary, height, m_topBoundary));

		Vector3 curCameraPos = m_mainCamera->GetPosition();
				
		// 현재 카메라 위치와 목표 위치의 거리를 구한다.
		float distance;
		DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(DirectX::XMVectorSubtract(targetPos, curCameraPos)));
		
		// 거리가 최소 거리보다 멀면 추적을 시작한다.
		if (m_chasingStartDistance < distance)
			m_isChasing = true;

		if (m_isChasing)
		{
			// 일정 이하의 거리에 도달하면 정지상태로 변경
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
