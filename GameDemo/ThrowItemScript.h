#pragma once
#include <Script.h>

class ThrowItemScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(ThrowItemScript)

		// Serialize
		virtual void Serialize(nlohmann::json& json);
	virtual void Deserialize(const nlohmann::json& json);

public:
	virtual ~ThrowItemScript();

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	virtual void OnCollisionEnter(const Collision& collision);
	virtual void OnCollisionStay(const Collision& collision) {};
	virtual void OnCollisionExit(const Collision& collision) {};
	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	virtual std::shared_ptr<Component> Clone() const override;

private:
};

