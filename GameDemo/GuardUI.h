#pragma once
#include "Script.h"

#include "GuardFSM.h"

/// <summary>
/// ¼Õ¼­Èñ
/// </summary>
class GuardUI : public Script
{
		// Reflection
	GENERATE_CLASS_TYPE_INFO(GuardUI)
		PROPERTY(m_state)
		PROPERTY(m_pos)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GuardUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//»ý¼ºÀÚ¿Í ¼Ò¸êÀÚ ---------------------------------------------------------------------------------------
public:
	GuardUI();
	//GuardUI(std::shared_ptr<GuardFSM> fsm);
	~GuardUI();

// ÄÄÆ÷³ÍÆ® ÇÔ¼ö ----------------------------------------------------------------------------------------
	virtual void Start();
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// ¸â¹ö ÇÔ¼ö ----------------------------------------------------------------------------------------

// ¸â¹ö º¯¼ö ----------------------------------------------------------------------------------------
public:
	int m_state;
	Vector3 m_pos;

private:
	std::shared_ptr<GuardFSM> m_guard;
};

