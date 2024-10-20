#include "GameEngine_pch.h"
#include "RayCaster.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "EntityDefine.h"
#include "Entity.h"

//디버깅
#include "IGraphics.h"
#include "Transform.h"
#include "CameraModule.h"
#include "CameraManager.h"

void RayCaster::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_startPosOffset"] =
	{
		{"x", m_startPosOffset.x},
		{"y", m_startPosOffset.y},
		{"z", m_startPosOffset.z}
	};

	json["m_unitDirOffset"] =
	{
		{"x", m_unitDirOffset.x},
		{"y", m_unitDirOffset.y},
		{"z", m_unitDirOffset.z}
	};
}

void RayCaster::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("m_startPosOffset"))
		{
			m_startPosOffset.x = json["m_startPosOffset"]["x"];
			m_startPosOffset.y = json["m_startPosOffset"]["y"];
			m_startPosOffset.z = json["m_startPosOffset"]["z"];
		}

		if (json.contains("m_unitDirOffset"))
		{
			m_unitDirOffset.x = json["m_unitDirOffset"]["x"];
			m_unitDirOffset.y = json["m_unitDirOffset"]["y"];
			m_unitDirOffset.z = json["m_unitDirOffset"]["z"];
		}
	}
}

RayCaster::RayCaster()
	: m_startPosOffset(Vector3(0,0,0)), m_unitDirOffset(Vector3(0,1,0))
{

}

RayCaster::~RayCaster()
{

}

std::shared_ptr<Component> RayCaster::Clone() const
{
	std::shared_ptr<RayCaster> clone = std::make_shared<RayCaster>();
	clone->m_distance = this->m_distance;
	clone->m_findMultiple = this->m_findMultiple;
	clone->m_startPosOffset = this->m_startPosOffset;
	clone->m_unitDirOffset = this->m_unitDirOffset;

	return clone;
}

void RayCaster::Update(double dt)
{
	m_unitDirOffset.Normalize();

#ifdef _DEBUG

// 부모 회전값으로 노말값도 회전시키기 
	Quaternion parentQuat = m_owner.lock()->GetComponent<Transform>()->GetWorldRotation();
	Vector3 parentRot = QuaternionUtils::ToEulerAngles(parentQuat);
	
	// 회전 행렬 생성
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(parentRot.x, parentRot.y, parentRot.z);

	// 회전 행렬을 사용하여 노말 벡터 회전
	m_rotatedDir = DirectX::XMVector3TransformNormal(m_unitDirOffset, rotation_matrix);

	// 회전을 반영한 포지션 계산
	Vector3 pos = m_owner.lock()->GetComponent<Transform>()->GetWorldPosition();
	m_translatedPos =  QuaternionUtils::RotateByPivot(m_startPosOffset, pos, parentRot);

	if(m_findMultiple)
		CollisionManager::GetInstance()->RaycastMultiple(m_translatedPos, m_rotatedDir, m_distance);
	else
		CollisionManager::GetInstance()->RaycastClosest(m_translatedPos, m_rotatedDir, m_distance);	

#endif
}

void RayCaster::Render()
{

}

void RayCaster::DebugRender()
{
	auto graphics = m_owner.lock()->GetGraphics().lock();
	auto camera = m_owner.lock()->GetCameraManager().lock()->GetMainCamera();

	if (!m_isModelCreate)
	{
		m_modelKey = graphics->LoadModel(m_cloneModelKey);
		m_isModelCreate = true;
	}

// 	Vector3 pos = m_owner.lock()->GetComponent<Transform>()->GetWorldPosition();
// 	//랜더되는 노말값 : 1,0,0 -> 우리의 노말값 변환은 어떻게? 
// 
// 	Vector3 normalFrom(1, 0, 0);
// 
// 	Vector3 dir;
// 
// 	//두 벡터의 회전축 -> 두 벡터가 이루는 평면의 법선 벡터
// 	DirectX::XMVECTOR axisOfRotation = DirectX::XMVector3Cross(normalFrom, m_unitDirOffset);
// 
// 	// 내적 -> 두 벡터 간의 각도
// 	float angle = std::acos(DirectX::XMVectorGetX(DirectX::XMVector3Dot(normalFrom, m_unitDirOffset)));
// 	
// 	// 축과 각도를 이용해 회전 행렬 생성
// 	auto rotmat = DirectX::XMMatrixRotationAxis(axisOfRotation, angle);
// 
// 
// 	auto transform = GetTransform();
// 	Vector3 tsl = pos + m_startPosOffset;
// 	Matrix world = Matrix::CreateScale(m_distance)
// 		* Matrix(rotmat)
// 		* Matrix::CreateTranslation(tsl);

	Vector3 pos = m_translatedPos;

	Vector3 normalFrom(1, 0, 0);

	Vector3 dir;

	//두 벡터의 회전축 -> 두 벡터가 이루는 평면의 법선 벡터
	DirectX::XMVECTOR axisOfRotation = DirectX::XMVector3Cross(normalFrom, m_rotatedDir);

	// 내적 -> 두 벡터 간의 각도
	float angle = std::acos(DirectX::XMVectorGetX(DirectX::XMVector3Dot(normalFrom, m_rotatedDir)));

	// 축과 각도를 이용해 회전 행렬 생성
	auto rotmat = DirectX::XMMatrixRotationAxis(axisOfRotation, angle);

	Vector3 tsl = pos;
	Matrix world = Matrix::CreateScale(m_distance)
		* Matrix(rotmat)
		* Matrix::CreateTranslation(tsl);


	// 버텍스 쉐이더
	GraphicsTransfer::CbTransform cbTransform;
	cbTransform.world = world;
	cbTransform.viewProj = camera->GetViewProjectionMatrix();

	// 픽셀 쉐이더는 나중에

	GraphicsTransfer::ModelData data;
	data.modelKey = m_modelKey;
	data.transform = cbTransform;

	graphics->ObjectRender(data);
}

void RayCaster::SetUnitDirectionOffset(Vector3 dir)
{
	dir.Normalize();
	m_unitDirOffset = dir;
}

void RayCaster::SetProperty(Vector3 startPos, Vector3 unitDirection, float distance)
{
	m_startPosOffset = startPos; 
	unitDirection.Normalize();
	m_unitDirOffset = unitDirection; 
	m_distance = distance;
}

RayCastInfo RayCaster::RaycastClosest()
{
	Quaternion parentQuat = m_owner.lock()->GetComponent<Transform>()->GetWorldRotation();
	Vector3 parentRot = QuaternionUtils::ToEulerAngles(parentQuat);

	// 회전 행렬 생성
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(parentRot.x, parentRot.y, parentRot.z);

	// 회전 행렬을 사용하여 노말 벡터 회전
	m_rotatedDir = DirectX::XMVector3TransformNormal(m_unitDirOffset, rotation_matrix);

	// 회전을 반영한 포지션 계산
	Vector3 pos = m_owner.lock()->GetComponent<Transform>()->GetWorldPosition();
	m_translatedPos = QuaternionUtils::RotateByPivot(m_startPosOffset, pos, parentRot);

	return CollisionManager::GetInstance()->RaycastClosest(m_translatedPos, m_rotatedDir, m_distance);
}

std::vector<RayCastInfo> RayCaster::RaycastMultiple()
{
	Quaternion parentQuat = m_owner.lock()->GetComponent<Transform>()->GetWorldRotation();
	Vector3 parentRot = QuaternionUtils::ToEulerAngles(parentQuat);

	// 회전 행렬 생성
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(parentRot.x, parentRot.y, parentRot.z);

	// 회전 행렬을 사용하여 노말 벡터 회전
	m_rotatedDir = DirectX::XMVector3TransformNormal(m_unitDirOffset, rotation_matrix);

	// 회전을 반영한 포지션 계산
	Vector3 pos = m_owner.lock()->GetComponent<Transform>()->GetWorldPosition();
	m_translatedPos = QuaternionUtils::RotateByPivot(m_startPosOffset, pos, parentRot);

	return CollisionManager::GetInstance()->RaycastMultiple(m_translatedPos, m_rotatedDir, m_distance);
}
