#pragma once
#include <Script.h>

class HideAutomaticScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(HideAutomaticScript)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(HideAutomaticScript, m_name, m_UID)
		virtual void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	virtual void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	HideAutomaticScript() {};
	~HideAutomaticScript() {};

	void Update(double dt) override;

	std::shared_ptr<Component> Clone() const override;

	virtual void OnTriggerEnter(Collider* collider) override;
	virtual void OnTriggerStay(Collider* collider)override;
	virtual void OnTriggerExit(Collider* collider)override;

private:
	bool* m_isPlayerHidden = nullptr;
};