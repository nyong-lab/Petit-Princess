#include "GameEngine_pch.h"
#include "SphereCollider.h"
#include "Entity.h"
#include "Transform.h"
#include "CameraModule.h"
#include "CollisionManager.h"
#include "IGraphics.h"
#include "Scene.h"
#include "CameraManager.h"

void SphereCollider::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_offset"] =
	{
		{"x", m_offset.x},
		{"y", m_offset.y},
		{"z", m_offset.z}
	};
}

void SphereCollider::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("m_offset"))
		{
			m_offset.x = json["m_offset"]["x"];
			m_offset.y = json["m_offset"]["y"];
			m_offset.z = json["m_offset"]["z"];
		}
	}
}

/// ------------------------------------------------------------------------------------------------
///								          생성자와 소멸자
/// -------------------------------------------------------------------------------------------------

SphereCollider::SphereCollider()
{
	Matrix mat = Matrix::Identity;
	m_rigidDynamic = m_physics->createRigidDynamic(MatToPxTransform(mat));

	m_shape = m_physics->createShape(
		PxSphereGeometry(static_cast<physx::PxReal>(m_radius)), &m_mateiral, 1, true);
	m_rigidDynamic->attachShape(*m_shape);
	m_rigidDynamic->userData = new ColliderInfo(this, m_colliderID);
}

SphereCollider::~SphereCollider()
{

}

/// --------------------------------------------------------------------------------------------------
///								         컴포넌트 함수들
/// --------------------------------------------------------------------------------------------------

void SphereCollider::Start()
{
	if (m_isInitialize) 
		return;

	Matrix mat = m_owner.lock()->GetComponent<Transform>()->GetWorldMatrix();
	switch (m_type)
	{
	case BodyType::Static:
		m_rigidStatic->setGlobalPose(MatToPxTransform(mat));
		if (m_scene)
			if (m_scene->IsSimulating()) 
				m_collisionManager->AddActorToScene(m_rigidStatic);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setGlobalPose(MatToPxTransform(mat));
		m_rigidDynamic->setLinearDamping(m_linearDamping);
		m_rigidDynamic->setAngularDamping(m_angularDamping);
		PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, m_mass);
		if(m_scene)
			if (m_scene->IsSimulating())
				m_collisionManager->AddActorToScene(m_rigidDynamic);
		break;
	}

	SetLayerCollision(m_owner.lock()->GetLayerCollision());
	m_isInitialize = true;
}

void SphereCollider::Update(double dt)
{
	Collider::Update(dt);
}

void SphereCollider::DebugRender()
{
	auto graphics = m_owner.lock()->GetGraphics().lock();
	auto camera = m_owner.lock()->GetCameraManager().lock()->GetMainCamera();

	if (!m_isModelCreate)
	{
		m_cloneModelKey = ModelKey::WireFrameSphere;
		m_modelKey = graphics->LoadModel(m_cloneModelKey);
		m_isModelCreate = true;
	}

	auto transform = GetTransform();
	Vector3 tsl = transform->GetWorldPosition() + m_offset;
	Quaternion quat = transform->GetWorldRotation();
	Matrix world = Matrix::CreateScale(m_radius, m_radius, m_radius)
		* Matrix::CreateFromQuaternion(quat)
		* Matrix::CreateTranslation(tsl);

	// 상수버퍼 업데이트

	GraphicsTransfer::CbTransform cbTransform;
	cbTransform.world = world;
	cbTransform.viewProj = camera->GetViewProjectionMatrix();

	GraphicsTransfer::CbPhysics cbPhysics;
	if (m_contactOthers.size() > 0 || m_triggerOthers.size() > 0)
		cbPhysics.isCollision = true;
	else cbPhysics.isCollision = false;

	GraphicsTransfer::ModelData data;
	data.modelKey = m_modelKey;
	data.transform = cbTransform;
	data.physics = cbPhysics;

	graphics->ObjectRender(data);
}


void SphereCollider::Finalize()
{
	Collider::Finalize();
}

std::shared_ptr<Component> SphereCollider::Clone() const
{
	// 멤버 변수 복사
	std::shared_ptr<SphereCollider> newSph = std::make_shared<SphereCollider>();
	Collider::CopyMembersThisToOther(newSph);
	newSph->m_radius = this->m_radius;
	newSph->m_collisionManager = this->m_collisionManager;
	newSph->m_offset = this->m_offset;

	newSph->InitialializeClone(m_owner.lock()->GetComponent<Transform>()->GetWorldMatrix(), m_owner.lock()->GetLayerCollision());
	return newSph;
}

/// --------------------------------------------------------------------------------------------------
///								         데이터 연동 & 업데이트
/// --------------------------------------------------------------------------------------------------

void SphereCollider::UpdatePhysXProperties()
{
	if (m_radius <= 0) m_radius = 0.001f;
	
	//사이즈 변경
	switch (m_type)
	{
	case BodyType::Static:
		if (m_rigidStatic) m_rigidStatic->detachShape(*m_shape);
		break;

	case BodyType::Dynamic:
		if (m_rigidDynamic) m_rigidDynamic->detachShape(*m_shape);
		break;
	}

	if (m_shape) m_shape->release();
	m_shape = m_physics->createShape(
		PxSphereGeometry(static_cast<physx::PxReal>(m_radius)), &m_mateiral, 1, true);

	PxTransform relativePose = PxTransform(PxVec3(m_offset.x, m_offset.y, m_offset.z));
	m_shape->setLocalPose(relativePose);

	switch (m_type)
	{
	case BodyType::Static:
		m_rigidStatic->attachShape(*m_shape);
		Collider::SetLayerCollision(m_owner.lock()->GetLayerCollision());
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->attachShape(*m_shape);
		PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, m_mass);
		Collider::SetLayerCollision(m_owner.lock()->GetLayerCollision());
		break;
	}

	Collider::UpdatePhysXProperties();
}

void SphereCollider::InitialializeClone(Matrix worldTransform, LayerCollision layer)
{
	m_colliderID = m_collisionManager->GetColliderID();
	m_physics = m_collisionManager->GetPhysics();
	m_mateiral = m_physics->createMaterial(m_dynamicFriction, m_staticFriction, m_restitution);

	m_isInitialize = true;
	m_shape = m_physics->createShape(
		PxSphereGeometry(static_cast<physx::PxReal>(m_radius)), &m_mateiral, 1, true);

	PxTransform relativePose = PxTransform(PxVec3(m_offset.x, m_offset.y, m_offset.z));
	m_shape->setLocalPose(relativePose);
	if (m_rigidDynamic) PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, m_mass);

	Collider::InitialializeClone(worldTransform, layer);
}

void SphereCollider::SetRadius(float radius)
{
	m_radius = radius;
	//if (m_radius <= 0) m_radius = 0.001f;

	//사이즈 변경
	switch (m_type)
	{
	case BodyType::Static:
		m_rigidStatic->detachShape(*m_shape);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->detachShape(*m_shape);
		break;
	}

	m_shape->release();
	m_shape = m_physics->createShape(
		PxSphereGeometry(static_cast<physx::PxReal>(m_radius)), &m_mateiral, 1, true);

	PxTransform relativePose = PxTransform(PxVec3(m_offset.x, m_offset.y, m_offset.z));
	m_shape->setLocalPose(relativePose);
	if (m_rigidDynamic) PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, m_mass);
	

	switch (m_type)
	{
	case BodyType::Static:
		m_rigidStatic->attachShape(*m_shape);
		Collider::SetLayerCollision(m_owner.lock()->GetLayerCollision());
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->attachShape(*m_shape);
		Collider::SetLayerCollision(m_owner.lock()->GetLayerCollision());
		break;
	}
}

