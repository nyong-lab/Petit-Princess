#include "GameDemo_pch.h"
#include "framework.h"
#include "GameProcess.h"

#include "GameEngine.h"
#include "../GameEngine/Timer.h"
#include "../GameEngine/CameraManager.h"

#include "imgui.h"
#include "Editor.h"

#include "IGraphics.h"

#include "PetitePrincess.h"

#include "../GameEngine/InputManager.h"

#include "resource.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING] = L"Demo";
WCHAR szWindowClass[MAX_LOADSTRING] = L"Demo";

int GameProcess::m_width = 1920;
int GameProcess::m_height = 1080;
bool GameProcess::m_isResized = false;


GameEngine* ginstance;


GameProcess::GameProcess(HINSTANCE hInstance, int CmdShow) : m_hInstance(hInstance), m_cmdShow(CmdShow), m_hWnd(nullptr), m_wcex(WNDCLASSEXW())
{

}

GameProcess::~GameProcess()
{

}

bool GameProcess::Initialize()
{
	RegisterClass(m_hInstance);
	if (!InitInstance())
	{
		return FALSE;
	}


	/// 그래픽스 초기화
	m_graphics = DXExport::GetInstance();
	m_graphics->Initialize(m_hWnd);

	/// 게임엔진 초기화
	m_gameEngine = std::make_unique<GameEngine>();
	m_gameEngine->Initialize(m_hWnd, m_graphics);
	ginstance = m_gameEngine.get();

	/// 타이머 초기화
	m_timer = std::make_shared<Timer>();
	m_timer->Initialize();

	/// 쁘띠 프린세스 초기화 (게임에 담길 씬들)
	m_petitePrincess = std::make_unique<PetitePrincess>();
	m_petitePrincess->Initialize(m_gameEngine, m_graphics);

	/// 에디터 초기화 (씬 등록 하고 에디터 초기화 해야 함)
	m_editor = std::make_unique<Editor>();
	m_editor->Initialize(m_hWnd,
		m_graphics,
		m_gameEngine->GetSceneManager(),
		m_gameEngine->GetCameraManager(),
		m_gameEngine->GetResourceManager(), m_timer);

	/// 게임엔진 스타트
	m_gameEngine->Start();

	return true;
}

int GameProcess::Loop()
{
	MSG msg = MSG();

	while (true)
	{
		m_timer->Update();
		wchar_t sz[255]{};
		swprintf_s(sz, L"FPS : %d", static_cast<int> (m_timer->FPS()));
		SetWindowText(m_hWnd, sz);

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			DestroyWindow(m_hWnd);
			break;
		}

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Update(m_timer->DeltaTime());
		Render(m_timer->DeltaTime());
	}

	return (int)msg.wParam;
}

void GameProcess::Finalize()
{
	m_gameEngine->Finalize();
}

void GameProcess::Update(double dt)
{
	if (m_isResized)
	{
		m_gameEngine->Resize(m_width, m_height);
		m_graphics->Resize(m_width, m_height);

		m_isResized = false;
	}

	m_editor->NewFrame();
	m_editor->Update(dt);
	m_graphics->Update(dt);
	if (!m_timer->IsPause())
	{
		m_gameEngine->Update(dt);
	}

}

void GameProcess::Render(double dt)
{
	m_graphics->BeginDraw();

	m_gameEngine->Render(dt);

	m_graphics->DefferedDrawTest();

	m_editor->Render();

	m_graphics->EndDraw();
}

void GameProcess::RegisterClass(HINSTANCE hInstance)
{
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.lpfnWndProc = WndProc;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hInstance = hInstance;
	m_wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wcex.lpszMenuName = NULL;
	m_wcex.lpszClassName = szWindowClass;
	m_wcex.hIconSm = LoadIcon(m_wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	RegisterClassExW(&m_wcex);
}

bool GameProcess::InitInstance()
{
	m_hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
		CW_USEDEFAULT, 0, width, hegiht, nullptr, nullptr, m_hInstance, nullptr);

	RECT rc = { 0,0,width, hegiht };
	AdjustWindowRectEx(&rc, WS_POPUP, FALSE, 0);

	// 창의 크기 설정
	SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, m_cmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CALLBACK GameProcess::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
	{
		int width = (int)LOWORD(lParam);
		int height = (int)HIWORD(lParam);

		m_width = width;
		m_height = height;
		m_isResized = true;

		break;
	}
	case WM_MOUSEWHEEL:
	{
		InputManager::GetInstance()->ProcessMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}