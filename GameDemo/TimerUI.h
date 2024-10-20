#pragma once
#include "Script.h"

class GameManager;
class TextRender;

/// <summary>
/// �ռ���
/// </summary>
class TimerUI : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(TimerUI)
		PROPERTY(m_position)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TimerUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//�����ڿ� �Ҹ��� ---------------------------------------------------------------------------------------
public:
	TimerUI();
	~TimerUI();

// ������Ʈ �Լ� ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// ��� �Լ� ----------------------------------------------------------------------------------------


// ��� ���� ----------------------------------------------------------------------------------------
private:
	std::shared_ptr<GameManager> m_mgr;
	std::weak_ptr<IGraphics> m_igraphics;
	std::shared_ptr<TextRender> m_textRender;

	float m_timer = 0.f;  //���� �޴������� �� ������Ʈ ���� �޾ƿ´�
	float m_endTime = 0.f;  //���� �Ŵ������� �޾ƿ´�


	// ~~~Render~~~ �Ƹ��� �������� �ۺ�
public:
	Vector2 m_position = Vector2(1750,200);
	Vector2 m_imgSize= Vector2(312,312);
	float m_startAngle = -75.f;
	float m_endAngle = 75.f;
	float m_curAngle = 0.f;
};

