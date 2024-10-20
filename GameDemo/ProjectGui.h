#pragma once

#include "BaseGui.h"

class IGraphics;
class SceneManager;
class CameraManager;
class ResourceManager;

class Entity;
class WorldGui;
class InspectorGui;
class Transform;

/// <summary>
/// 에셋, 리소스등을 띄우는 창을 관리하는 클래스
/// 작성자 : 김동일
/// </summary>
class ProjectGui : public BaseGui
{
	friend class WorldGui;

public:
	ProjectGui();
	~ProjectGui() {}

	void Initialize(
		std::weak_ptr<WorldGui> worldGui,
		std::weak_ptr<InspectorGui> inspectorGui,
		std::weak_ptr<IGraphics> graphics,
		std::weak_ptr<SceneManager> sceneManager,
		std::weak_ptr<CameraManager> cameraManager,
		std::weak_ptr<ResourceManager> gameResourceManager);

	void Update(std::weak_ptr<Entity> selectEntity);

private:
	// 프로젝트 창
	void ShowProject();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 프로젝트창에 띄울 폴더 리스트
	void DirectoryList();

	// 리소스를 찾을 검색 창
	void SerchResource();

	// 입력한 검색 버퍼를 가지고서 리소스 경로를 찾아서 띄운다.
	void SerchResourcePath();

	// 프리팹, 씬 삭제 버튼 (경로에 있는 파일 삭제)
	void DestroyPrefabAndSceneButton();

	// data 폴더가 아니라면 뒤로 돌아가는 버튼을 띄운다.
	void BackDirectory();

	// 검색한 스트링을 가지고서 알맞은 리소스들을 띄운다.
	void SearchAndShowResources();

	// 현재 바라보고있는 폴더를 순회하며 엔트리들을 띄운다.
	void TraverseCurrentDirectory();

	// 썸네일 및 패딩 사이즈 조절
	void ThumbnailPaddingSize();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 드래그앤드랍으로 프리팹 만들기
	void DragDropTargetMakePrefab();

	// 프리팹 정보 디시리얼라이즈
	void DeserializePrefabInformation(const std::filesystem::path& path);

private:
	std::weak_ptr<IGraphics> m_graphics;
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<ResourceManager> m_gameResourceManager;

	std::weak_ptr<WorldGui> m_worldGui;
	std::weak_ptr<InspectorGui> m_inspectorGui;

	std::filesystem::path m_currentDirectory;
	std::filesystem::path m_currentPath;
	std::filesystem::path m_selectPath;
	std::vector<std::filesystem::path> m_resourcesPath;
	std::filesystem::path m_dataPath;

	char m_serchBuffer[256];
	std::string m_serchResrouceName;
	std::string m_destoryButtonName;

	float m_padding = 16.f;
	float m_thumbnailSize = 70;
};

