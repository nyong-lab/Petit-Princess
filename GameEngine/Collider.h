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
	Force,			// 시간동안 일정한 크기와 방향을 유지
	Impulse,		// 힘의 크기와 방향이 순간적으로 적용됨
	VelocityChange,	// 즉각적인 속도 변화
	Acceleration    // 질량을 고려하지 않은 가속도 변화
};


/// <summary>
/// 충돌감지와 물리 연산을 담당할 컴포넌트
/// 기본적으로 Dynamic으로 생성된다. 
/// 
/// 손서희
/// 
/// 2024.02.06 00:11
/// 수정자 : 김동일 -> collider 같은 경우에는 생성자 단계에서 이벤트 매니저를 통하여 씬을 배정받는다.
/// 하지만 디시리얼라이즈 하는 과정에서 콜라이더는 그 프레임에 만들어지지만 start 함수를 호출받는것과
/// 씬을 배정받는건 다음 프레임에서 되기에 SetScene 함수를 추가하여 알맞은 scene을 배정받고 start를 호출시킨다. 
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

/// 생성자와 소멸자 -------------------------------------------------------------------------------------------
public:
	Collider();
	~Collider();
	virtual void Update(double dt);
	virtual void FixedUpdate(double FixedTime);
	virtual void Start();
	virtual void DebugRender();
	virtual void Finalize() override;

	// 복사본을 만들어내서 반환.
	virtual std::shared_ptr<Component> Clone() const override;

	virtual void UpdatePhysXProperties();
	unsigned int GetColliderID() { return m_colliderID; }
	void UpdateTransform(Vector3 worldPosition, Quaternion worldQuaternion);
	void CopyMembersThisToOther(std::shared_ptr<Collider> other) const;
	virtual void InitialializeClone(Matrix worldTransform, LayerCollision layer);
	void SetLayerCollision(LayerCollision layer);

// 충돌 콜백 -----------------------------------------------------------------------------------------------------
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


	// 동일 추가
	void SetScene(std::shared_ptr<Scene> scene) { m_scene = scene; }

// 물리 작용 -----------------------------------------------------------------------------------------------------
	void AddForce(Vector3 force, ForceMode forceMode);
	void AddTorque(Vector3 torque, ForceMode forceMode);
	void ClearForce(ForceMode forceMode = ForceMode::Force);
	void CleareTorque(ForceMode forceMode = ForceMode::Force);
	void SetLinearVelocity(Vector3 velocity);
	void SetAngularVeloctiy(Vector3 velocity);

protected:
	PxTransform MatToPxTransform(Matrix mat);
	//Matrix PxTransformToMatrix(PxTransform);

/// 멤버 변수 -----------------------------------------------------------------------------------------------------
protected:
	unsigned int m_colliderID;			// 충돌체의 아이디는 고유한 값을 가진다.
	bool m_isInitialize = false;
	CollisionManager* m_collisionManager;
	PxPhysics* m_physics;
	ShapeType m_shapeType = ShapeType::NonePlane;
	std::shared_ptr<Scene> m_scene;

	// 형태
	BodyType m_type = BodyType::Dynamic;
	PxRigidDynamic* m_rigidDynamic;
	PxRigidStatic* m_rigidStatic;
	PxShape* m_shape;
	bool m_isTrigger			= false;
	

	// Material
	PxMaterial* m_mateiral;
	float m_dynamicFriction		= 0.5f;   // 동적 마찰
	float m_staticFriction		= 0.5f;   // 정지 마찰
	float m_restitution			= 0.3f;	  // 반발력 (0.0 비탄성충돌 ~ 1.0 탄성충돌) 
	float m_linearDamping		= 0.0f;   // 운동에 적용되는 감속도 (0.0 ~ 1, 0.47:공구형 물체, 0.2:차량,비행기)
	float m_angularDamping		= 0.05f;
	float m_mass				= 10.f;

	// 제한자
	bool m_disableGravity		= false;
	bool m_freezePositionX		= false;
	bool m_freezePositionY		= false;
	bool m_freezePositionZ		= false;
	bool m_freezeRotationX		= false;
	bool m_freezeRotationY		= false;
	bool m_freezeRotationZ		= false;
	
	// 충돌 디버깅, 업데이트 콜백 관련
	std::map<unsigned int, Collision> m_contactOthers;
	std::map<unsigned int, Collider*> m_triggerOthers;

	/// ** Debug Render **
	bool m_isModelCreate = false;
	std::wstring m_modelKey = L"";
	GraphicsTransfer::ModelKey m_cloneModelKey;	 // Clone을 위한 원본 키
};