#pragma once

#include <Script.h>

class DongilScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(DongilScript)
		PROPERTY(m_moveSpeed)
		PROPERTY(m_turnSpeed)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(DongilScript, m_name, m_UID, m_moveSpeed, m_turnSpeed)
		void Serialize(nlohmann::json& json) override;
	void Deserialize(const nlohmann::json& json) override;

public:
	DongilScript() {};
	~DongilScript() {};

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual std::shared_ptr<Component> Clone() const override;
	
private:
	// 위치 이동
	float m_moveSpeed = 10.f; // 스피드

	float m_turnSpeed = 7.f;						// 방향전환 속도: 숫자 클수록 빠름
	Vector3 m_targetVector = { 0.f, 0.f, 0.f };		// 가려는 방향: 방향키에 따라 바로 전환된다
	Vector3 m_headVector = { 0.f, 0.f, 1.f };		// 캐릭터가 바라보는 방향: m_targetVector를 토대로 부드럽게 계산됨

	bool m_isInteractive = false;
};

