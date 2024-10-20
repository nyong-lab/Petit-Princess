#include "GameDemo_pch.h"
#include "PopUpUI.h"
#include "Entity.h"
#include "InputManager.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "EventManager.h"

PopUpUI::PopUpUI()
{

}

PopUpUI::~PopUpUI()
{

}

void PopUpUI::Start()
{

}

void PopUpUI::Update(double dt)
{
	if (InputManager::GetInstance()->IsKeyDown(VK_ESCAPE))
		m_isOn = !m_isOn;

	m_owner.lock()->SetRender(m_isOn);

	if (m_isOn && InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
	{ 
		Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();
		
		// 돌아가기
		if (330.f <= mousePos.x && mousePos.x <= 610.f
			&& 540.f <= mousePos.y && mousePos.y <= 610.f)
		//m_isOn = false;
		{ }
		// 메인화면으로 가기	
		if (330.f <= mousePos.x && mousePos.x <= 610.f
			&& 700.f <= mousePos.y && mousePos.y <= 760.f)
		{ }
		//EventManager::GetInstance()->SceneChange(
		//	m_owner.lock()->GetSceneManager().lock()->GetScene("Title"));
	}
}

void PopUpUI::Render(double dt)
{

}

std::shared_ptr<Component> PopUpUI::Clone() const
{
	auto clone = std::make_shared<PopUpUI>();
	return clone;
}
