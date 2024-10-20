#pragma once
#include "Component.h"
#include "Collider.h"

/// <summary>
/// 구 범위 충돌 체크 Collider Component

/// 작성자 : 손서희
/// 
/// 24.01.28 [수정] 김동일
/// collisionManager를 받아오는 생성자, 전방 선언, 헤더 파일 추가
/// </summary>
class SphereCollider : public Collider
{
	/// 기본 생성자 때문에 일단 주석 
	// Reflection
 	GENERATE_CLASS_TYPE_INFO(SphereCollider)
 		PROPERTY(m_radius)
		PROPERTY(m_offset)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SphereCollider,
	m_name, m_UID,
	m_shapeType, m_type, m_dynamicFriction, m_staticFriction, m_isTrigger,
	m_restitution, m_linearDamping, m_angularDamping, m_mass, m_disableGravity,
	m_freezePositionX, m_freezePositionY, m_freezePositionZ,
	m_freezeRotationX,m_freezeRotationY, m_freezeRotationZ,
	m_radius)

	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	// Constructors
	SphereCollider();
	virtual ~SphereCollider() override;

	// Public Methods
	virtual void Start();
	virtual void Update(double dt) override;
	virtual void Render(double dt) {}
	virtual void DebugRender() override;
	virtual void Finalize() override;
	virtual std::shared_ptr<Component> Clone() const override;

	virtual void UpdatePhysXProperties() override;
	virtual void InitialializeClone(Matrix worldTransform, LayerCollision layer) override;

public:
	// Getters & Setters
	void SetRadius(float radius);
	void SetOffset(const Vector3& offset) { m_offset = offset; }

private:
	// Properties
	float m_radius = 0.5f;
	Vector3 m_offset = Vector3(0.f, 0.f, 0.f);
};

