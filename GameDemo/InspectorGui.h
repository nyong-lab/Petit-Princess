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
/// 인스펙터 창을 관리하는 클래스
/// 작성자 : 김동일
/// </summary>
class InspectorGui : public BaseGui
{
	friend class ProjectGui;

public:
	InspectorGui();
	~InspectorGui() {}

	void Initialize(std::weak_ptr<IGraphics> graphics, std::weak_ptr<SceneManager> sceneManager, std::weak_ptr<CameraManager> cameraManager, std::weak_ptr<ResourceManager> gameResourceManager);
	void Update(std::weak_ptr<Entity> selectEntity, double dt);

	// 컴포넌트가 들고있는 리플렉션한 변수 정보
	void ReadPropertyInformation(const Property* property);

private:
	// 인스펙터 창
	void ShowInspector();

	// 엔티티 정보
	void ReadEntityInformation();

	// 컴포넌트 정보
	void ReadComponentInformation(const TypeInfo& typeInfo);

	// 리플렉션한 변수가 Primitive 타입
	void ReadPrimitiveType(const Property* property);

	// 리플렉션한 변수가 String 타입
	void ReadPrimiteveStringType(const Property* property);

	// 리플렉션한 변수가 Enum Class 타입
	void ReadEnumClassType(const Property* property);

	// std::vector 타입
	void ReadSTD_VectorType(const Property* property);
	
	// 리플렉션한 변수가 복잡한 타입 (map, vector, list) 등
	void ReadComplexType(const Property* property);

	// 리플렉션한 데이터가 struct 


	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 기능들 ㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 엔티티 이름 변경
	void ChangeEntityName();

	// 엔티티 활성화 여부 체크 박스 
	void ActiveCheckBox();

	// 엔티티 컬링 체크 박스
	void CullingEntityCheckBox();

	// Layer 선택 콤보 박스
	void SelectLayer();
	
	// LayerCollision 선택 콤보 박스
	void SelectLayerCollision();

	// 컴포넌트 추가 콤보 박스
	void AddComponentCombo();

	// 텍스트파일 리스트 갱신
	void GetTextFileList();

	// 음식 가게 세트화
	void FoodStoreSet(GraphicsTransfer::ModelKey storeKey);

	// 이거 사용 안하고 컴포넌트 옆에 delete 버튼 두게끔 바꿨다.
	// 위치는 ReadEntityInformation 맨 아래
	//// 컴포넌트 삭제 콤보 박스
	//void DestroyComponentButton(std::vector<std::string> destroyComponentList);

	void IsRenderEntity();

	// ㅡㅡㅡㅡㅡㅡㅡ 기능 속 코드 정리 ㅡㅡㅡㅡㅡㅡ
	void SearchComponentCombo();
	void ComponentCombo();
	void ScriptCombo();

	// ㅡㅡㅡㅡㅡㅡㅡ 중복 코드 함수화
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

