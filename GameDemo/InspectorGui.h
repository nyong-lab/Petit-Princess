#pragma once

#include "BaseGui.h"

class Entity;
class Component;
class IGraphics;
class CameraManager;
class SceneManager;
class ResourceManager;

enum class GraphicsTransfer::ModelKey;

#include "MeshFilter.h"

/// <summary>
/// �ν����� â�� �����ϴ� Ŭ����
/// �ۼ��� : �赿��
/// </summary>
class InspectorGui : public BaseGui
{
	friend class ProjectGui;

public:
	InspectorGui();
	~InspectorGui() {}

	void Initialize(std::weak_ptr<IGraphics> graphics, std::weak_ptr<SceneManager> sceneManager, std::weak_ptr<CameraManager> cameraManager, std::weak_ptr<ResourceManager> gameResourceManager);
	void Update(std::weak_ptr<Entity> selectEntity, double dt);

	// ������Ʈ�� ����ִ� ���÷����� ���� ����
	void ReadPropertyInformation(const Property* property);

private:
	// �ν����� â
	void ShowInspector();

	// ��ƼƼ ����
	void ReadEntityInformation();

	// ������Ʈ ����
	void ReadComponentInformation(const TypeInfo& typeInfo);

	// ���÷����� ������ Primitive Ÿ��
	void ReadPrimitiveType(const Property* property);

	// ���÷����� ������ String Ÿ��
	void ReadPrimiteveStringType(const Property* property);

	// ���÷����� ������ Enum Class Ÿ��
	void ReadEnumClassType(const Property* property);

	// std::vector Ÿ��
	void ReadSTD_VectorType(const Property* property);
	
	// ���÷����� ������ ������ Ÿ�� (map, vector, list) ��
	void ReadComplexType(const Property* property);

	// ���÷����� �����Ͱ� struct 


	// �ѤѤѤѤѤѤѤѤѤ� ��ɵ� �ѤѤѤѤѤѤѤѤ�

	// ��ƼƼ �̸� ����
	void ChangeEntityName();

	// ��ƼƼ Ȱ��ȭ ���� üũ �ڽ� 
	void ActiveCheckBox();

	// ��ƼƼ �ø� üũ �ڽ�
	void CullingEntityCheckBox();

	// Layer ���� �޺� �ڽ�
	void SelectLayer();
	
	// LayerCollision ���� �޺� �ڽ�
	void SelectLayerCollision();

	// ������Ʈ �߰� �޺� �ڽ�
	void AddComponentCombo();

	// �ؽ�Ʈ���� ����Ʈ ����
	void GetTextFileList();

	// ���� ���� ��Ʈȭ
	void FoodStoreSet(GraphicsTransfer::ModelKey storeKey);

	// �̰� ��� ���ϰ� ������Ʈ ���� delete ��ư �ΰԲ� �ٲ��.
	// ��ġ�� ReadEntityInformation �� �Ʒ�
	//// ������Ʈ ���� �޺� �ڽ�
	//void DestroyComponentButton(std::vector<std::string> destroyComponentList);

	void IsRenderEntity();

	// �ѤѤѤѤѤѤ� ��� �� �ڵ� ���� �ѤѤѤѤѤ�
	void SearchComponentCombo();
	void ComponentCombo();
	void ScriptCombo();

	// �ѤѤѤѤѤѤ� �ߺ� �ڵ� �Լ�ȭ
	void CheckColliderTypeAndInvokeFunction(const Property* property);

private:
	char m_entityNameBuffer[256];
	std::vector<std::string> m_componentStringList;
	std::vector<std::string> m_scriptStringList;
	std::weak_ptr<Component> m_currentViewComponent;
	std::weak_ptr<Entity> m_selectEntity;

	std::unordered_map<std::string, std::vector<std::string>> m_enumClassStringList;

	Vector3 m_currentAxis = { 0.f,0.f,0.f };
	Vector3 m_prevAxis = { 0.f,0.f,0.f };
	Vector3 m_degreeAxis = { 0.f,0.f,0.f };

	std::string m_currentChildComponentName;
	double m_deltaTime;

	bool m_selectBeginCombo;

	std::weak_ptr<IGraphics> m_graphics;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<ResourceManager> m_gameResourceManager;

	MeshFilter::PBRSet* m_pbrSet = nullptr;

	char m_componentComboSearchBuffer[256];
	std::string m_searchComponentName;

	char m_enumComboSerchBuffer[256];
	std::string m_serchEnumName;

	char m_wstringBuffer[256];

	std::vector<std::string> m_textList;
};

