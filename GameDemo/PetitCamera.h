#pragma once


#include "../GameEngine/Script.h"

/// <summary>
/// 플레이어를 따라다니는 탑뷰 인게임 카메라
/// 손서희
/// </summary>
class PetitCamera : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(PetitCamera)
		PROPERTY(m_topBoundary)
		PROPERTY(m_bottomBoundary)
		PROPERTY(m_rightBoundary)
		PROPERTY(m_leftBoundary)
		PROPERTY(m_posOffset)
		PROPERTY(m_yaw)
		PROPERTY(m_pitch)
		PROPERTY(m_cameraChaseSpeed)
		PROPERTY(m_chasingStartDistance)
		PROPERTY(m_chasingStopDistance)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PetitCamera, m_name, m_UID)
	void Serialize(nlohmann::json& json) override;
	void Deserialize(const nlohmann::json& json) override;

//생성자와 소멸자 ---------------------------------------------------------------------------------------
public:
	PetitCamera();
	~PetitCamera();


// 컴포넌트 함수 ----------------------------------------------------------------------------------------
	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	virtual std::shared_ptr<Component> Clone() const override;

// 멤버 함수 ----------------------------------------------------------------------------------------
	void SetTarget(std::shared_ptr<Entity> entity) { m_target = entity; }
	void SetBoundary(float top, float bottom, float right, float left) { m_topBoundary = top, m_bottomBoundary = bottom, m_rightBoundary = right, m_leftBoundary = left; }

// 멤버 변수 ----------------------------------------------------------------------------------------
private:
	std::shared_ptr<CameraModule> m_mainCamera;
	std::shared_ptr<Entity> m_target;
	bool m_isChasing;

	float m_topBoundary;
	float m_bottomBoundary;
	float m_rightBoundary;
	float m_leftBoundary;

	Vector3 m_posOffset;
	float m_yaw;
	float m_pitch;
	float m_cameraChaseSpeed;

	float m_chasingStartDistance;
	float m_chasingStopDistance;
};

