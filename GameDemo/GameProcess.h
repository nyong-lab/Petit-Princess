#pragma once
	
class Editor;
class GameEngine;
class Timer;
class IGraphics;
class PetitePrincess;

class GameProcess
{
public:
	GameProcess(HINSTANCE hInstance, int CmdShow);
	~GameProcess();

	bool Initialize();
	int Loop();
	void Finalize();

private:
	void Update(double dt);
	void Render(double dt);

	void RegisterClass(HINSTANCE hInstance);
	bool InitInstance();

private:
	HINSTANCE m_hInstance;
	int m_cmdShow;
	WNDCLASSEXW m_wcex;
	HWND m_hWnd;

	std::unique_ptr<GameEngine> m_gameEngine;
	std::shared_ptr<IGraphics> m_graphics;
	std::shared_ptr<Timer> m_timer;
	std::unique_ptr<PetitePrincess> m_petitePrincess;

	std::unique_ptr<Editor> m_editor;

	const LONG width = 1920;
	const LONG hegiht = 1080;

	static int m_width;  // = 1920;
	static int m_height; // = 1080;
	static bool m_isResized;

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};




