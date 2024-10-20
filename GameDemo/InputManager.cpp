#include "GameEngine_pch.h"
#include "InputManager.h"

void InputManager::Initialize(HWND hwnd)
{
	for (int i = 0; i < 255; i++)
	{
		m_keyState[i] = KEYSTATE::None;
	}

	//마우스 초기화
	GetCursorPos(&m_prevMousePos);
	ScreenToClient(hwnd, &m_prevMousePos);
}

void InputManager::Update()
{
	//키스테이스 배열 크기
	//0xFE + 1 = 255

	if (GetFocus())
	{
		for (int i = 0; i < 255; i++)
		{
			SHORT keyState = GetAsyncKeyState(i);

			// 마지막 비트가 1이면 키가 현재 눌린 상태임
			bool isKeyDown = keyState & 0x8001;

			//안눌렸다 -> 눌렸다
			if ((m_keyState[i] == KEYSTATE::None || m_keyState[i] == KEYSTATE::Up) && isKeyDown)
			{
				m_keyState[i] = KEYSTATE::Down;
			}
			//눌렸다 -> 눌렸다
			else if ((m_keyState[i] == KEYSTATE::Down) && isKeyDown)
			{
				m_keyState[i] = KEYSTATE::Press;
			}
			//눌렸다 -> 안눌렸다
			else if ((m_keyState[i] == KEYSTATE::Down || m_keyState[i] == KEYSTATE::Press) && !isKeyDown)
			{
				m_keyState[i] = KEYSTATE::Up;
			}
			//안눌렸다 -> 안눌렸다
			else if (m_keyState[i] == KEYSTATE::Up && !isKeyDown)
			{
				m_keyState[i] = KEYSTATE::None;
			}
		}

		//마우스 이동 변화량 계산
		GetCursorPos(&m_mousePos);
		m_mousedxdy = Vector2(float(m_mousePos.x - m_prevMousePos.x),
			                  float(m_mousePos.y - m_prevMousePos.y));

		m_prevMousePos = m_mousePos;
	}

	// ImGui 텍스트 입력 중에는 키 입력을 받지 않기위함
	// 더 좋은 방법은?
	if (ImGui::IsAnyItemActive())
	{
		for (int i = 0; i < 255; ++i)
		{
			m_keyState[i] = KEYSTATE::None;
		}
	}
}

void InputManager::Finalize()
{
}

InputManager::InputManager()
{

}
