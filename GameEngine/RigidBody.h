#pragma once

#include "Component.h"

class RigidBody : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(RigidBody)

public:
	RigidBody();
	virtual ~RigidBody() override;

	virtual void Update(double dt) abstract override;
	virtual void Render(double dt) abstract override;
	virtual void Finalize() override;

	virtual std::shared_ptr<Component> Clone() const override;

public:
	void AddForce(const Vector3& force);
	void MovePosition(const Vector3& position);
	void MoveRotation(const Vector3& rotation);

public:
	// Getters & Setters
	void SetMass(float mass) { m_mass = mass; }
	void SetDrag(float drag) { m_drag = drag; }
	void SetIsKinematic(bool isKinematic) { m_isKinematic = isKinematic; }
	void SetUseGravity(bool useGravity) { m_useGravity = useGravity; }
	void SetConstraintFreezeRotationX(bool freezeRotationX) { m_ConstraintFreezeRotationX = freezeRotationX; }
	void SetConstraintFreezeRotationY(bool freezeRotationY) { m_ConstraintFreezeRotationY = freezeRotationY; }
	void SetConstraintFreezeRotationZ(bool freezeRotationZ) { m_ConstraintFreezeRotationZ = freezeRotationZ; }

	float GetMass() const { return m_mass; }
	float GetDrag() const { return m_drag; }
	bool GetIsKinematic() const { return m_isKinematic; }
	bool GetUseGravity() const { return m_useGravity; }
	bool GetConstraintFreezeRotationX() const { return m_ConstraintFreezeRotationX; }
	bool GetConstraintFreezeRotationY() const { return m_ConstraintFreezeRotationY; }
	bool GetConstraintFreezeRotationZ() const { return m_ConstraintFreezeRotationZ; }

private:
	// Properties
	float m_mass;
	float m_drag;
	bool m_isKinematic;
	bool m_useGravity;
	bool m_ConstraintFreezeRotationX;
	bool m_ConstraintFreezeRotationY;
	bool m_ConstraintFreezeRotationZ;
};

