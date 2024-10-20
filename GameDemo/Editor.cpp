#include "GameDemo_pch.h"
#include "Editor.h"

#include "IGraphics.h"
#include "Device.h"

#include "Scene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "CameraModule.h"

#include "DockingGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "ProjectGui.h"
#include "WorldGui.h"
#include "SeveralGui.h"

#include <locale>

#include "Timer.h"

Editor::Editor()
{

}

Editor::~Editor()
{

}

void Editor::Initialize(HWND hWnd,
	std::weak_ptr<IGraphics> graphics, 
	std::weak_ptr<SceneManager> sceneManager,
	std::weak_ptr<CameraManager> cameraManager,
	std::weak_ptr<ResourceManager> gameResourceManager,
	std::weak_ptr<Timer> timer)
{
	// ImGui Initialize
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	/// 기본 테마 (다크)
	//ImGui::StyleColorsDark();

	/// 깃허브 모던 테마
	//Theme_MoonLight();

	/// 깃허브 캔디 테마? (종류 여러개)
	//Theme_Blender();
	//Theme_Cyberpunk();
	Theme_Nord();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.f;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	/// c++20 u8과 ImGui 호환 안되는 듯
	/// ImGui::Text 한글 출력 안된다.
	// 폰트 추가
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
	//io.Fonts->AddFontFromFileTTF("../Resource/Font/malgun.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());

	auto deviceManager = std::any_cast<std::shared_ptr<Device>>(graphics.lock()->GetDeviceManager());

	//Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(deviceManager->GetDevice().Get(), deviceManager->GetDeviceContext().Get());

	// MapEditor Initialize
	m_dockingGui = std::make_unique<DockingGui>();
	m_worldGui = std::make_shared<WorldGui>();
	m_hierarchyGui = std::make_unique<HierarchyGui>();
	m_inspectorGui = std::make_shared<InspectorGui>();
	m_projectGui = std::make_shared<ProjectGui>();
	m_severalGui = std::make_unique<SeveralGui>();

	m_graphics = graphics;
	m_sceneManager = sceneManager;
	m_cameraManager = cameraManager;
	m_gameResourceManager = gameResourceManager;
	m_timer = timer;

	m_worldGui->Initialize(m_projectGui, graphics, sceneManager, cameraManager, gameResourceManager);
	m_hierarchyGui->Initialize(graphics, sceneManager, cameraManager, gameResourceManager);
	m_inspectorGui->Initialize(graphics, sceneManager, cameraManager, gameResourceManager);
	m_severalGui->Initialize(cameraManager, sceneManager, timer);
	m_projectGui->Initialize(m_worldGui, m_inspectorGui, graphics, sceneManager, cameraManager, gameResourceManager);
}

void Editor::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Editor::Update(double dt)
{	
	/// 도킹 시스템 On
	m_dockingGui->Update();

	/// ImGui 자체 제공하는 여러 기능이 담겨있는 창 
	//ImGui::ShowDemoWindow();

	/// 월드 창 : 데이터 저장 및 로드, 창 활성화 여부 (구현 중)
	m_worldGui->Update();

	/// 하이라키 창 : 계층구조 관리, 엔티티 선택
	m_selectEntity = m_hierarchyGui->Update(m_selectEntity);

	/// 인스펙터 창 : 선택된 엔티티 세부 정보
	m_inspectorGui->Update(m_selectEntity, dt);

	/// 프로젝트 창 : 에셋, 리소스 등을 관리 -> 드래그앤드랍으로 컴포넌트에 붙일기 (구현 예정)
	m_projectGui->Update(m_selectEntity);

	/// 잡다한 창 (디버그, 카메라, 테스트 등)
	m_severalGui->Update();
}

void Editor::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// ImGui 창이 프로그램 밖으로 이동했을때 제대로 렌더링 되도록 하는 코드
	// 다중 뷰포트 관련??
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Editor::Finalize()
{
	// ImGui Clear
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// MapEditor Clear

}

void Editor::Theme_Blender(ImGuiStyle* dst /*= NULL*/)
{
	// 'Blender Dark' theme from v3.0.0 [Improvised]
	// Colors grabbed using X11 Soft/xcolor
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;
	ImGui::StyleColorsDark(style);//Reset to base/dark theme
	colors[ImGuiCol_Text] = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.19f, 0.39f, 0.69f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.20f, 0.39f, 0.69f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
	style->WindowPadding = ImVec2(12.00f, 8.00f);
	style->ItemSpacing = ImVec2(7.00f, 3.00f);
	style->GrabMinSize = 20.00f;
	style->WindowRounding = 8.00f;
	style->FrameBorderSize = 0.00f;
	style->FrameRounding = 4.00f;
	style->GrabRounding = 12.00f;
}

void Editor::Theme_Cyberpunk(ImGuiStyle* dst /*= NULL*/)
{
	// Cyberpunk Neon [Improvised]
	// https://github.com/Roboron3042/Cyberpunk-Neon
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;
	ImGui::StyleColorsDark(style);//Reset to base/dark theme
	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.82f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.36f, 0.63f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.04f, 0.12f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.04f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.06f, 0.27f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.75f, 1.00f, 0.20f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.34f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.81f, 0.95f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25f, 0.00f, 0.54f, 0.81f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.88f, 1.00f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.95f, 0.19f, 0.67f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.19f, 0.92f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 0.95f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.81f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.00f, 0.98f, 1.00f, 0.52f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.94f, 0.00f, 1.00f, 0.80f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.95f, 1.00f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.94f, 0.00f, 1.00f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.74f, 0.00f, 1.00f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.34f, 0.00f, 1.00f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 1.00f, 0.85f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.89f, 0.26f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.88f, 1.00f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.36f, 0.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.92f, 1.00f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.62f, 0.00f, 0.80f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 0.88f, 1.00f);
	style->FrameBorderSize = 0.00f;
	style->WindowRounding = 0.00f;
	style->FrameRounding = 0.00f;
	style->ScrollbarRounding = 0.00f;
	style->GrabRounding = 0.00f;
}

void Editor::Theme_Nord(ImGuiStyle* dst /*= NULL*/)
{
	// Nord/Nordic GTK [Improvised]
	// https://github.com/EliverLara/Nordic
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;
	ImGui::StyleColorsDark(style);//Reset to base/dark theme
	colors[ImGuiCol_Text] = ImVec4(0.85f, 0.87f, 0.91f, 0.88f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.49f, 0.50f, 0.53f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.09f, 0.09f, 0.09f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.23f, 0.26f, 0.32f, 0.60f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.61f, 0.74f, 0.87f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.24f, 0.31f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.15f, 0.18f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.17f, 0.19f, 0.23f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
	style->WindowBorderSize = 1.00f;
	style->ChildBorderSize = 1.00f;
	style->PopupBorderSize = 1.00f;
	style->FrameBorderSize = 1.00f;
}

void Editor::Theme_MoonLight(ImGuiStyle* dst /*= NULL*/)
{
	// Moonlight style by deathsu/madam-herta
// https://github.com/Madam-Herta/Moonlight/
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 1.0f;
	style.WindowPadding = ImVec2(12.0f, 12.0f);
	style.WindowRounding = 11.5f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
	style.FrameRounding = 11.89999961853027f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
	style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
	style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
	style.IndentSpacing = 0.0f;
	style.ColumnsMinSpacing = 4.900000095367432f;
	style.ScrollbarSize = 11.60000038146973f;
	style.ScrollbarRounding = 15.89999961853027f;
	style.GrabMinSize = 3.700000047683716f;
	style.GrabRounding = 20.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09250493347644806f, 0.100297249853611f, 0.1158798336982727f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1120669096708298f, 0.1262156516313553f, 0.1545064449310303f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.971993625164032f, 1.0f, 0.4980392456054688f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7953379154205322f, 0.4980392456054688f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1821731775999069f, 0.1897992044687271f, 0.1974248886108398f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1545050293207169f, 0.1545048952102661f, 0.1545064449310303f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1414651423692703f, 0.1629818230867386f, 0.2060086131095886f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1072951927781105f, 0.107295036315918f, 0.1072961091995239f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1293079704046249f, 0.1479243338108063f, 0.1931330561637878f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1459212601184845f, 0.1459220051765442f, 0.1459227204322815f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.999999463558197f, 1.0f, 0.9999899864196777f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1249424293637276f, 0.2735691666603088f, 0.5708154439926147f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8841201663017273f, 0.7941429018974304f, 0.5615870356559753f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9570815563201904f, 0.9570719599723816f, 0.9570761322975159f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9356134533882141f, 0.9356129765510559f, 0.9356223344802856f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.266094446182251f, 0.2890366911888123f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}