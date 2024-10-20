#include "GameDemo_pch.h"
#include "BaseGui.h"

void BaseGui::CreateOpenPopup(const char* popupName)
{
	ImGui::OpenPopup(popupName);

	ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing);
}

void BaseGui::CreateBeginPopup(const char* popupName, const char* textString, const char* popupButtonName, ImVec2 popupButtonSize /*= {120.f, 30.f}*/, ImVec2 firstDummySize  /*= { 0.f, 0.f }*/)
{
	if (ImGui::BeginPopupModal(popupName, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(textString);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Dummy({ firstDummySize.x, firstDummySize.y });
		ImGui::SameLine();

		if (ImGui::Button(popupButtonName, popupButtonSize))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

bool BaseGui::m_isShowHierarchyGui = true;
bool BaseGui::m_isShowInspectorGui = true;
bool BaseGui::m_isShowProjectGui = true;
bool BaseGui::m_isShowCameraGui = true;
bool BaseGui::m_isShowDebugGui = true;

bool BaseGui::m_isShowProjectPrefab = false;

ImVec4 BaseGui::m_projectPrefabColor = { 0.f, 0.7f, 0.9f, 1.0f };

ImVec4 BaseGui::m_projectSceneColor = { 0.6f, 0.6f, 0.2f, 1.0f };

ImVec4 BaseGui::m_selectButtonColor = { 1.f, 0.3f, 0.5f, 1.f };

ImVec4 BaseGui::m_selectPrefabAndMergeSceneColor = { 0.9f, 0.7f, 0.2f, 1.0f };

bool BaseGui::m_isSelectEntityDragActive = false;

bool BaseGui::m_isMakePrefabDragActive = false;

bool BaseGui::m_isClickProjectPrefab = false;

std::shared_ptr<Entity> BaseGui::m_selectPrefab = nullptr;
