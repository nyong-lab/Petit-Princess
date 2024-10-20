#include "GameEngine_pch.h"
#include "RayCaster.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "EntityDefine.h"
#include "Entity.h"

//�����
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

// �θ� ȸ�������� �븻���� ȸ����Ű�� 
	Quaternion parentQuat = m_owner.lock()->GetComponent<Transform>()->GetWorldRotation();
	Vector3 parentRot = QuaternionUtils::ToEulerAngles(parentQuat);
	
	// ȸ�� ��� ����
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(parentRot.x, parentRot.y, parentRot.z);

	// ȸ�� ����� ����Ͽ� �븻 ���� ȸ��
	m_rotatedDir = DirectX::XMVector3TransformNormal(m_unitDirOffset, rotation_matrix);

	// ȸ���� �ݿ��� ������ ���
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
// 	//�����Ǵ� �븻�� : 1,0,0 -> �츮�� �븻�� ��ȯ�� ���? 
// 
// 	Vector3 normalFrom(1, 0, 0);
// 
// 	Vector3 dir;
// 
// 	//�� ������ ȸ���� -> �� ���Ͱ� �̷�� ����� ���� ����
// 	DirectX::XMVECTOR axisOfRotation = DirectX::XMVector3Cross(normalFrom, m_unitDirOffset);
// 
// 	// ���� -> �� ���� ���� ����
// 	float angle = std::acos(DirectX::XMVectorGetX(DirectX::XMVector3Dot(normalFrom, m_unitDirOffset)));
// 	
// 	// ��� ������ �̿��� ȸ�� ��� ����
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

	//�� ������ ȸ���� -> �� ���Ͱ� �̷�� ����� ���� ����
	DirectX::XMVECTOR axisOfRotation = DirectX::XMVector3Cross(normalFrom, m_rotatedDir);

	// ���� -> �� ���� ���� ����
	float angle = std::acos(DirectX::XMVectorGetX(DirectX::XMVector3Dot(normalFrom, m_rotatedDir)));

	// ��� ������ �̿��� ȸ�� ��� ����
	auto rotmat = DirectX::XMMatrixRotationAxis(axisOfRotation, angle);

	Vector3 tsl = pos;
	Matrix world = Matrix::CreateScale(m_distance)
		* Matrix(rotmat)
		* Matrix::CreateTranslation(tsl);


	// ���ؽ� ���̴�
	GraphicsTransfer::CbTransform cbTransform;
	cbTransform.world = world;
	cbTransform.viewProj = camera->GetViewProjectionMatrix();

	// �ȼ� ���̴��� ���߿�

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

	// ȸ�� ��� ����
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(parentRot.x, parentRot.y, parentRot.z);

	// ȸ�� ����� ����Ͽ� �븻 ���� ȸ��
	m_rotatedDir = DirectX::XMVector3TransformNormal(m_unitDirOffset, rotation_matrix);

	// ȸ���� �ݿ��� ������ ���
	Vector3 pos = m_owner.lock()->GetComponent<Transform>()->GetWorldPosition();
	m_translatedPos = QuaternionUtils::RotateByPivot(m_startPosOffset, pos, parentRot);

	return CollisionManager::GetInstance()->RaycastClosest(m_translatedPos, m_rotatedDir, m_distance);
}

std::vector<RayCastInfo> RayCaster::RaycastMultiple()
{
	Quaternion parentQuat = m_owner.lock()->GetComponent<Transform>()->GetWorldRotation();
	Vector3 parentRot = QuaternionUtils::ToEulerAngles(parentQuat);

	// ȸ�� ��� ����
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(parentRot.x, parentRot.y, parentRot.z);

	// ȸ�� ����� ����Ͽ� �븻 ���� ȸ��
	m_rotatedDir = DirectX::XMVector3TransformNormal(m_unitDirOffset, rotation_matrix);

	// ȸ���� �ݿ��� ������ ���
	Vector3 pos = m_owner.lock()->GetComponent<Transform>()->GetWorldPosition();
	m_translatedPos = QuaternionUtils::RotateByPivot(m_startPosOffset, pos, parentRot);

	return CollisionManager::GetInstance()->RaycastMultiple(m_translatedPos, m_rotatedDir, m_distance);
}
