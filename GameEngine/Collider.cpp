#pragma once
#include "GameEngine_pch.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "Scene.h"
#include "Collider.h"
#include "Entity.h"
#include "CameraModule.h"
#include "SceneManager.h"

using namespace physx;

/// ------------------------------------------------------------------------------------------------
///								          �����ڿ� �Ҹ���
/// -------------------------------------------------------------------------------------------------

Collider::Collider()
	:m_colliderID(0), m_physics(nullptr), m_mateiral(nullptr), m_rigidDynamic(nullptr), m_shape(nullptr), m_rigidStatic(nullptr),
	m_collisionManager(nullptr)
{
	m_collisionManager = CollisionManager::GetInstance();
	m_colliderID = m_collisionManager->GetColliderID();
	m_physics = m_collisionManager->GetPhysics();
	m_mateiral = m_physics->createMaterial(m_dynamicFriction, m_staticFriction, m_restitution);
	m_scene = EventManager::GetInstance()->GetCurScene();
}

Collider::~Collider()
{

}

/// --------------------------------------------------------------------------------------------------
///								         ������Ʈ �Լ���
/// --------------------------------------------------------------------------------------------------

void Collider::Start()
{

}

void Collider::Update(double dt)
{
	// Ʈ���������� ������ ���� PhysX����� �Ѱ��ش�. ������Ʈ ������ ���� �ļ����� ����Ǿ�� �Ѵ�. (������ ����)
	Matrix mat = m_owner.lock()->GetComponent<Transform>()->GetWorldMatrix();
	switch (m_type)
	{
	case BodyType::Static:
		m_rigidStatic->setGlobalPose(MatToPxTransform(mat));
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setGlobalPose(MatToPxTransform(mat));
		break;
	}
}

void Collider::FixedUpdate(double FixedTime)
{
	// Stay �ݹ� ����
	OnCollisionStay();
	OnTriggerStay();
}

void Collider::DebugRender()
{

}

void Collider::Finalize()
{
	auto a = this;
	// �ٸ� �ݶ��̴��� �浹 ��Ͽ��� ����
	for (auto& col : m_contactOthers)
	{
		if(col.second.otherCollider)
			col.second.otherCollider->CallbackDeleteContact(m_colliderID);
	}	

	for (auto& col : m_triggerOthers)
	{
		if(col.second)
			col.second->CallbackDeleteTrigger(m_colliderID);
	}		

	ColliderInfo* info;

	switch (m_type)
	{
	case BodyType::Static:
		if (m_owner.lock()->GetSceneManager().lock()->GetCurScene()->IsSimulating()) m_collisionManager->RemoveActorFormScene(m_rigidStatic);
		info = static_cast<ColliderInfo*>(m_rigidStatic->userData);
		info->collider = nullptr;
		delete m_rigidStatic->userData;
		m_rigidStatic->userData = nullptr;
		break;

	case BodyType::Dynamic:
	
		if (m_owner.lock()->GetSceneManager().lock()->GetCurScene()->IsSimulating())m_collisionManager->RemoveActorFormScene(m_rigidDynamic);
		info = static_cast<ColliderInfo*>(m_rigidDynamic->userData);
		info->collider = nullptr;
		delete m_rigidDynamic->userData;
		m_rigidDynamic->userData = nullptr;
		break;
	}
}

std::shared_ptr<Component> Collider::Clone() const
{
	//std::shared_ptr<Collider> newCol = std::make_shared<Collider>();
	assert(false && L"�߸��� ȣ���Դϴ�");
	return nullptr;
}

/// --------------------------------------------------------------------------------------------------
///								         ������ ���� & ������Ʈ
/// --------------------------------------------------------------------------------------------------

void Collider::UpdatePhysXProperties()
{
	if (m_restitution < 0.f) m_restitution = 0.f;
	if (m_restitution > 1.f) m_restitution = 1.f;
	if (m_linearDamping < 0.f) m_linearDamping = 0.f;
	if (m_angularDamping < 0.f) m_angularDamping = 0.f;
	if (m_mass <= 0.f) m_mass = 0.001f;

	bool isAddScene = m_scene->IsPrevSimulating();

	// ���̳��Ϳ��� ����ƽ���� �ٲ� ��� 
	if (m_type == BodyType::Static && m_rigidStatic == nullptr)
	{
		Matrix mat = m_owner.lock()->GetComponent<Transform>()->GetWorldMatrix();
		m_rigidStatic = m_physics->createRigidStatic(MatToPxTransform(mat));
		m_rigidDynamic->detachShape(*m_shape);
		m_rigidStatic->attachShape(*m_shape);

		if (m_scene->IsPrevSimulating())
		{
			m_collisionManager->RemoveActorFormScene(m_rigidDynamic); 
			isAddScene = false;
		}

		m_rigidStatic->userData = m_rigidDynamic->userData;
		m_rigidDynamic->release();
		m_rigidDynamic = nullptr;
	}

	// ����ƽ���� ���̳������� �ٲ� ���
	if (m_type == BodyType::Dynamic && m_rigidDynamic == nullptr)
	{
		Matrix mat = m_owner.lock()->GetComponent<Transform>()->GetWorldMatrix();
		m_rigidDynamic = m_physics->createRigidDynamic(MatToPxTransform(mat));
		m_rigidStatic->detachShape(*m_shape);
		m_rigidDynamic->attachShape(*m_shape);

		if (m_scene->IsPrevSimulating())
		{
			m_collisionManager->RemoveActorFormScene(m_rigidStatic); 
			isAddScene = false;
		}
		m_rigidDynamic->userData = m_rigidStatic->userData;
		m_rigidStatic->release();
		m_rigidStatic = nullptr;
	}

	switch (m_type)
	{
	case BodyType::Static:
	{
		// �߷�
		m_rigidStatic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m_disableGravity);

		// ������ ����
		SetLayerCollision(m_owner.lock()->GetLayerCollision());
		m_rigidStatic->detachShape(*m_shape);
		if (m_isTrigger)
		{
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}
		else
		{
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}

		m_rigidStatic->attachShape(*m_shape);

		// ���׸���
		m_mateiral->setStaticFriction(m_staticFriction);
		m_mateiral->setDynamicFriction(m_dynamicFriction);
		m_mateiral->setRestitution(m_restitution);

		if (m_scene->IsSimulating() && !isAddScene) m_collisionManager->AddActorToScene(m_rigidStatic);
		if (!m_scene->IsSimulating() && isAddScene) m_collisionManager->RemoveActorFormScene(m_rigidStatic);
		break;
	}
	case BodyType::Dynamic:
	{
		// �߷�
		m_rigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m_disableGravity);

		// ������ ����
		SetLayerCollision(m_owner.lock()->GetLayerCollision());
		m_rigidDynamic->detachShape(*m_shape);
		if (m_isTrigger)
		{
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}
		else
		{
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}
		m_rigidDynamic->attachShape(*m_shape);

		// ���׸���
		m_mateiral->setStaticFriction(m_staticFriction);
		m_mateiral->setDynamicFriction(m_dynamicFriction);
		m_mateiral->setRestitution(m_restitution);

		// ����
		m_rigidDynamic->setLinearDamping(m_linearDamping);
		m_rigidDynamic->setAngularDamping(m_angularDamping);

		// ����
		PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, m_mass);

		//������
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, m_freezePositionX); // X �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, m_freezePositionY); // Y �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, m_freezePositionZ); // Z �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, m_freezeRotationX); // X �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, m_freezeRotationY); // Y �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, m_freezeRotationZ); // Z �� ����

		if (m_scene->IsSimulating() && !isAddScene) 
			m_collisionManager->AddActorToScene(m_rigidDynamic);
		if (!m_scene->IsSimulating() && isAddScene) 
			m_collisionManager->RemoveActorFormScene(m_rigidDynamic);
		break;
	}
	}
}

void Collider::UpdateTransform(Vector3 worldPosition, Quaternion worldQuaternion)
{
	// PhysX ���� �ùķ��̼� ����� Transform�� �����Ѵ�.
	m_owner.lock()->GetComponent<Transform>()->SetWorldPosition(worldPosition);
	m_owner.lock()->GetComponent<Transform>()->SetWorldRotation(worldQuaternion);
}

void Collider::CopyMembersThisToOther(std::shared_ptr<Collider> other) const
{
	other->m_isTrigger			= this->m_isTrigger;
	other->m_type				= this->m_type;
	other->m_shapeType			= this->m_shapeType;
	other->m_dynamicFriction	= this->m_dynamicFriction;
	other->m_staticFriction		= this->m_staticFriction;
	other->m_restitution		= this->m_restitution;
	other->m_linearDamping		= this->m_linearDamping;
	other->m_angularDamping		= this->m_angularDamping;
	other->m_mass				= this->m_mass;
	other->m_disableGravity		= this->m_disableGravity;
	other->m_freezePositionX	= this->m_freezePositionX;
	other->m_freezePositionY	= this->m_freezePositionY;
	other->m_freezePositionZ	= this->m_freezePositionZ;
	other->m_freezeRotationX	= this->m_freezeRotationX;
	other->m_freezeRotationY	= this->m_freezeRotationY;
	other->m_freezeRotationZ	= this->m_freezeRotationZ;

	other->m_scene				= this->m_scene;
}

void Collider::InitialializeClone(Matrix worldTransform, LayerCollision layer)
{
	switch (m_type)
	{
	case BodyType::Static:
		m_rigidStatic = m_physics->createRigidStatic(MatToPxTransform(worldTransform));
		m_rigidStatic->attachShape(*m_shape);
		m_rigidStatic->userData = new ColliderInfo(this, m_colliderID);
		if(m_owner.lock())
			Collider::SetLayerCollision(m_owner.lock()->GetLayerCollision());
		m_rigidStatic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m_disableGravity);
		m_rigidStatic->detachShape(*m_shape);
		m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
		m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
		m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		m_rigidStatic->attachShape(*m_shape);
		m_mateiral->setStaticFriction(m_staticFriction);
		m_mateiral->setDynamicFriction(m_dynamicFriction);
		m_mateiral->setRestitution(m_restitution);

		if (m_scene->IsSimulating()) m_collisionManager->AddActorToScene(m_rigidStatic); ///TODO::�ùķ��̼� ���� üũ
		break;

	case BodyType::Dynamic:
		m_rigidDynamic = m_physics->createRigidDynamic(MatToPxTransform(worldTransform));
		m_rigidDynamic->attachShape(*m_shape);
		m_rigidDynamic->userData = new ColliderInfo(this, m_colliderID);
		m_rigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m_disableGravity);
		if(m_owner.lock())
			Collider::SetLayerCollision(m_owner.lock()->GetLayerCollision());
		m_rigidDynamic->detachShape(*m_shape);
		m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
		m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
		m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		m_rigidDynamic->attachShape(*m_shape);
		m_mateiral->setStaticFriction(m_staticFriction);
		m_mateiral->setDynamicFriction(m_dynamicFriction);
		m_mateiral->setRestitution(m_restitution);
		m_rigidDynamic->setLinearDamping(m_linearDamping);
		m_rigidDynamic->setAngularDamping(m_angularDamping);
		PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, m_mass);
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, m_freezePositionX); // X �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, m_freezePositionY); // Y �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, m_freezePositionZ); // Z �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, m_freezeRotationX); // X �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, m_freezeRotationY); // Y �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, m_freezeRotationZ); // Z �� ����

		if (m_scene->IsSimulating()) m_collisionManager->AddActorToScene(m_rigidDynamic); 
	}

	SetLayerCollision(layer);
}

void Collider::SetLayerCollision(LayerCollision layer)
{
	if (m_shapeType == ShapeType::Plane)
		return;

	switch (m_type)
	{
	case BodyType::Static:
		if(m_rigidStatic)
			m_rigidStatic->detachShape(*m_shape);
		m_collisionManager->SetUpFiltering(m_shape, layer);
		if (m_rigidStatic)
			m_rigidStatic->attachShape(*m_shape);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->detachShape(*m_shape);
		m_collisionManager->SetUpFiltering(m_shape, layer);
		m_rigidDynamic->attachShape(*m_shape);
		break;
	}
}

/// --------------------------------------------------------------------------------------------------
///								         �浹 �ݹ�
/// --------------------------------------------------------------------------------------------------

void Collider::OnCollisionEnter(const CollisionInfo& collisionInfo)
{
	// �浹���� ��Ͽ� �߰�
	m_contactOthers[collisionInfo.otherId] = collisionInfo.collision;

	// ��ƼƼ�� �ݹ� ����
	GetOwner().lock()->OnCollisionEnter(collisionInfo.collision);
}

void Collider::OnCollisionStay()
{
	// ����Ʈ ���� ���� ���鼭 ��ƼƼ�� ����
	for (auto& cols : m_contactOthers)
	{
		GetOwner().lock()->OnCollisionStay(cols.second);
	}
}

void Collider::OnCollisionExit(const CollisionInfo& collisionInfo)
{
	// �浹���� ��Ͽ��� ����
	m_contactOthers.erase(collisionInfo.otherId);

	// ��ƼƼ�� �ݹ� ����
	GetOwner().lock()->OnCollisionExit(collisionInfo.collision);
}

void Collider::OnTriggerEnter(ColliderInfo* otherColliderInfo)
{
	// Ʈ�������� ��Ͽ� �߰�
	m_triggerOthers[otherColliderInfo->id] = otherColliderInfo->collider;

	// ��ƼƼ�� �ݹ� ����
	GetOwner().lock()->OnTriggerEnter(otherColliderInfo->collider);
}

void Collider::OnTriggerStay()
{
	// Ʈ�������� ���� ���鼭 ��ƼƼ�� ����
	for (auto& cols : m_triggerOthers)
	{
		GetOwner().lock()->OnTriggerStay(cols.second);
	}
}

void Collider::OnTriggerExit(ColliderInfo* otherColliderInfo)
{
	// Ʈ�������� ��Ͽ��� ����
	if(otherColliderInfo)
		m_triggerOthers.erase(otherColliderInfo->id);

	// ��ƼƼ�� �ݹ� ����
	if(otherColliderInfo != nullptr)
		GetOwner().lock()->OnTriggerExit(otherColliderInfo->collider);
	else
		GetOwner().lock()->OnTriggerExit(nullptr);
}

void Collider::CallbackDeleteTrigger(unsigned int id)
{
	// Ʈ�������� ��Ͽ��� ����
	m_triggerOthers.erase(id);
}

void Collider::CallbackDeleteContact(unsigned int id)
{
	// ����Ʈ ���� ��Ͽ��� ����
	m_contactOthers.erase(id);
}

/// --------------------------------------------------------------------------------------------------
///								         Setters
/// --------------------------------------------------------------------------------------------------

void Collider::SetBodyType(BodyType type)
{
	m_type = type;

	if(m_isInitialize)
		Collider::UpdatePhysXProperties();
	
	else
	{
		m_rigidDynamic->release();
		Matrix mat = Matrix::Identity;
		m_rigidStatic = m_physics->createRigidStatic(MatToPxTransform(mat));
		m_rigidStatic->attachShape(*m_shape);
		m_rigidStatic->userData = new ColliderInfo(this, m_colliderID);
	}
}

void Collider::SetIsTrigger(bool isTrigger)
{
	m_isTrigger = isTrigger;

	switch (m_type)
	{
	case BodyType::Static:
		m_rigidStatic->detachShape(*m_shape);
		if (m_isTrigger)
		{
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}
		else
		{
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}
		m_rigidStatic->attachShape(*m_shape);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->detachShape(*m_shape);
		if (m_isTrigger)
		{
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}
		else
		{
			m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, !m_isTrigger);
		}
		m_rigidDynamic->attachShape(*m_shape);
		break;
	}
}

void Collider::SetDisableGravity(bool arg)
{
	m_disableGravity = arg;

	switch (m_type)
	{
	case BodyType::Static:
		//m_rigidStatic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m_disableGravity);
		MessageBoxA(NULL, "static collider�� gravity�� ������� �ʽ��ϴ�.", "Collider �˸�", MB_OK);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m_disableGravity);
		break;
	}
}

void Collider::SetMaterial(float dynamicFriction, float staticFriction, float restitution)
{
	m_dynamicFriction = dynamicFriction;
	m_staticFriction = staticFriction;
	m_restitution = restitution;

	m_mateiral->setStaticFriction(m_staticFriction);
	m_mateiral->setDynamicFriction(m_dynamicFriction);
	m_mateiral->setRestitution(m_restitution);
}

void Collider::SetDamping(float linear, float angular)
{
	m_linearDamping = linear;
	m_angularDamping = angular;

	switch (m_type)
	{
	case BodyType::Static:
		MessageBoxA(NULL, "static collider�� damping�� ������� �ʽ��ϴ�.", "Collider �˸�", MB_OK);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setLinearDamping(m_linearDamping);
		m_rigidDynamic->setAngularDamping(m_angularDamping);
		break;
	}
}

void Collider::SetMass(float mass)
{
	m_mass = mass;
	switch (m_type)
	{
	case BodyType::Static:
		MessageBoxA(NULL, "static collider�� mass�� ������� �ʽ��ϴ�.", "Collider �˸�", MB_OK);
		break;

	case BodyType::Dynamic:
		PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, mass);
		break;
	}
}

void Collider::FreezePosition(bool x, bool y, bool z)
{
	m_freezePositionX = x;
	m_freezePositionY = y;
	m_freezePositionZ = z;

	switch (m_type)
	{
	case BodyType::Static:
		// PASS
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, m_freezePositionX); // X �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, m_freezePositionY); // Y �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, m_freezePositionZ); // Z �� ����
		break;
	}
}

void Collider::FreezeRotation(bool x, bool y, bool z)
{
	m_freezeRotationX = x;
	m_freezeRotationY = y;
	m_freezeRotationZ = z;

	switch (m_type)
	{
	case BodyType::Static:
		//PASS
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, m_freezeRotationX); // X �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, m_freezeRotationY); // Y �� ����
		m_rigidDynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, m_freezeRotationZ); // Z �� ����
		break;
	}
}

/// --------------------------------------------------------------------------------------------------
///								         ���� ����
/// --------------------------------------------------------------------------------------------------

void Collider::AddForce(Vector3 force, ForceMode forceMode)
{
	if (m_rigidDynamic == nullptr)
	{
		MessageBoxA(NULL, "����ƽ rigidBody���� AddForce�� ������� �ʽ��ϴ�.", "Collider ���", MB_OK);
		return;
	}

	switch (forceMode)
	{
	case ForceMode::Force:
		m_rigidDynamic->addForce(PxVec3(force.x, force.y, force.z), PxForceMode::eFORCE);
		break;

	case ForceMode::Impulse:
		m_rigidDynamic->addForce(PxVec3(force.x, force.y, force.z), PxForceMode::eIMPULSE);
		break;

	case ForceMode::VelocityChange:
		m_rigidDynamic->addForce(PxVec3(force.x, force.y, force.z), PxForceMode::eVELOCITY_CHANGE);
		break;

	case ForceMode::Acceleration:
		m_rigidDynamic->addForce(PxVec3(force.x, force.y, force.z), PxForceMode::eACCELERATION);
		break;
	}
}

void Collider::AddTorque(Vector3 torque, ForceMode forceMode)
{
	if (m_rigidDynamic == nullptr)
	{
		MessageBoxA(NULL, "����ƽ rigidBody���� AddTorque�� ������� �ʽ��ϴ�.", "Collider ���", MB_OK);
		return;
	}

	switch (forceMode)
	{
	case ForceMode::Force:
		m_rigidDynamic->addTorque(PxVec3(torque.x, torque.y, torque.z), PxForceMode::eFORCE);
		break;

	case ForceMode::Impulse:
		m_rigidDynamic->addTorque(PxVec3(torque.x, torque.y, torque.z), PxForceMode::eIMPULSE);
		break;

	case ForceMode::VelocityChange:
		m_rigidDynamic->addTorque(PxVec3(torque.x, torque.y, torque.z), PxForceMode::eVELOCITY_CHANGE);
		break;

	case ForceMode::Acceleration:
		m_rigidDynamic->addTorque(PxVec3(torque.x, torque.y, torque.z), PxForceMode::eACCELERATION);
		break;
	}
}

void Collider::ClearForce(ForceMode forceMode)
{
	if (m_rigidDynamic == nullptr)
	{
		MessageBoxA(NULL, "����ƽ rigidBody���� ClearForce�� ������� �ʽ��ϴ�.", "Collider ���", MB_OK);
		return;
	}

	switch (forceMode)
	{
	case ForceMode::Force:
		m_rigidDynamic->clearForce(PxForceMode::eFORCE);
		break;

	case ForceMode::Impulse:
		m_rigidDynamic->clearForce(PxForceMode::eFORCE);
		break;

	case ForceMode::VelocityChange:
		m_rigidDynamic->clearForce(PxForceMode::eFORCE);
		break;

	case ForceMode::Acceleration:
		m_rigidDynamic->clearForce(PxForceMode::eFORCE);
		break;
	}
}

void Collider::CleareTorque(ForceMode forceMode)
{
	if (m_rigidDynamic == nullptr)
	{
		MessageBoxA(NULL, "����ƽ rigidBody���� clearTorque�� ������� �ʽ��ϴ�.", "Collider ���", MB_OK);
		return;
	}

	switch (forceMode)
	{
	case ForceMode::Force:
		m_rigidDynamic->clearTorque(PxForceMode::eFORCE);
		break;

	case ForceMode::Impulse:
		m_rigidDynamic->clearTorque(PxForceMode::eFORCE);
		break;

	case ForceMode::VelocityChange:
		m_rigidDynamic->clearTorque(PxForceMode::eFORCE);
		break;

	case ForceMode::Acceleration:
		m_rigidDynamic->clearTorque(PxForceMode::eFORCE);
		break;
	}
}

void Collider::SetLinearVelocity(Vector3 velocity)
{
	switch (m_type)
	{
	case BodyType::Static:
		MessageBoxA(NULL, "����ƽ rigidBody���� SetLinearVelocity�� ������� �ʽ��ϴ�.", "Collider ���", MB_OK);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z), true);
		break;
	}
}

void Collider::SetAngularVeloctiy(Vector3 velocity)
{
	switch (m_type)
	{
	case BodyType::Static:
		MessageBoxA(NULL, "����ƽ rigidBody���� SetAngularVeloctiy�� ������� �ʽ��ϴ�.", "Collider ���", MB_OK);
		break;

	case BodyType::Dynamic:
		m_rigidDynamic->setAngularVelocity(PxVec3(velocity.x, velocity.y, velocity.z), true);
		break;
	}
}

/// --------------------------------------------------------------------------------------------------
///								         private �Լ�
/// --------------------------------------------------------------------------------------------------

physx::PxTransform Collider::MatToPxTransform(Matrix mat)
{
	Vector3 position;
	Quaternion quaternion;
	Vector3 scale;
	mat.Decompose(scale, quaternion, position);

	//������ ����
	//position += offset;

	// ������ ��ġ�� ȸ���� ����Ͽ� PxTransform ����
	physx::PxVec3 pxPosition(position.x, position.y, position.z);
	physx::PxQuat pxQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	//physx::PxQuat pxQuaternion(quaternion.w, quaternion.x, quaternion.y, quaternion.z );

	return physx::PxTransform(pxPosition, pxQuaternion);
}
