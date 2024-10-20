#pragma once
#include "ISingleton.h"

/// <summary>
/// 키입력을 관리할 매니저 클래스 - 싱글톤
/// 특정 키 설정 등등 - 어떤 무기를 어떤 키에 설정하면 해당 키에 그것이 셋팅되어야한다
/// 
/// 한프레임 안에 동일한 키 입력에 동일한 상태를 유지해야한다 - 프레임 동기화
/// 
/// 작성자 유승운
/// 수정 - 손서희 : 마우스 관련 기능 추가
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

	KEYSTATE m_keyState[0xFE/*VK_OEM_CLEAR*/ + 1]; //https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes 각각의 모든 키에 대한 상태 배열

	POINT m_mousePos;
	POINT m_prevMousePos;
	Vector2 m_mousedxdy;

	WheelState m_wheelState = WheelState::None;

	HWND m_hwnd;

	//static InputManager* m_inputManager;
};

