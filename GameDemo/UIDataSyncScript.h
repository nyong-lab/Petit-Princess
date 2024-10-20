#pragma once
#include <../GameEngine/Script.h>

class UIMinimapCurPos;

class UIDataSyncScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(UIDataSyncScript)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(UIDataSyncScript, m_name, m_UID)
	virtual void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	virtual void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	UIDataSyncScript() {};
	~UIDataSyncScript() {};

	void Update(double dt) override;

	std::shared_ptr<Component> Clone() const override;

	virtual void OnTriggerEnter(Collider* collider) override;
	virtual void OnTriggerStay(Collider* collider)override;
	virtual void OnTriggerExit(Collider* collider)override;

	void Start();

private:
	// 이 스크립트를 가진 엔티티가 관리할 엔티티들
	std::shared_ptr<Entity> m_player;
	std::shared_ptr<Entity> m_minimapCurPos;
};