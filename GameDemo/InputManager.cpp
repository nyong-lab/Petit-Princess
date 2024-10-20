#include "GameEngine_pch.h"
#include "InputManager.h"

void InputManager::Initialize(HWND hwnd)
{
	for (int i = 0; i < 255; i++)
	{
		m_keyState[i] = KEYSTATE::None;
	}

	//���콺 �ʱ�ȭ
	GetCursorPos(&m_prevMousePos);
	ScreenToClient(hwnd, &m_prevMousePos);
}

void InputManager::Update()
{
	//Ű�����̽� �迭 ũ��
	//0xFE + 1 = 255

	if (GetFocus())
	{
		for (int i = 0; i < 255; i++)
		{
			SHORT keyState = GetAsyncKeyState(i);

			// ������ ��Ʈ�� 1�̸� Ű�� ���� ���� ������
			bool isKeyDown = keyState & 0x8001;

			//�ȴ��ȴ� -> ���ȴ�
			if ((m_keyState[i] == KEYSTATE::None || m_keyState[i] == KEYSTATE::Up) && isKeyDown)
			{
				m_keyState[i] = KEYSTATE::Down;
			}
			//���ȴ� -> ���ȴ�
			else if ((m_keyState[i] == KEYSTATE::Down) && isKeyDown)
			{
				m_keyState[i] = KEYSTATE::Press;
			}
			//���ȴ� -> �ȴ��ȴ�
			else if ((m_keyState[i] == KEYSTATE::Down || m_keyState[i] == KEYSTATE::Press) && !isKeyDown)
			{
				m_keyState[i] = KEYSTATE::Up;
			}
			//�ȴ��ȴ� -> �ȴ��ȴ�
			else if (m_keyState[i] == KEYSTATE::Up && !isKeyDown)
			{
				m_keyState[i] = KEYSTATE::None;
			}
		}

		//���콺 �̵� ��ȭ�� ���
		GetCursorPos(&m_mousePos);
		m_mousedxdy = Vector2(float(m_mousePos.x - m_prevMousePos.x),
			                  float(m_mousePos.y - m_prevMousePos.y));

		m_prevMousePos = m_mousePos;
	}

	// ImGui �ؽ�Ʈ �Է� �߿��� Ű �Է��� ���� �ʱ�����
	// �� ���� �����?
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
