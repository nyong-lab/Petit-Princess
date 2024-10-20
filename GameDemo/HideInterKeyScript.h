#pragma once

#include "Script.h"

//class FoodTableScript;
//class TechniquesScript;

class HideInteractionScript;

class HideInterKeyScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(HideInterKeyScript)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(HideInterKeyScript, m_name, m_UID)

		void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	HideInterKeyScript() {}
	virtual void Update(double dt) override;
	virtual void Render(double dt) override;

	virtual std::shared_ptr<Component> Clone() const override;

public:
	bool GetInteractive();
	Vector3 GetObjectPosition();

public:
	//std::shared_ptr<FoodTableScript> m_foodTableScript;
	//std::shared_ptr<TechniquesScript> m_techniquesScript;
	std::shared_ptr<HideInteractionScript> m_hideInteractionScript;
};

