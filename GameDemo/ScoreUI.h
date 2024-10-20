#pragma once
#include "Script.h"

class PlayerMoveScript;
class TextRender;

/// <summary>
/// �ռ���
/// 
/// �÷��̾��� �ڽ����� �־������
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

//�����ڿ� �Ҹ��� ---------------------------------------------------------------------------------------
public:
	ScoreUI();
	~ScoreUI();

// ������Ʈ �Լ� ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// ��� �Լ� ----------------------------------------------------------------------------------------
	void TransferInfo(float hunger, int score) { m_hunger = hunger, m_realScore = score; };
	void ScoreIncrease(int scoreIncrese);

private:
	void ProcessQueue(scoreEffect& effect, double dt);

// ��� ���� ----------------------------------------------------------------------------------------
private:
	std::weak_ptr<IGraphics> m_graphics;


private:
	float m_hunger = 100; 
	// 0~13  : 1.0 �����!
	// 14~29 : 1.5 ���� ������
	// 30~48 : 2.0 ���� ������
	// 49~69 : 2.5 ���� ������
	// 70~100: 3.0 ��θ���~

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
	Vector2 textPos = Vector2(-50, -25); // �����! ����

	int totalSize = 15;
	Vector2 totalPos = Vector2(98, -24.5);
	int scoreSize = 28;
	Vector2 scorePos = Vector2(67, -3.7); //4�ڸ��� ����

	Vector4 scaleColor = Vector4(253 / 255.f, 231 / 255.f, 207 / 255.f, 1);

public:
	Vector2 m_basePos = Vector2(1750, 365);
	Vector2 m_imgSize = Vector2(340, 112);
};

