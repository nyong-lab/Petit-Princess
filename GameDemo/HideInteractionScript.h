#pragma once
#include <Script.h>
class HideInteractionScript :
	public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(HideInteractionScript)
	PROPERTY(m_canPlayerHide)
	PROPERTY(m_hideCoolTime)
	PROPERTY(m_canRenderUi)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(HideInteractionScript, m_canPlayerHide, m_hideCoolTime, m_canRenderUi)
		void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	HideInteractionScript() {};
	virtual ~HideInteractionScript() {};

	virtual void Update(double dt) override;

	virtual void OnCollisionEnter(const Collision& collision) {};
	virtual void OnCollisionStay(const Collision& collision) {};
	virtual void OnCollisionExit(const Collision& collision) {};
	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	virtual std::shared_ptr<Component> Clone() const override;

	bool GetInteractive() const { return m_canRenderUi; }

private:
	// 플레이어가 접근해서 상호작용 가능한가?
	bool m_canPlayerHide = true;		

	// 플레이어가 숨었나?
	bool* m_isPlayerHidden = nullptr;			

	// 숨기 가능 쿨타임
	float m_hideCoolTime = 3.f;

	// 숨기 가능 UI 렌더 가능 상태인가?
	bool m_canRenderUi = true;
};

