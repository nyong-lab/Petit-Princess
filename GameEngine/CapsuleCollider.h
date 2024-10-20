#pragma once
#include "Component.h"
#include "Collider.h"
#include "GraphicsTransfer.h"

class CollisionManager;

/// <summary>
/// 
/// 작성자 : 손서희
/// 
/// 24.01.28 [수정] 김동일
/// collisionManager를 받아오는 생성자, 전방 선언, 헤더 파일 추가
/// </summary>
class CapsuleCollider : public Collider
{
	/// 기본 생성자 때문에 일단 주석 
	//Reflection
	GENERATE_CLASS_TYPE_INFO(CapsuleCollider)
		PROPERTY(m_radius)
		PROPERTY(m_halfHeight)
		PROPERTY(m_offset)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(CapsuleCollider,
	m_name, m_UID,
	m_shapeType, m_type, m_dynamicFriction, m_staticFriction, m_isTrigger,
	m_restitution, m_linearDamping, m_angularDamping, m_mass, m_disableGravity,
	m_freezePositionX, m_freezePositionY, m_freezePositionZ,
	m_freezeRotationX, m_freezeRotationY, m_freezeRotationZ,
	m_radius, m_halfHeight)

	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	// Constructors
	CapsuleCollider();
	virtual ~CapsuleCollider() override;

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
	void SetSize(float radius, float halfHeight);
	void SetOffset(const Vector3& offset) { m_offset = offset; }

private:
	// Properties
	float m_radius			= 0.5f;
	float m_halfHeight		= 1.f;
	Vector3 m_offset		= Vector3(0.f, 0.f, 0.f);

private:
	bool m_isBufferInit = false;
	std::vector<std::wstring> m_constantBuffers;
};

