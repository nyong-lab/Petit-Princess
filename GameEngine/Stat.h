#pragma once
#include "Component.h"

/// <summary>
/// 2024.01.08
/// 전투를 담당할 컴포넌트
/// 작성 : 유승운
/// </summary>
class Stat : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Stat)
		PROPERTY(m_hp)
		PROPERTY(m_mp)
		PROPERTY(m_power)
		PROPERTY(m_defence)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Stat, m_name, m_UID, m_hp, m_mp, m_power, m_defence)
	virtual void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	virtual void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	Stat();
	~Stat();

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	virtual std::shared_ptr<Component> Clone() const override;

private:
	int m_hp = 0;
	int m_mp = 0;
	int m_power = 0;
	int m_defence = 0;

	//무기
};