#pragma once
#include "Script.h"

class FoodTableScript;

/// <summary>
/// 손서희
/// 
/// 상인의 자식으로 넣어야 한다.
/// </summary>
class MerchantUI : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MerchantUI)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MerchantUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//생성자와 소멸자 ---------------------------------------------------------------------------------------
public:
	MerchantUI();
	~MerchantUI();

// 컴포넌트 함수 ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// 멤버 함수 ----------------------------------------------------------------------------------------


// 멤버 변수 ----------------------------------------------------------------------------------------
public:
	int m_state;	 // 무엇을 띄울지? 
	// 0 : 아무것도 띄우지 않겠다 
	// 1 : 맛집 준비 
	// 2 : 다먹은 맛집
	// 3 : 재방문 쿨타임
	// 4 : 눈치게이지
	// 5 : 적발 

	int m_foodtype;  // 맛집 종류
	// 0 : 고기
	// 1 : 사과(과일)
	// 2 : 바게트(빵)
	// 3 : 게(해산물)
	// 4 : 컵케익(디저트)

	float m_ratio;     // 눈치게이지 or 재방문 게이지

	Vector2 m_pos;
	const Vector2 m_size = Vector2(90, 90);

private:
	std::shared_ptr<FoodTableScript> m_foodTable;

};

