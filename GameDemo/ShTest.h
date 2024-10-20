#pragma once
#include <Script.h>
class ShTest : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(ShTest)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ShTest, m_name, m_UID)
		void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	ShTest();
	~ShTest();

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	virtual std::shared_ptr<Component> Clone() const override { return std::make_shared<ShTest>(); }
};

