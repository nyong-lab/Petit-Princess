#pragma once
#include <Script.h>
#include <random>

class PointLight;

/// <summary>
/// 횃불처럼 일렁이는 포인트라이트
/// 손서희
/// </summary>
class TorchLight : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(TorchLight)
		PROPERTY(m_isLightOn)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TorchLight, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//생성자와 소멸자 ---------------------------------------------------------------------------------------
public:
	TorchLight();
	~TorchLight();

// 컴포넌트 함수 ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual std::shared_ptr<Component> Clone() const override;

// 멤버 함수 ----------------------------------------------------------------------------------------
	

// 멤버 변수 ----------------------------------------------------------------------------------------
private:
	float m_timer = 0.f;
	std::shared_ptr<PointLight> m_point;

	float m_minLuma = 3.2f; // 최소 밝기
	float m_maxLuma = 3.8f; // 최대 밝기
	Vector3 m_color = Vector3(255, 143, 54);

	float m_onTime;			// 불이 켜지기 시작하는 시간
	bool m_isLightOn = false;  // 처음부터 켜저 있는 조명인지
	bool m_isWorkingDone = false;

	//깜빡임 관련
	float m_changeTime;
	int m_isGoingLighter;

	// 랜덤
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> changeDist;
	std::uniform_real_distribution<float> onDist;
};