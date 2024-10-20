#pragma once

#include "Component.h"
#include "Collider.h"

#include "GraphicsTransfer.h"

class CollisionManager;

/// <summary>
/// Box 범위 충돌 체크 Collider Component
/// </summary>
class BoxCollider : public Collider
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(BoxCollider)
		PROPERTY(m_size)
		PROPERTY(m_offset)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxCollider,
	m_name, m_UID,
	m_shapeType, m_type, m_dynamicFriction, m_staticFriction, m_isTrigger,
	m_restitution, m_linearDamping, m_angularDamping, m_mass, m_disableGravity,
	m_freezePositionX, m_freezePositionY, m_freezePositionZ,
	m_freezeRotationX, m_freezeRotationY, m_freezeRotationZ
	/*, m_size*/)

	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	// Constructors
	BoxCollider();
	virtual ~BoxCollider() override;

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
	void SetSize(const Vector3& size);
	void SetOffset(const Vector3& offset) { m_offset = offset; }

private:
	// Properties
	Vector3 m_size = Vector3(1.f, 1.f, 1.f);
	Vector3 m_offset = Vector3(0.f,0.f,0.f); 
};

