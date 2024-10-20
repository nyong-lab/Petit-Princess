#include "GameDemo_pch.h"
#include "ScoreUI.h"
#include "Entity.h"
#include "IGraphics.h"
#include "MathUtils.h"

ScoreUI::ScoreUI()
{

}

ScoreUI::~ScoreUI()
{

}

void ScoreUI::Start()
{
	m_graphics = m_owner.lock()->GetGraphics();
}

void ScoreUI::Update(double dt)
{

}

void ScoreUI::Render(double dt)
{
	// 큐에 들어있는 작업들을 수행하면서 텍스트 드로우
	for (auto& q : m_effectQueue)
	{
		ProcessQueue(q, dt);
	}

	// 작업이 완료된건 삭제처리
	std::erase_if(m_effectQueue, [](const scoreEffect& effect) { return effect.isDone; });

	// 배율 표시
	int ratio;
	if (m_hunger > 0.70f)
	{
		ratio = 3.0f;
		m_graphics.lock()->DrawChar(L"X 3.0", m_basePos + scalePos, scaleSize, scaleColor);
		m_graphics.lock()->DrawChar(L"배부르다~", m_basePos + textPos, textSize, Vector4(0, 1, 0, 1));
	}
	else if (m_hunger > 0.49f)
	{
		ratio = 2.5f;
		m_graphics.lock()->DrawChar(L"X 2.5", m_basePos + scalePos, scaleSize, scaleColor);
		m_graphics.lock()->DrawChar(L"조금 부족해", m_basePos + textPos, textSize, Vector4(0, 1, 0, 1));
	}
	else if (m_hunger > 0.30f)
	{
		ratio = 2.0f;
		m_graphics.lock()->DrawChar(L"X 2.0", m_basePos + scalePos, scaleSize, scaleColor);
		m_graphics.lock()->DrawChar(L"아직 부족해", m_basePos + textPos, textSize, Vector4(1, 0, 0, 1));
	}
	else if (m_hunger > 0.14)
	{
		ratio = 1.5f;
		m_graphics.lock()->DrawChar(L"X 1.5", m_basePos + scalePos, scaleSize, scaleColor);
		m_graphics.lock()->DrawChar(L"많이 부족해", m_basePos + textPos, textSize, Vector4(1, 0, 0, 1));
	}
	else
	{
		ratio = 1.0f;
		m_graphics.lock()->DrawChar(L"X 1.0", m_basePos + scalePos, scaleSize, Vector4(1, 1, 1, 1));
		m_graphics.lock()->DrawChar(L"배고파!", m_basePos + textPos, textSize, Vector4(1, 0, 0, 1));
	}

	// 점수 표시
	int score = m_lerpedScore * ratio;
	std::wstring str = std::to_wstring(score);
	std::wstring pretty;
	if (str.length() == 4) 
	{
		pretty = str.substr(0, 1) + L',' + str.substr(1);
		m_graphics.lock()->DrawChar(pretty, m_basePos + scorePos, scoreSize, Vector4(1, 1, 1, 1));
	}
	else if (str.length() == 5)
	{
		pretty = str.substr(0, 2) + L',' + str.substr(2);
		m_graphics.lock()->DrawChar(pretty, m_basePos + scorePos + Vector2(-14,0), scoreSize, Vector4(1, 1, 1, 1));
	}
	else if (str.length() == 6)
	{
		pretty = str.substr(0, 3) + L',' + str.substr(3);
		m_graphics.lock()->DrawChar(pretty, m_basePos + scorePos + Vector2(-27, 0), scoreSize, Vector4(1, 1, 1, 1));
	}

	m_graphics.lock()->DrawChar(L"TOTAL", m_basePos + totalPos, totalSize, Vector4(1, 1, 1, 1));
	
}

void ScoreUI::ScoreIncrease(int scoreIncrese)
{
	scoreEffect effect;
	effect.lastTime = m_leftTime;
	effect.score = scoreIncrese;
	effect.lastScore = scoreIncrese;
	effect.state = EffectState::eMoveLeft;
	effect.isDone = false;
	effect.pos = m_textEffectPos1 + m_basePos;

	m_effectQueue.push_back(effect);
}

void ScoreUI::ProcessQueue(scoreEffect& effect, double dt)
{
	std::wstring str = std::to_wstring(effect.score);

	switch (effect.state)
	{
	case ScoreUI::eMoveLeft: 
	{
		Vector2 pos = MathUtils::Lerp(effect.pos, m_basePos + m_textEffectPos2, dt * 17);
		effect.pos = pos;
		m_graphics.lock()->DrawChar(str, pos, 28, Vector4(1, 1, 1, 1));

		effect.lastTime -= dt;
		if (effect.lastTime < 0)
		{
			effect.lastTime = m_upTime;
			effect.state = eMoveUp;
			effect.pos = m_basePos + m_textEffectPos2;
		}
		break;
	}


	case ScoreUI::eMoveUp: 
	{
		Vector2 pos = MathUtils::Lerp(effect.pos, m_basePos + m_textEffectPos3, dt * 6);
		float alpha = MathUtils::Lerp(effect.alpha, 0.f, dt * 6);
		effect.alpha = alpha;
		effect.pos = pos;
		m_graphics.lock()->DrawChar(str, pos, 28, Vector4(1, 1, 1, alpha));

		effect.lastTime -= dt;
		if (effect.lastTime < 0)
		{
			effect.lastTime = m_increaseTime;
			effect.state = eScoreUp;
			effect.pos = m_basePos + m_textEffectPos3;
		}
		break;
	}

	case ScoreUI::eScoreUp:

		int var = effect.score * dt;

		if ((effect.lastScore - var) <= 0)
		{
			effect.isDone = true;
			m_lerpedScore += effect.lastScore;
		}
		else
		{
			effect.lastScore -= var;
			m_lerpedScore += var;
		}
		break;
	}
}

std::shared_ptr<Component> ScoreUI::Clone() const
{
	auto clone = std::make_shared<ScoreUI>();
	return clone;
}
