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
	// �÷��̾ �����ؼ� ��ȣ�ۿ� �����Ѱ�?
	bool m_canPlayerHide = true;		

	// �÷��̾ ������?
	bool* m_isPlayerHidden = nullptr;			

	// ���� ���� ��Ÿ��
	float m_hideCoolTime = 3.f;

	// ���� ���� UI ���� ���� �����ΰ�?
	bool m_canRenderUi = true;
};

