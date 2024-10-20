#pragma once
#include <Script.h>

class GameManager;
class DirectionalLight;

/// <summary>
/// 시간에 따라 변하는 태양
/// 손서희
/// </summary>
class SunLight : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(SunLight)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SunLight, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//생성자와 소멸자 ---------------------------------------------------------------------------------------
public:
	SunLight();
	~SunLight();


// 컴포넌트 함수 ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual std::shared_ptr<Component> Clone() const override;

// 멤버 함수 ----------------------------------------------------------------------------------------
	

// 멤버 변수 ----------------------------------------------------------------------------------------
private:
	double m_timer = 0.f;   // 타이머 -> GameManager한테 Update마다 받아오기
	double m_endTime = 0.f; // 전체 시간 흐름 -> GameManager한테 받아오기
	std::shared_ptr<GameManager> m_gameMgr;
	std::shared_ptr<DirectionalLight> m_dir;

	// 아침
	float m_morningTime = 0.f; // endTime을 받아와서 곱해서 저장
	Vector4 m_morningAmb = Vector4(172,215,250,180);
	Vector3 m_morningColor = Vector3(219,228,240);
	float m_morningIntensity = 0.1f;

	// 낮
	float m_noonTime = 0.f;
	Vector4 m_noonAmb = Vector4(255, 255, 255, 247);
	Vector3 m_noonColor = Vector3(255, 255, 255);
	float m_noonIntensity = 0.14f;

	// 저녁
	float m_evenStartTime = 0.f;
	float m_evenTime = 0.f;
	Vector4 m_evenAmb = Vector4(230, 230, 255, 235);
	Vector3 m_evenColor = Vector3(200, 41, 150);
	float m_evenIntensity = 0.07f;

	// 밤
	float m_nightTime = 0.f;
	Vector4 m_nightAmb = Vector4(81, 78, 136, 235);
	Vector3 m_nightColor = Vector3(145, 143, 192);;
	float m_nightIntensity = 0.03f;

	// 밤~
	Vector4 m_midNightAmb = Vector4(156, 199, 255, 120);

	Vector3 m_startDir = Vector3(-3.8f, -8.1f, 4.8f);
	Vector3 m_finalDir = Vector3(6.5f, -8.1f, 4.8f);
};