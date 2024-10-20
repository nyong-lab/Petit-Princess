#pragma once

class CollisionManager;
class CameraModule;
class Scene;
#include "Component.h"
#include "EntityDefine.h"
#include <PxPhysicsAPI.h>
using namespace physx;
#include <set>


enum class BodyType
{
	Static,
	Dynamic,
	//Kinematic
};

enum class ForceMode
{
	Force,			// �ð����� ������ ũ��� ������ ����
	Impulse,		// ���� ũ��� ������ ���������� �����
	VelocityChange,	// �ﰢ���� �ӵ� ��ȭ
	Acceleration    // ������ ������� ���� ���ӵ� ��ȭ
};


/// <summary>
/// �浹������ ���� ������ ����� ������Ʈ
/// �⺻������ Dynamic���� �����ȴ�. 
/// 
/// �ռ���
/// 
/// 2024.02.06 00:11
/// ������ : �赿�� -> collider ���� ��쿡�� ������ �ܰ迡�� �̺�Ʈ �Ŵ����� ���Ͽ� ���� �����޴´�.
/// ������ ��ø�������� �ϴ� �������� �ݶ��̴��� �� �����ӿ� ����������� start �Լ��� ȣ��޴°Ͱ�
/// ���� �����޴°� ���� �����ӿ��� �Ǳ⿡ SetScene �Լ��� �߰��Ͽ� �˸��� scene�� �����ް� start�� ȣ���Ų��. 
class Collider : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Collider)
		PROPERTY(m_type)
		PROPERTY(m_isTrigger)
		PROPERTY(m_disableGravity)
		PROPERTY(m_dynamicFriction)
		PROPERTY(m_staticFriction)
		PROPERTY(m_restitution)
		PROPERTY(m_linearDamping)
		PROPERTY(m_angularDamping)
		PROPERTY(m_mass)

	// Serialize
	virtual void Serialize(nlohmann::json& json) abstract;
	virtual void Deserialize(const nlohmann::json& json) abstract;

protected:
	enum class ShapeType 
	{
		Plane,
		NonePlane,
	};

/// �����ڿ� �Ҹ��� -------------------------------------------------------------------------------------------
public:
	Collider();
	~Collider();
	virtual void Update(double dt);
	virtual void FixedUpdate(double FixedTime);
	virtual void Start();
	virtual void DebugRender();
	virtual void Finalize() override;

	// ���纻�� ������ ��ȯ.
	virtual std::shared_ptr<Component> Clone() const override;

	virtual void UpdatePhysXProperties();
	unsigned int GetColliderID() { return m_colliderID; }
	void UpdateTransform(Vector3 worldPosition, Quaternion worldQuaternion);
	void CopyMembersThisToOther(std::shared_ptr<Collider> other) const;
	virtual void InitialializeClone(Matrix worldTransform, LayerCollision layer);
	void SetLayerCollision(LayerCollision layer);

// �浹 �ݹ� -----------------------------------------------------------------------------------------------------
public:
	// Messages
	void OnCollisionEnter(const CollisionInfo& collisionInfo);
	void OnCollisionStay();
	void OnCollisionExit(const CollisionInfo& collisionInfo);
	void OnTriggerEnter(ColliderInfo* otherColliderInfo);
	void OnTriggerStay();
	void OnTriggerExit(ColliderInfo* otherColliderInfo);

protected:
	void CallbackDeleteTrigger(unsigned int id);
	void CallbackDeleteContact(unsigned int id);

// Setter -----------------------------------------------------------------------------------------------------
public:
	void SetBodyType(BodyType type);
	void SetIsTrigger(bool isTrigger);
	void SetDisableGravity(bool arg);
	void SetMaterial(float dynamicFriction, float staticFriction, float restitution);
	void SetDamping(float linear, float angular);
	void SetMass(float mass);
	void FreezePosition(bool x, bool y, bool z);
	void FreezeRotation(bool x, bool y, bool z);


	// ���� �߰�
	void SetScene(std::shared_ptr<Scene> scene) { m_scene = scene; }

// ���� �ۿ� -----------------------------------------------------------------------------------------------------
	void AddForce(Vector3 force, ForceMode forceMode);
	void AddTorque(Vector3 torque, ForceMode forceMode);
	void ClearForce(ForceMode forceMode = ForceMode::Force);
	void CleareTorque(ForceMode forceMode = ForceMode::Force);
	void SetLinearVelocity(Vector3 velocity);
	void SetAngularVeloctiy(Vector3 velocity);

protected:
	PxTransform MatToPxTransform(Matrix mat);
	//Matrix PxTransformToMatrix(PxTransform);

/// ��� ���� -----------------------------------------------------------------------------------------------------
protected:
	unsigned int m_colliderID;			// �浹ü�� ���̵�� ������ ���� ������.
	bool m_isInitialize = false;
	CollisionManager* m_collisionManager;
	PxPhysics* m_physics;
	ShapeType m_shapeType = ShapeType::NonePlane;
	std::shared_ptr<Scene> m_scene;

	// ����
	BodyType m_type = BodyType::Dynamic;
	PxRigidDynamic* m_rigidDynamic;
	PxRigidStatic* m_rigidStatic;
	PxShape* m_shape;
	bool m_isTrigger			= false;
	

	// Material
	PxMaterial* m_mateiral;
	float m_dynamicFriction		= 0.5f;   // ���� ����
	float m_staticFriction		= 0.5f;   // ���� ����
	float m_restitution			= 0.3f;	  // �ݹ߷� (0.0 ��ź���浹 ~ 1.0 ź���浹) 
	float m_linearDamping		= 0.0f;   // ��� ����Ǵ� ���ӵ� (0.0 ~ 1, 0.47:������ ��ü, 0.2:����,�����)
	float m_angularDamping		= 0.05f;
	float m_mass				= 10.f;

	// ������
	bool m_disableGravity		= false;
	bool m_freezePositionX		= false;
	bool m_freezePositionY		= false;
	bool m_freezePositionZ		= false;
	bool m_freezeRotationX		= false;
	bool m_freezeRotationY		= false;
	bool m_freezeRotationZ		= false;
	
	// �浹 �����, ������Ʈ �ݹ� ����
	std::map<unsigned int, Collision> m_contactOthers;
	std::map<unsigned int, Collider*> m_triggerOthers;

	/// ** Debug Render **
	bool m_isModelCreate = false;
	std::wstring m_modelKey = L"";
	GraphicsTransfer::ModelKey m_cloneModelKey;	 // Clone�� ���� ���� Ű
};