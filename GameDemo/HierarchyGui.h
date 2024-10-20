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
/// ���̶�Ű â�� �����ϴ� Ŭ����
/// �ۼ��� : �赿��
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
	// ���̶�Ű â
	void ShowHierarchy();

	// ��ƼƼ�� (Tree ���·� ���������� ��Ÿ����.)
	void ReadEntities();
	void ReadEntityChildren(std::weak_ptr<Entity> entity);

	// �ѤѤѤѤѤѤѤѤѤ� ��ɵ� �ѤѤѤѤѤѤѤѤ�
	// ���� �� �̸� ����
	void ChangeCurrentSceneName();

	// ��ƼƼ �˻�
	void SearchEntity();

	// ��ƼƼ �߰� ��ư
	void AddEntityButton();

	// ��ƼƼ ���� ��ư
	void DestoryEntityButton();

	// ������ �߰� (������Ʈ â -> ���̶�Ű â)
	void DragDropTargetAddPrefabButton();

	// ��ƼƼ ���� ��ư (���� ���������� �������� �ʴ´�. ���Ŀ� �ڽ� ���������� ���� �� ����)
	void CloneEntityButton();

	// ���̶�Ű â ��ƼƼ�� ����
	void SortEntitiesButton();

	// ��ƼƼ ���� ��ư (�ν�����â�� ���� ���� + �������� ���� + �ش� ��ƼƼ�� ī�޶� �̵� ����)
	void SelectEntityButton(std::weak_ptr<Entity> entity);

	// �������� ������ ���� �巡�׾ص�� 
	void DragDropEntityHierarchy(std::weak_ptr<Entity> entity);

	// �������� ����� ���� �巡�׾ص��
	void DragDropSourceMakePrefab(std::weak_ptr<Entity> entity);

	// �ѤѤѤѤѤѤѤѤ� �˾�
	
	// ������ ���� ���� �˾�
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

	// ���� �� �̸� ����
	char m_sceneNameBuffer[256];
	std::wstring m_currentSceneName;

	bool m_failedMakePrefab;
	bool m_isCurrentEntityPrefab;

	ImVec4 m_destroyPrefabColor = { 1.f, 0.f, 0.f, 1.0f };
};
