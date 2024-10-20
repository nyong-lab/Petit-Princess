#include "GameDemo_pch.h"
#include "HierarchyGui.h"

#include "Scene.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "Entity.h"
#include "Transform.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "PathManager.h"


HierarchyGui::HierarchyGui()
	: m_hierarchySerchBuffer{ 0 }
	, m_defaultNameCount(0)
	, m_failedMakePrefab(false)
{
}

void HierarchyGui::Initialize(
	std::weak_ptr<IGraphics> graphics,
	std::weak_ptr<SceneManager> sceneManager,
	std::weak_ptr<CameraManager> cameraManager,
	std::weak_ptr<ResourceManager> gameResourceManager)
{
	m_graphics = graphics;
	m_sceneManager = sceneManager;
	m_cameraManager = cameraManager;
	m_gameResourceManager = gameResourceManager;
}

std::weak_ptr<Entity> HierarchyGui::Update(std::weak_ptr<Entity> selectEntity)
{
	if (m_sceneManager.lock()->GetCurScene())
		m_currentSceneName = m_sceneManager.lock()->GetCurScene()->GetSceneName();
	m_selectEntity = selectEntity;
	m_curScene = m_sceneManager.lock()->GetCurScene();
	if (m_isShowHierarchyGui)
	{
		ShowHierarchy();
	}

	return m_selectEntity;
}

void HierarchyGui::ShowHierarchy()
{
	ImGui::Begin("Hierarchy", &m_isShowHierarchyGui);

	// 전체 도킹공간을 처리가 제대로 안먹히므로 일단 이 기능은 사용 안한다.
	//// IsWindowHovered : 마우스가 imgui 창 위에 있는지 여부를 확인
	//// IsAnyItemHovered : imgui 창 내의 어떤 항목도 호버되지 않았을 때를 확인
	//// IsMouseClicked : 마우스 왼쪽 버튼이 클릭되었을때를 확인
	//if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(0))
	//{
	//	*selectEntity = nullptr;
	//}

	// 씬 이름 변경
	ChangeCurrentSceneName();

	// 엔티티 검색 창
	SearchEntity();

	// 엔티티 추가 버튼
	AddEntityButton();

	// 엔티티 삭제 버튼
	DestoryEntityButton();

	// 엔티티 복사 버튼
	CloneEntityButton();

	// 엔티티들 정렬 버튼
	SortEntitiesButton();

	// 프리펩 추가 버튼
	DragDropTargetAddPrefabButton();

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	// 씬의 등록된 엔티티들을 Hierarchy 창에 띄운다.
	ReadEntities();

	// 프리팹 만들기 실패 팝업
	FailedMakePrefabPopup();

	ImGui::End();
}

void HierarchyGui::SearchEntity()
{
	// 검색 창
	if (ImGui::InputText("Search Entity", m_hierarchySerchBuffer, sizeof(m_hierarchySerchBuffer)))
	{
		size_t size = std::strlen(m_hierarchySerchBuffer);

		for (int i = 0; i < size; ++i)
		{
			m_hierarchySerchBuffer[i] = std::tolower(m_hierarchySerchBuffer[i]);
		}
	}

	// 찾고싶은 엔티티 이름
	m_serchEntityName = m_hierarchySerchBuffer;

	// 검색창 초기화 버튼
	if (!m_serchEntityName.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button("Clear"))
		{
			m_hierarchySerchBuffer[0] = '\0';
		}
	}
}

void HierarchyGui::AddEntityButton()
{
	if (ImGui::Button("Add Entity"))
	{
		++m_defaultNameCount;
		m_curScene.lock()->CreateAddHierarchyEntity("Entity (" + std::to_string(m_defaultNameCount) + ")");
	}
}

void HierarchyGui::DestoryEntityButton()
{
	std::string destroyStr = "Destroy Entity : ";
	if (m_selectEntity.lock())
	{
		destroyStr = "Destroy Entity : " + m_selectEntity.lock()->GetName();
	}

	if (m_selectEntity.lock())
	{
		if (ImGui::Button(destroyStr.c_str()))
		{
			CreateOpenPopup(destroyStr.c_str());
		}

		if (ImGui::BeginPopupModal(destroyStr.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to destroy this entity?");

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Dummy({ 0.0001f, 0.f });
			ImGui::SameLine();

			if (ImGui::Button("Destory Entity", ImVec2(120, 30)))
			{
				if (m_selectEntity.lock())
				{
					m_curScene.lock()->DestroyEntity(m_selectEntity);
					m_selectEntity.reset();
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			ImGui::Dummy(ImVec2(2, 0));

			ImGui::SameLine();

			if (ImGui::Button("Return", ImVec2(120, 30)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	else
	{
		if (ImGui::Button(destroyStr.c_str()))
		{
			CreateOpenPopup("Error : Delete Entity");
		}
		CreateBeginPopup("Error : Delete Entity", "Error : No entity is selected.", "OK", { 120.f, 30.f }, { 17.f, 0.f });
	}
}

void HierarchyGui::DragDropTargetAddPrefabButton()
{
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.9f, 1.f, 1.0f));
	ImGui::Text(" Bring Prefab \n Drag and drop the prefab in the project window here");
	ImGui::PopStyleColor();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Bring Prefab"))
		{
			const wchar_t* path = static_cast<const wchar_t*>(payload->Data);

			if (PathManager::PathHasExtension(path, L".json"))
			{
				m_sceneManager.lock()->LoadPrefab(path, m_sceneManager.lock()->GetCurScene());

				const std::filesystem::path& filePath = path;
				std::string name = filePath.filename().string();
				StringUtils::RemoveSubstring(name, ".json");
			}
			else
				m_failedMakePrefab = true;
		}

		ImGui::EndDragDropTarget();
	}
}

void HierarchyGui::CloneEntityButton()
{
	std::string cloneStr = "Clone Entity : ";
	if (m_selectEntity.lock())
	{
		cloneStr = "Clone Entity : " + m_selectEntity.lock()->GetName();
	}
	if (m_selectEntity.lock())
	{
		if (ImGui::Button(cloneStr.c_str()))
		{
			if (m_selectEntity.lock())
				m_curScene.lock()->CreateCloneAddEntity(m_selectEntity.lock());
		}
	}
	else
	{
		if (ImGui::Button(cloneStr.c_str()))
		{
			CreateOpenPopup("Error : Clone Entity");
		}

		CreateBeginPopup("Error : Clone Entity", "Error : No entity is selected.", "OK", { 120.f, 30.f }, { 17.f, 0.f });
	}
}

bool CompareEntitiesByName(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b)
{
	return a->GetName() < b->GetName();
}

void HierarchyGui::SortEntitiesButton()
{
	if (ImGui::Button("Sort"))
	{
		auto& entities = m_sceneManager.lock()->GetCurScene()->GetEntities();
		std::sort(entities.begin(), entities.end(), CompareEntitiesByName);
	}
}

void HierarchyGui::SelectEntityButton(std::weak_ptr<Entity> entity)
{
	// 클릭되어 있다면 버튼 색상으로 알아보기 쉽게 표시
	bool isSelected = (m_selectEntity.lock() == entity.lock());
	if (isSelected && !m_isClickProjectPrefab)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, m_selectButtonColor);
		if (ImGui::Button("select"))
		{
			// 프로젝트 창 프리팹 선택 해제 
			m_selectPrefab.reset();

			// 하이러키창 엔티티를 선택했다면 프로젝트 창 prefab은 끈다.
			m_isShowProjectPrefab = false;
			m_cameraManager.lock()->LookAt(entity.lock()->GetComponent<Transform>()->GetLocalPosition(), 10.f);
		}

		ImGui::PopStyleColor();
	}
	// 아니라면 일반 버튼
	else if (ImGui::Button("select"))
	{
		// 프로젝트 창 프리팹 선택 해제 
		m_selectPrefab.reset();
		m_isClickProjectPrefab = false;

		// 하이러키창 엔티티를 선택했다면 프로젝트 창 prefab은 끈다.
		m_isShowProjectPrefab = false;
		m_selectEntity = entity;
	}
}

void HierarchyGui::DragDropSourceMakePrefab(std::weak_ptr<Entity> entity)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
	{
		ImGui::Text(entity.lock()->GetName().c_str());

		std::shared_ptr<Entity> entityPtr = entity.lock();
		ImGui::SetDragDropPayload("Make Prefab", &entityPtr, sizeof(std::shared_ptr<Entity>));
		ImGui::EndDragDropSource();
	}
}

void HierarchyGui::DragDropEntityHierarchy(std::weak_ptr<Entity> entity)
{
	// ImGuiDragDropFlags_SourceAutoExpirePayload : 드랍된게 없다면 payload 자동 소멸
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
	{
		ImGui::Text(entity.lock()->GetName().c_str());

		std::shared_ptr<Entity> entityPtr = entity.lock();
		ImGui::SetDragDropPayload("Entity", &entityPtr, sizeof(std::shared_ptr<Entity>));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("Entity");

		if (payLoad)
		{
			std::shared_ptr<Entity>* entityPtr = static_cast<std::shared_ptr<Entity>*>(payLoad->Data);
			std::weak_ptr<Entity> child = *entityPtr;

			// 드래그 중이던 엔티티
			std::weak_ptr<Transform> dragTransform = child.lock()->GetComponent<Transform>();

			// 드랍한 위치 엔티티
			std::weak_ptr<Transform> dropTransform = entity.lock()->GetComponent<Transform>();

			// 드랍된 위치 엔티티의 자식이 드래그한 엔티티인경우에는 아무런 처리를 하지 않는다.
			if (dropTransform.lock()->IsChildOf(dragTransform))
			{
				return;
			}

			// 다시 한번 올려놓으면 서로를 분리한다.
			if (dragTransform.lock()->GetParent().lock() == dropTransform.lock())
			{
				dropTransform.lock()->RemoveChild(dragTransform.lock());
			}

			// 아니라면 드랍된곳의 자식으로 들어간다.
			else
			{
				// 여기도 분기 많음 
				dropTransform.lock()->SetChild(dragTransform.lock());
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void HierarchyGui::ReadEntities()
{
	if(m_curScene.lock())
		m_entities = m_curScene.lock()->GetEntities();
	for (auto entity : m_entities)
	{
		std::string name = entity->GetName();
		std::string lowerName;

		// Entity name 대문자 -> 소문자로 변환
		for (int j = 0; j < name.size(); ++j)
		{
			lowerName.push_back(std::tolower(name[j]));
		}

		// 소문자로 변환시킨 엔티티 이름에서 검색한 이름이 있는지 찾기
		if (lowerName.find(m_serchEntityName) == std::string::npos)
		{
			continue;
		}

		// 부모가 없는 엔티티만 고른다 -> if문 내부에서 자식들은 재귀함수로 계층구조 표현
		if (entity->GetComponent<Transform>()->GetParent().lock() == nullptr)
		{
			// 자식이 있는지 여부를 확인
			bool hasChildren = !entity->GetComponent<Transform>()->GetChildren().empty();

			// 자식 여부에 따라서 Flag 값 설정
			ImGuiTreeNodeFlags nodeFlags = hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf;

			ImGui::PushID(entity->GetID());

			if (ImGui::TreeNodeEx(entity->GetName().c_str(), nodeFlags))
			{
				ImGui::SameLine();

				// 프리팹을 만들기 위한 드래그앤드랍
				DragDropSourceMakePrefab(entity);

				// 엔티티 선택 버튼
				SelectEntityButton(entity);

				// 계층구조를 위한 드래그앤드랍
				DragDropEntityHierarchy(entity);

				// 자식 계층구조 재귀함수
				ReadEntityChildren(entity);

				ImGui::TreePop();
			}

			m_isCurrentEntityPrefab = false;

			ImGui::PopID();
		}
	}
}

void HierarchyGui::FailedMakePrefabPopup()
{
	if (m_failedMakePrefab)
	{
		CreateOpenPopup("Failed make prefab");

		if (ImGui::BeginPopupModal("Failed make prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("The file is not in JSON format.");

			ImGui::Spacing();
			ImGui::Spacing();

			/*		ImGui::Dummy({ 80, 0 });
					ImGui::SameLine();*/

			if (ImGui::Button("Close", ImVec2(150, 30)))
			{
				m_failedMakePrefab = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void HierarchyGui::ReadEntityChildren(std::weak_ptr<Entity> entity)
{
	auto& children = entity.lock()->GetComponent<Transform>()->GetChildren();
	for (auto& child : children)
	{
		bool hasChildren = !child->GetChildren().empty();
		ImGuiTreeNodeFlags nodeFlags = hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf;
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx(child->GetOwner().lock()->GetName().c_str(), nodeFlags))
		{
			// 프리팹을 만들기 위한 드래그앤드랍
			DragDropSourceMakePrefab(child->GetOwner().lock());

			ImGui::SameLine();

			SelectEntityButton(child->GetOwner().lock());

			DragDropEntityHierarchy(child->GetOwner().lock());

			if (child)
			{
				ReadEntityChildren(child->GetOwner().lock());
			}

			ImGui::TreePop();
		}
	}
}

void HierarchyGui::ChangeCurrentSceneName()
{
	std::string sceneName = StringUtils::ToString(m_currentSceneName);
	strcpy_s(m_sceneNameBuffer, sceneName.c_str());

	if (ImGui::InputText("Scene Name", m_sceneNameBuffer, sizeof(m_sceneNameBuffer)))
	{
		if (strlen(m_sceneNameBuffer) > 0)  // 텍스트가 있는 경우에만 처리
		{
			std::string newName = m_sceneNameBuffer;
			std::wstring wName = StringUtils::ToWString(newName);
			m_sceneManager.lock()->GetCurScene()->SetSceneName(wName);
			m_currentSceneName = wName;
		}
		else
		{
			m_sceneManager.lock()->GetCurScene()->SetSceneName(L"");
			m_currentSceneName = L"";
		}
	}
}

//auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
//auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
//
//auto viewportOffset = ImGui::GetWindowPos();
//
//Vector2 viewportBounds[2];
//
//viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
//viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
//
//bool viewportFocused = false;
//bool viewportHovered = false;
//
//viewportFocused = ImGui::IsWindowFocused();
//viewportHovered = ImGui::IsWindowHovered();
//
////Application::Get().GetImGuiLayer()->BlockEvents(!viewportHovered);
//
//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
//Vector2 viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
//
//uint64_t textureID = 0;
//ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
//
//// 이미지 위에 버튼을 그립니다.
//ImDrawList* drawList = ImGui::GetWindowDrawList();
//
//// 이미지의 왼쪽 상단 좌표를 가져옵니다.
//ImVec2 imagePos = ImGui::GetWindowPos();
//
//// 버튼의 위치 설정
//ImVec2 buttonPos = { imagePos.x + 10.0f, imagePos.y + 10.0f };
//
//// 버튼의 크기 설정
//ImVec2 buttonSize(100.0f, 25.0f);
//
//// 버튼의 우측 하단 좌표 계산
//ImVec2 buttonEndPos(buttonPos.x + buttonSize.x, buttonPos.y + buttonSize.y);
//
//// 버튼을 그립니다.
//drawList->AddRectFilled(buttonPos, buttonEndPos, IM_COL32(255, 0, 0, 255));
//drawList->AddText(buttonPos, IM_COL32(255, 255, 255, 255), "MyButton", nullptr);
//
//// 마우스 이벤트 처리
//if (ImGui::IsMouseHoveringRect(buttonPos, buttonEndPos))
//{
//	// 마우스가 버튼 위에 있는 경우에 수행할 동작을 추가합니다.
//	if (ImGui::IsMouseClicked(0))
//	{
//		// 마우스 왼쪽 버튼이 클릭된 경우에 수행할 동작을 추가합니다.
//		// ...
//
//
//	}
//
//
//}
//
//if (ImGui::BeginDragDropTarget())
//{
//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Bring Prefab"))
//	{
//		const wchar_t* path = (const wchar_t*)payload->Data;
//
//	}
//
//	ImGui::EndDragDropTarget();
//}