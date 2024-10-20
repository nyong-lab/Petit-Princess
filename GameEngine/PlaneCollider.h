#pragma once
#include "Collider.h"


/// <summary>
/// 평면 콜라이더. Static만 가능하다. 
/// 평면 아래에 있는 것은 모두 충돌로 판정한다.
/// 
/// 손서희
/// </summary>
class PlaneCollider : public Collider
{
	GENERATE_CLASS_TYPE_INFO(PlaneCollider)
		PROPERTY(m_normal)
		PROPERTY(m_distance)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlaneCollider,
			m_name, m_UID,
			m_shapeType, m_type, m_dynamicFriction, m_staticFriction, m_isTrigger,
			m_restitution, m_linearDamping, m_angularDamping, m_mass, m_disableGravity,
			m_freezePositionX, m_freezePositionY, m_freezePositionZ,
			m_freezeRotationX, m_freezeRotationY, m_freezeRotationZ,
			m_distance /*, m_normal*/)

		void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	PlaneCollider();
	virtual ~PlaneCollider() override;

	virtual void Start();
	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void DebugRender() override;
	virtual void Finalize() override;
	virtual std::shared_ptr<Component> Clone() const override;
	virtual void InitialializeClone(Matrix worldTransform, LayerCollision layer) override;
	virtual void UpdatePhysXProperties() override;

	void SetNormalAndDistance(Vector3 normal, float distance);

	//삭제된 함수들 
	void SetBodyType(BodyType type) = delete;
	void SetIsTrigger(bool isTrigger) = delete;
	void SetDisableGravity(bool arg) = delete;
	void SetDamping(float linear, float angular) = delete;
	void SetMass(float mass) = delete;
	void FreezePosition(bool x, bool y, bool z) = delete;
	void FreezeRotation(bool x, bool y, bool z) = delete;
	void AddForce(Vector3 force, ForceMode forceMode) = delete;
	void AddTorque(Vector3 torque, ForceMode forceMode) = delete;
	void SetLinearVelocity(Vector3 velocity) = delete;
	void SetAngularVeloctiy(Vector3 velocity) = delete;

private:
	Vector3 m_normal = Vector3(0, 1, 0);
	float m_distance = 0.f;
};

