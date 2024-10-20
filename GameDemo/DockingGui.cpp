#include "GameDemo_pch.h"
#include "DockingGui.h"

#include "Entity.h"

void DockingGui::Update()
{
	CreateDockingWindow();
}

void DockingGui::CreateDockingWindow()
{
	/// ȭ�� ��ü ũ�⿡ ���� Gui â�� ���� 
	/// â�� ��ŷ�� �ο��ؼ� ����߾��µ� ���� �̷��� ���� �ʿ䰡 ������. 
	/// �׳� ����Ʈ ������ ���ؼ� ��ŷâ�� ������ �Լ��� �־���.. 
	//// ��ŷ�� ���� ȭ�� ũ�⸸�� ����Ʈ â�� �����.
	//static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
	//
	//// ImGui�� ����Ʈ �߿��� ���� ����Ʈ�� ���� ������ �����µ� 
	//// ���� ����Ʈ�� ��ü ImGui â�� ������ ��Ÿ����. 
	//// ó���� �ʱ�ȭ �ܰ迡�� ������ hWnd ������ Ȱ���ѵ�
	//ImGuiViewport* viewport = ImGui::GetMainViewport();

	//// ������ â�̳� ��ŷ ������ ����Ʈ ������ ���ϰ� �ȴ�.
	//ImGui::SetNextWindowViewport(viewport->ID);

	//// ȭ�� �»�� �𼭸� ��ǥ, ȭ�� ũ�� (����Ʈ ũ��) 
	//ImGui::SetNextWindowPos(viewport->Pos); 
	//ImGui::SetNextWindowSize(viewport->Size); 

	//// �׵θ�, ����, �ձ۱� ���� ����
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	//// ���� (���� ���̴����ҽ��並 Ȱ���ϰ� �ȴٸ� �� ����ϰ� ǥ�� ����)
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

	/// ����Ʈ �������� ��ŷ �ý����� ���������ν� Gui�� ���� ȭ�鿡���� ���콺 �Է� ������ ������ �� �־���,
	/// ������Ʈ���� ��ŷâ�� ������ ������ �ذ��� �� �ִ�.
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}
