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

	// ��ü ��ŷ������ ó���� ����� �ȸ����Ƿ� �ϴ� �� ����� ��� ���Ѵ�.
	//// IsWindowHovered : ���콺�� imgui â ���� �ִ��� ���θ� Ȯ��
	//// IsAnyItemHovered : imgui â ���� � �׸� ȣ������ �ʾ��� ���� Ȯ��
	//// IsMouseClicked : ���콺 ���� ��ư�� Ŭ���Ǿ������� Ȯ��
	//if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(0))
	//{
	//	*selectEntity = nullptr;
	//}

	// �� �̸� ����
	ChangeCurrentSceneName();

	// ��ƼƼ �˻� â
	SearchEntity();

	// ��ƼƼ �߰� ��ư
	AddEntityButton();

	// ��ƼƼ ���� ��ư
	DestoryEntityButton();

	// ��ƼƼ ���� ��ư
	CloneEntityButton();

	// ��ƼƼ�� ���� ��ư
	SortEntitiesButton();

	// ������ �߰� ��ư
	DragDropTargetAddPrefabButton();

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	// ���� ��ϵ� ��ƼƼ���� Hierarchy â�� ����.
	ReadEntities();

	// ������ ����� ���� �˾�
	FailedMakePrefabPopup();

	ImGui::End();
}

void HierarchyGui::SearchEntity()
{
	// �˻� â
	if (ImGui::InputText("Search Entity", m_hierarchySerchBuffer, sizeof(m_hierarchySerchBuffer)))
	{
		size_t size = std::strlen(m_hierarchySerchBuffer);

		for (int i = 0; i < size; ++i)
		{
			m_hierarchySerchBuffer[i] = std::tolower(m_hierarchySerchBuffer[i]);
		}
	}

	// ã����� ��ƼƼ �̸�
	m_serchEntityName = m_hierarchySerchBuffer;

	// �˻�â �ʱ�ȭ ��ư
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
	// Ŭ���Ǿ� �ִٸ� ��ư �������� �˾ƺ��� ���� ǥ��
	bool isSelected = (m_selectEntity.lock() == entity.lock());
	if (isSelected && !m_isClickProjectPrefab)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, m_selectButtonColor);
		if (ImGui::Button("select"))
		{
			// ������Ʈ â ������ ���� ���� 
			m_selectPrefab.reset();

			// ���̷�Űâ ��ƼƼ�� �����ߴٸ� ������Ʈ â prefab�� ����.
			m_isShowProjectPrefab = false;
			m_cameraManager.lock()->LookAt(entity.lock()->GetComponent<Transform>()->GetLocalPosition(), 10.f);
		}

		ImGui::PopStyleColor();
	}
	// �ƴ϶�� �Ϲ� ��ư
	else if (ImGui::Button("select"))
	{
		// ������Ʈ â ������ ���� ���� 
		m_selectPrefab.reset();
		m_isClickProjectPrefab = false;

		// ���̷�Űâ ��ƼƼ�� �����ߴٸ� ������Ʈ â prefab�� ����.
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
	// ImGuiDragDropFlags_SourceAutoExpirePayload : ����Ȱ� ���ٸ� payload �ڵ� �Ҹ�
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

			// �巡�� ���̴� ��ƼƼ
			std::weak_ptr<Transform> dragTransform = child.lock()->GetComponent<Transform>();

			// ����� ��ġ ��ƼƼ
			std::weak_ptr<Transform> dropTransform = entity.lock()->GetComponent<Transform>();

			// ����� ��ġ ��ƼƼ�� �ڽ��� �巡���� ��ƼƼ�ΰ�쿡�� �ƹ��� ó���� ���� �ʴ´�.
			if (dropTransform.lock()->IsChildOf(dragTransform))
			{
				return;
			}

			// �ٽ� �ѹ� �÷������� ���θ� �и��Ѵ�.
			if (dragTransform.lock()->GetParent().lock() == dropTransform.lock())
			{
				dropTransform.lock()->RemoveChild(dragTransform.lock());
			}

			// �ƴ϶�� ����Ȱ��� �ڽ����� ����.
			else
			{
				// ���⵵ �б� ���� 
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

		// Entity name �빮�� -> �ҹ��ڷ� ��ȯ
		for (int j = 0; j < name.size(); ++j)
		{
			lowerName.push_back(std::tolower(name[j]));
		}

		// �ҹ��ڷ� ��ȯ��Ų ��ƼƼ �̸����� �˻��� �̸��� �ִ��� ã��
		if (lowerName.find(m_serchEntityName) == std::string::npos)
		{
			continue;
		}

		// �θ� ���� ��ƼƼ�� ���� -> if�� ���ο��� �ڽĵ��� ����Լ��� �������� ǥ��
		if (entity->GetComponent<Transform>()->GetParent().lock() == nullptr)
		{
			// �ڽ��� �ִ��� ���θ� Ȯ��
			bool hasChildren = !entity->GetComponent<Transform>()->GetChildren().empty();

			// �ڽ� ���ο� ���� Flag �� ����
			ImGuiTreeNodeFlags nodeFlags = hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf;

			ImGui::PushID(entity->GetID());

			if (ImGui::TreeNodeEx(entity->GetName().c_str(), nodeFlags))
			{
				ImGui::SameLine();

				// �������� ����� ���� �巡�׾ص��
				DragDropSourceMakePrefab(entity);

				// ��ƼƼ ���� ��ư
				SelectEntityButton(entity);

				// ���������� ���� �巡�׾ص��
				DragDropEntityHierarchy(entity);

				// �ڽ� �������� ����Լ�
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
			// �������� ����� ���� �巡�׾ص��
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
		if (strlen(m_sceneNameBuffer) > 0)  // �ؽ�Ʈ�� �ִ� ��쿡�� ó��
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
//// �̹��� ���� ��ư�� �׸��ϴ�.
//ImDrawList* drawList = ImGui::GetWindowDrawList();
//
//// �̹����� ���� ��� ��ǥ�� �����ɴϴ�.
//ImVec2 imagePos = ImGui::GetWindowPos();
//
//// ��ư�� ��ġ ����
//ImVec2 buttonPos = { imagePos.x + 10.0f, imagePos.y + 10.0f };
//
//// ��ư�� ũ�� ����
//ImVec2 buttonSize(100.0f, 25.0f);
//
//// ��ư�� ���� �ϴ� ��ǥ ���
//ImVec2 buttonEndPos(buttonPos.x + buttonSize.x, buttonPos.y + buttonSize.y);
//
//// ��ư�� �׸��ϴ�.
//drawList->AddRectFilled(buttonPos, buttonEndPos, IM_COL32(255, 0, 0, 255));
//drawList->AddText(buttonPos, IM_COL32(255, 255, 255, 255), "MyButton", nullptr);
//
//// ���콺 �̺�Ʈ ó��
//if (ImGui::IsMouseHoveringRect(buttonPos, buttonEndPos))
//{
//	// ���콺�� ��ư ���� �ִ� ��쿡 ������ ������ �߰��մϴ�.
//	if (ImGui::IsMouseClicked(0))
//	{
//		// ���콺 ���� ��ư�� Ŭ���� ��쿡 ������ ������ �߰��մϴ�.
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