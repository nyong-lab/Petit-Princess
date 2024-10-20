#include "GameDemo_pch.h"
#include "TimerUI.h"
#include "Entity.h"
#include "MathUtils.h"
#include "IGraphics.h"
#include "GameManager.h"
#include "TextRender.h"

TimerUI::TimerUI()
{

}

TimerUI::~TimerUI()
{

}

void TimerUI::Start()
{
	m_mgr = m_owner.lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner().lock()->GetComponent<GameManager>();
	m_textRender = m_owner.lock()->GetComponent<TextRender>();
	m_igraphics = m_owner.lock()->GetGraphics();
	m_endTime = (float)m_mgr->GetGameTime();

	m_textRender->m_leftTop = m_position + Vector2(-38, 53.f);
	m_textRender->m_color = Vector4(1, 1, 1, 1);
	m_textRender->m_fontSize = 25;
}

void TimerUI::Update(double dt)
{
	m_timer = (float)m_mgr->GetCurrentGameTime();
	m_curAngle = MathUtils::Lerp(m_startAngle, m_endAngle, m_timer / m_endTime);

	// 남은시간 표시
	int last = static_cast<int>(m_endTime - m_timer);
	int min = last / 60;
	int sec = last % 60;

	std::wstring minText = std::to_wstring(min); if (minText.length() == 1) minText = L"0" + minText;
	std::wstring secText = std::to_wstring(sec); if (secText.length() == 1) secText = L"0" + secText;
	m_textRender->m_text = minText + L" : " + secText;
}

void TimerUI::Render(double dt)
{

}

std::shared_ptr<Component> TimerUI::Clone() const
{
	auto clone = std::make_shared<TimerUI>();
	return clone;
}
