#pragma once
#include "Script.h"

/// <summary>
/// 손서희
/// 
/// 클릭 이벤트는 여기서 다 받고, 자식
/// </summary>
class MainUI : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MainUI)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MainUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//생성자와 소멸자 ---------------------------------------------------------------------------------------
public:
	MainUI();
	~MainUI();

// 컴포넌트 함수 ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// 멤버 함수 ----------------------------------------------------------------------------------------
	float GetFade() { return m_fade; }
	void FaidOut(double dt);
// 멤버 변수 ----------------------------------------------------------------------------------------
private:
	std::weak_ptr<IGraphics> m_igraphics;


	// ~~~Render~~~ 아몰랑 귀찮으니 퍼블릭
public:
	//Vector2 m_bgSize= Vector2(500,500);
	//Vector2 m_exitSize = Vector2(830, 690);
	//Vector2 m_creditSize = Vector2(1500, 840); -> 넌 그냥 쉐이더로 가자
	bool m_isOnExit = false;
	bool m_isOnCredit = false;
	float m_fade = 1.f;
	float m_time = 0.f;
	bool m_isGo = false;
};

