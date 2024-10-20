#pragma once
#include "Script.h"

class PlayerMoveScript;
class TextRender;

/// <summary>
/// 손서희
/// 
/// 플레이어의 자식으로 넣어버리자
/// </summary>
class ScoreUI : public Script
{
	enum EffectState
	{
		eMoveLeft,
		eMoveUp,
		eScoreUp
	};

	struct scoreEffect
	{
		EffectState state;
		int score;
		int lastScore;
		float lastTime;
		bool isDone;
		Vector2 pos;
		float alpha = 1;
	};
	
	
	// Reflection
	GENERATE_CLASS_TYPE_INFO(ScoreUI)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScoreUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//생성자와 소멸자 ---------------------------------------------------------------------------------------
public:
	ScoreUI();
	~ScoreUI();

// 컴포넌트 함수 ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// 멤버 함수 ----------------------------------------------------------------------------------------
	void TransferInfo(float hunger, int score) { m_hunger = hunger, m_realScore = score; };
	void ScoreIncrease(int scoreIncrese);

private:
	void ProcessQueue(scoreEffect& effect, double dt);

// 멤버 변수 ----------------------------------------------------------------------------------------
private:
	std::weak_ptr<IGraphics> m_graphics;


private:
	float m_hunger = 100; 
	// 0~13  : 1.0 배고파!
	// 14~29 : 1.5 많이 부족해
	// 30~48 : 2.0 아직 부족해
	// 49~69 : 2.5 조금 부족해
	// 70~100: 3.0 배부르다~

	int m_realScore = 0;
	int m_lerpedScore = 0;
	
	std::vector<scoreEffect> m_effectQueue;

	const float m_leftTime = 0.2f;
	const float m_upTime = 0.2f;
	const float m_increaseTime = 1.f;

	Vector2 m_textEffectPos1 = Vector2(200, 50);
	Vector2 m_textEffectPos2 = Vector2(70, 50);
	Vector2 m_textEffectPos3 = Vector2(70, -10);

	int scaleSize = 36;
	Vector2 scalePos = Vector2(-69, -9);
	int textSize = 15;
	Vector2 textPos = Vector2(-50, -25); // 배고파! 기준

	int totalSize = 15;
	Vector2 totalPos = Vector2(98, -24.5);
	int scoreSize = 28;
	Vector2 scorePos = Vector2(67, -3.7); //4자리수 기준

	Vector4 scaleColor = Vector4(253 / 255.f, 231 / 255.f, 207 / 255.f, 1);

public:
	Vector2 m_basePos = Vector2(1750, 365);
	Vector2 m_imgSize = Vector2(340, 112);
};

