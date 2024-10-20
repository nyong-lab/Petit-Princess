#pragma once
#include "Script.h"

/// <summary>
/// �ռ���
/// 
/// Ŭ�� �̺�Ʈ�� ���⼭ �� �ް�, �ڽ�
/// </summary>
class MainUI : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MainUI)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MainUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//�����ڿ� �Ҹ��� ---------------------------------------------------------------------------------------
public:
	MainUI();
	~MainUI();

// ������Ʈ �Լ� ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// ��� �Լ� ----------------------------------------------------------------------------------------
	float GetFade() { return m_fade; }
	void FaidOut(double dt);
// ��� ���� ----------------------------------------------------------------------------------------
private:
	std::weak_ptr<IGraphics> m_igraphics;


	// ~~~Render~~~ �Ƹ��� �������� �ۺ�
public:
	//Vector2 m_bgSize= Vector2(500,500);
	//Vector2 m_exitSize = Vector2(830, 690);
	//Vector2 m_creditSize = Vector2(1500, 840); -> �� �׳� ���̴��� ����
	bool m_isOnExit = false;
	bool m_isOnCredit = false;
	float m_fade = 1.f;
	float m_time = 0.f;
	bool m_isGo = false;
};

