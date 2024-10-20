#pragma once
#include "ISingleton.h"

/// <summary>
/// Ű�Է��� ������ �Ŵ��� Ŭ���� - �̱���
/// Ư�� Ű ���� ��� - � ���⸦ � Ű�� �����ϸ� �ش� Ű�� �װ��� ���õǾ���Ѵ�
/// 
/// �������� �ȿ� ������ Ű �Է¿� ������ ���¸� �����ؾ��Ѵ� - ������ ����ȭ
/// 
/// �ۼ��� ���¿�
/// ���� - �ռ��� : ���콺 ���� ��� �߰�
/// </summary>

enum class KEYSTATE
{
	None,
	Down,
	Up,
	Press
};

enum class WheelState
{
	None,
	Down,
	Up
};

class InputManager : public ISingleton<InputManager>
{
	friend class ISingleton<InputManager>;
public:
	
	//static InputManager* GetInstance();

	void Initialize(HWND hwnd);
	void Update();
	void Finalize();

	KEYSTATE GetKeyState(int key) { return m_keyState[key]; }
	bool IsKeyDown(int key) {return m_keyState[key] == KEYSTATE::Down;}
	bool IsKeyNone(int key) {return m_keyState[key] == KEYSTATE::None;}
	bool IsKeyUp(int key) {return m_keyState[key] == KEYSTATE::Up;}
	bool IsKeyPress(int key) {return m_keyState[key] == KEYSTATE::Press;}

	Vector2 GetMousePosition() { return Vector2((float)m_mousePos.x, (float)m_mousePos.y); }
	Vector2 GetMouseDxdy() { return m_mousedxdy; }

	WheelState GetWheelState() { return m_wheelState; }
	void ProcessMouseWheel(int value);

private:
	InputManager();

	KEYSTATE m_keyState[0xFE/*VK_OEM_CLEAR*/ + 1]; //https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes ������ ��� Ű�� ���� ���� �迭

	POINT m_mousePos;
	POINT m_prevMousePos;
	Vector2 m_mousedxdy;

	WheelState m_wheelState = WheelState::None;

	HWND m_hwnd;

	//static InputManager* m_inputManager;
};

