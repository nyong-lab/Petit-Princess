#include "GameDemo_pch.h"
#include "MainUI.h"
#include "InputManager.h"
#include "Entity.h"
#include "IGraphics.h"
#include "EventManager.h"
#include "DongilScene.h"
#include "SceneManager.h"
#include "Entity.h"
#include "SoundManager.h"

MainUI::MainUI()
{

}

MainUI::~MainUI()
{

}

void MainUI::Start()
{

}

void MainUI::Update(double dt)
{
	Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();
	
	m_isOnExit;
	m_isOnCredit;
	// 페이드 아웃
	if(m_isGo)
		FaidOut(dt);
	// 아무것도 열려있지 않은 상태
	if (!m_isOnExit && !m_isOnCredit
		&& InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
	{
		// 훔치기 영역
		if (330.f <= mousePos.x && mousePos.x <= 610.f
			&& 540.f <= mousePos.y && mousePos.y <= 610.f)
		{
			SoundManager::GetInstance()->PlaySFX("Click.mp3");
			//m_fade = 0.0;
			m_isGo = true;
			/// 기존
			//EventManager::GetInstance()->SceneChange(m_owner.lock()->GetSceneManager().lock()->GetScene("STAGE1"));

			/// 경로상에 위치한 스테이지 호출 (로딩 시간이 좀 있음..)
			//m_owner.lock()->GetSceneManager().lock()->LoadScene(L"RealStage");

			/// 최종?
			//std::shared_ptr<Scene> scene = m_owner.lock()->GetSceneManager().lock()->GetScene("RealStage");
			//EventManager::GetInstance()->SceneChange(scene);
			////m_owner.lock()->GetSceneManager().lock()->MosterPointerScene(scene);
			//EventManager::GetInstance()->StorePointerConnection(scene);
		}

		// 만든이 영역
		if (330.f <= mousePos.x && mousePos.x <= 610.f
			&& 700.f <= mousePos.y && mousePos.y <= 760.f)
		{
			SoundManager::GetInstance()->PlaySFX("Click.mp3");
			m_isOnCredit = true;
		}

		// 안훔치기 영역
		if (330.f <= mousePos.x && mousePos.x <= 610.f
			&& 840.f <= mousePos.y && mousePos.y <= 910.f)
		{
			SoundManager::GetInstance()->PlaySFX("Click.mp3");
			m_isOnExit = true;
		}
	}

	else if (m_isOnExit)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			m_isOnExit = false;
			SoundManager::GetInstance()->PlaySFX("Click.mp3");
		}

		else if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
		{
			// 종료
			if (650.f <= mousePos.x && mousePos.x <= 940.f
				&& 670.f <= mousePos.y && mousePos.y <= 740.f)
				ExitProcess(0);

			// 돌아가기
			if (970.f <= mousePos.x && mousePos.x <= 1260.f
				&& 670.f <= mousePos.y && mousePos.y <= 740.f)
			{
				SoundManager::GetInstance()->PlaySFX("Click.mp3");
				m_isOnExit = false;
			}
		}
	}

	else if (m_isOnCredit)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			m_isOnCredit = false;
			SoundManager::GetInstance()->PlaySFX("Click.mp3");
		}

		else if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
		{
			// 돌아가기
			if (1260.f <= mousePos.x && mousePos.x <= 1350.f
				&& 265.f <= mousePos.y && mousePos.y <= 330.f)
			{
				SoundManager::GetInstance()->PlaySFX("Click.mp3");
				m_isOnCredit = false;
			}
		}
	}
}

void MainUI::Render(double dt)
{
	auto g = m_owner.lock()->GetGraphics();
	
// 	Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();
// 	std::wstring strr = L"x : " + std::to_wstring(mousePos.x) + L" y : " + std::to_wstring(mousePos.y);
// 	g.lock()->DrawChar(strr, mousePos);
}

std::shared_ptr<Component> MainUI::Clone() const
{
	auto clone = std::make_shared<MainUI>();
	return clone;
}

void MainUI::FaidOut(double dt)
{
	m_time += dt;
	m_fade -= dt;
	if (m_time > 1.0f)
	{
		EventManager::GetInstance()->SceneChange(m_owner.lock()->GetSceneManager().lock()->GetScene("STAGE1"));
		m_fade = 1.f;
		m_time = 0.f;
	}
}

