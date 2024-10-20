#pragma once

class Editor;
class GameEngine;
class Timer;
class IGraphics;
class PetitePrincess;

class DebugProcess
{
public:
	DebugProcess(HINSTANCE hInstance, int CmdShow);
	~DebugProcess();

	bool Initialize();
	int Loop();
	void Finalize();

private:
	void Update();
	void Render();

	void RegisterClass(HINSTANCE hInstance);
	bool InitInstance();

private:
	HINSTANCE m_hInstance;
	int m_cmdShow;
	WNDCLASSEXW m_wcex;
	HWND m_hWnd;

	std::unique_ptr<GameEngine> m_gameEngine;
	std::shared_ptr<IGraphics> m_graphics;
	std::unique_ptr<Editor> m_editor;
	std::shared_ptr<Timer> m_timer;
	std::unique_ptr<PetitePrincess> m_petitePrincess;

	static int m_width;  // = 1920;
	static int m_height; // = 1080;
	static bool m_isResized;

public:
	static LRESULT CALLBACK DebugWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};





