#include "GameDemo_pch.h"
#include "DockingGui.h"

#include "Entity.h"

void DockingGui::Update()
{
	CreateDockingWindow();
}

void DockingGui::CreateDockingWindow()
{
	/// 화면 전체 크기에 대한 Gui 창을 만들어서 
	/// 창에 도킹을 부여해서 사용했었는데 굳이 이렇게 만들 필요가 없었다. 
	/// 그냥 뷰포트 영역에 대해서 도킹창을 만들어내는 함수가 있었다.. 
	//// 도킹을 위해 화면 크기만한 뷰포트 창을 만든다.
	//static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
	//
	//// ImGui의 뷰포트 중에서 메인 뷰포트에 대한 정보를 얻어오는데 
	//// 메인 뷰포트는 전체 ImGui 창의 영역을 나타낸다. 
	//// 처음에 초기화 단계에서 가져간 hWnd 같은걸 활용한듯
	//ImGuiViewport* viewport = ImGui::GetMainViewport();

	//// 생설될 창이나 도킹 영역이 뷰포트 영역에 속하게 된다.
	//ImGui::SetNextWindowViewport(viewport->ID);

	//// 화면 좌상단 모서리 좌표, 화면 크기 (뷰포트 크기) 
	//ImGui::SetNextWindowPos(viewport->Pos); 
	//ImGui::SetNextWindowSize(viewport->Size); 

	//// 테두리, 여백, 둥글기 전부 없앰
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	//// 투명도 (추후 쉐이더리소스뷰를 활용하게 된다면 더 깔끔하게 표현 가능)
	//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.15f);

	//windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	//windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMouseInputs;

	//if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
	//	windowFlags |= ImGuiWindowFlags_NoBackground;

	//ImGui::Begin("DockSpace Demo", 0, windowFlags);

	//ImGuiIO& io = ImGui::GetIO();
	//if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	//{
	//	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	//}

	//ImGui::End();

	//ImGui::PopStyleVar(4);

	/// 뷰포트 영역으로 도킹 시스템을 구현함으로써 Gui와 실제 화면에서의 마우스 입력 연결을 제어할 수 있었고,
	/// 오브젝트들을 도킹창이 가리는 문제를 해결할 수 있다.
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}
