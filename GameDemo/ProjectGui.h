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
/// ����, ���ҽ����� ���� â�� �����ϴ� Ŭ����
/// �ۼ��� : �赿��
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
	// ������Ʈ â
	void ShowProject();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

	// ������Ʈâ�� ��� ���� ����Ʈ
	void DirectoryList();

	// ���ҽ��� ã�� �˻� â
	void SerchResource();

	// �Է��� �˻� ���۸� ������ ���ҽ� ��θ� ã�Ƽ� ����.
	void SerchResourcePath();

	// ������, �� ���� ��ư (��ο� �ִ� ���� ����)
	void DestroyPrefabAndSceneButton();

	// data ������ �ƴ϶�� �ڷ� ���ư��� ��ư�� ����.
	void BackDirectory();

	// �˻��� ��Ʈ���� ������ �˸��� ���ҽ����� ����.
	void SearchAndShowResources();

	// ���� �ٶ󺸰��ִ� ������ ��ȸ�ϸ� ��Ʈ������ ����.
	void TraverseCurrentDirectory();

	// ����� �� �е� ������ ����
	void ThumbnailPaddingSize();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

	// �巡�׾ص������ ������ �����
	void DragDropTargetMakePrefab();

	// ������ ���� ��ø��������
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

