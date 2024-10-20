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
	// ��ġ �̵�
	float m_moveSpeed = 10.f; // ���ǵ�

	float m_turnSpeed = 7.f;						// ������ȯ �ӵ�: ���� Ŭ���� ����
	Vector3 m_targetVector = { 0.f, 0.f, 0.f };		// ������ ����: ����Ű�� ���� �ٷ� ��ȯ�ȴ�
	Vector3 m_headVector = { 0.f, 0.f, 1.f };		// ĳ���Ͱ� �ٶ󺸴� ����: m_targetVector�� ���� �ε巴�� ����

	bool m_isInteractive = false;
};

