#pragma once

#include "BaseGui.h"

class Entity;
class Scene;
class CameraManager;
class ResourceManager;
class SceneManager;
class IGraphics;
class Transform;


/// <summary>
/// 하이라키 창을 관리하는 클래스
/// 작성자 : 김동일
/// </summary>
class HierarchyGui : public BaseGui
{
public:
	HierarchyGui();
	~HierarchyGui() {}

	void Initialize(
		std::weak_ptr<IGraphics> graphics,
		std::weak_ptr<SceneManager> sceneManager,
		std::weak_ptr<CameraManager> cameraManager,
		std::weak_ptr<ResourceManager> gameResourceManager);

	std::weak_ptr<Entity> Update(std::weak_ptr<Entity> selectEntity);

private:
	// 하이라키 창
	void ShowHierarchy();

	// 엔티티들 (Tree 형태로 계층구조를 나타낸다.)
	void ReadEntities();
	void ReadEntityChildren(std::weak_ptr<Entity> entity);

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 기능들 ㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	// 현재 씬 이름 변경
	void ChangeCurrentSceneName();

	// 엔티티 검색
	void SearchEntity();

	// 엔티티 추가 버튼
	void AddEntityButton();

	// 엔티티 삭제 버튼
	void DestoryEntityButton();

	// 프리팹 추가 (프로젝트 창 -> 하이라키 창)
	void DragDropTargetAddPrefabButton();

	// 엔티티 복사 버튼 (현재 계층구조는 복사하지 않는다. 추후에 자식 계층구조는 복사 할 예정)
	void CloneEntityButton();

	// 하이라키 창 엔티티들 정렬
	void SortEntitiesButton();

	// 엔티티 선택 버튼 (인스펙터창에 정보 띄우기 + 계층구조 변경 + 해당 엔티티로 카메라 이동 가능)
	void SelectEntityButton(std::weak_ptr<Entity> entity);

	// 계층구조 변경을 위한 드래그앤드랍 
	void DragDropEntityHierarchy(std::weak_ptr<Entity> entity);

	// 프리팹을 만들기 위한 드래그앤드랍
	void DragDropSourceMakePrefab(std::weak_ptr<Entity> entity);

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡ 팝업
	
	// 프리팹 생성 실패 팝업
	void FailedMakePrefabPopup();

private:
	char m_hierarchySerchBuffer[256];
	int m_defaultNameCount;
	std::string m_serchEntityName;

	std::weak_ptr<IGraphics> m_graphics;
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<ResourceManager> m_gameResourceManager;
	std::weak_ptr<Scene> m_curScene;

	std::vector<std::shared_ptr<Entity>> m_entities;
	std::weak_ptr<Entity> m_selectEntity;

	// 현재 씬 이름 버퍼
	char m_sceneNameBuffer[256];
	std::wstring m_currentSceneName;

	bool m_failedMakePrefab;
	bool m_isCurrentEntityPrefab;

	ImVec4 m_destroyPrefabColor = { 1.f, 0.f, 0.f, 1.0f };
};
