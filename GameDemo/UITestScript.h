#pragma once
#include "Script.h"

enum class UIType
{
	STATIC,
	ONMOUSE, 
	GAUGE,
	Jiwon,
	HUNGRY, 
	SENSE,
	DELICIOUS_FOOD, 
	REVISIT_COOLTIME,
	MINIMAP,
	MINIMAPCURPOS,
	SKILL,
	FADE,
};

class Entity;
class Transform;
class TextRender;

class UITestScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(UITestScript)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(UITestScript, m_name, m_UID, m_type, m_isMinimapOn)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	UITestScript();
	~UITestScript();
	UITestScript(UIType type);

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	virtual std::shared_ptr<Component> Clone() const override;
	float GetDuration() { return m_duration; }
	void SetDuration(float val) { m_duration = val; }
	UIType GetType() { return m_type; }
	
	float GetGaugePower() const { return m_gaugePower; }
	float GetHungry() const { return *m_hungryPointer; }
	void SetHungry(float* pHungry) 
	{ 
		if (pHungry == nullptr)
			return;
		m_hungryPointer = pHungry; 
	}

	bool GetIsMinimapOn() { return m_isMinimapOn; }
	void SetPlayerPointer(std::weak_ptr<Entity> entity) { m_player = entity;}
	Matrix GetPlayerWorldTM();
	Vector3 GetPlayerPosition();
	bool GetIsFirst() { return m_isFirst; }

private:
	float m_duration;
	float m_increase;
	bool m_isEndDuration;
	bool m_isFirstUpdate;
	UIType m_type;
	DirectX::SimpleMath::Vector3 m_position;

	// ���� ���� ������� UI
	bool m_aimModeActivated = false;							// �������� ���� �ִ°�
	float m_timeSinceLClick = 0.f;								// Ŭ�� �� ���� �ð�
	//std::shared_ptr<UIGauge> m_aimGaugeBarUI = nullptr;		// ���� ������
	bool m_isFirst;

	float m_gaugePower = 0.f;									// ������ ��ġ
	float m_gaugeSpeed = 0.5f;									// ������ �Դٰ��ٰŸ��� �ӵ�
	bool m_aimGaugeGoingUp = true;								// ������ ����
	Matrix m_playerTM;

	float* m_hungryPointer;										// ����� ��ġ
	
	// ����
	bool m_isSenseActive = false;

	// �̴ϸ�
	bool m_isMinimapOn = false;
	std::weak_ptr<Entity> m_player;

	// �ؽ�Ʈ ���
	std::shared_ptr<TextRender> m_textRender;
	
	// ��ų 
	int m_skillCount;

	// fade
	float m_fade;
};

