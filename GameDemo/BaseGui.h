#pragma once


class Entity;

/// <summary>
/// Gui 창들이 공통적으로 사용하는 함수를 모아둔 Base 클래스
/// 작성자 : 김동일
/// </summary>
class BaseGui
{
public:
	BaseGui() {};
	~BaseGui() {};

public:
	void CreateOpenPopup(const char* popupName);
	void CreateBeginPopup(const char* popupName, const char* textString, const char* popupButtonName, ImVec2 popupButtonSize = { 120.f, 30.f }, ImVec2 firstDummySize = { 0.f, 0.f });

	static bool m_isShowHierarchyGui;
	static bool m_isShowInspectorGui;
	static bool m_isShowProjectGui;
	static bool m_isShowCameraGui;
	static bool m_isShowDebugGui;
	static bool m_isShowProjectPrefab;

	static ImVec4 m_projectPrefabColor;
	static ImVec4 m_projectSceneColor;
	static ImVec4 m_selectButtonColor;
	static ImVec4 m_selectPrefabAndMergeSceneColor;

	static bool m_isSelectEntityDragActive;
	static bool m_isMakePrefabDragActive;
	
	static bool m_isClickProjectPrefab;

	static std::shared_ptr<Entity> m_selectPrefab;
};

