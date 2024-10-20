#include "GameDemo_pch.h"
#include "ProjectGui.h"

#include "Entity.h"

#include "PathManager.h"
#include "WorldGui.h"
#include "Serialize.h"
#include "InspectorGui.h"

#include "SceneManager.h"
#include "CameraManager.h"
#include "ResourceManager.h"

#include "MeshRenderer.h"
#include "StringUtils.h"


ProjectGui::ProjectGui()
	: m_currentDirectory(PathManager::GetDataPath())
	, m_serchBuffer{ 0 }
{
	m_dataPath = PathManager::GetDataPath();
}

void ProjectGui::Initialize(
	std::weak_ptr<WorldGui> worldGui,
	std::weak_ptr<InspectorGui> inspectorGui,
	std::weak_ptr<IGraphics> graphics,
	std::weak_ptr<SceneManager> sceneManager,
	std::weak_ptr<CameraManager> cameraManager,
	std::weak_ptr<ResourceManager> gameResourceManager)
{
	m_worldGui = worldGui;
	m_inspectorGui = inspectorGui;

	m_graphics = graphics;
	m_sceneManager = sceneManager;
	m_cameraManager = cameraManager;
	m_gameResourceManager = gameResourceManager;
}

void ProjectGui::Update(std::weak_ptr<Entity> selectEntity)
{
	if (m_isShowProjectGui)
	{
		ShowProject();
	}
}

void ProjectGui::ShowProject()
{
	ImGui::Begin("Project", &m_isShowProjectGui);

	// ���� ����Ʈ ����
	DirectoryList();

	ImGui::End();
}

void ProjectGui::DirectoryList()
{
	// ���ҽ� �˻� â
	SerchResource();

	// �˻��� ��Ʈ������ ������ ��ġ�ϴ� ���ҽ� ��ε��� ��ƿ´�.
	if (m_serchResrouceName.size())
		SerchResourcePath();

	ImGui::Spacing();
	ImGui::Spacing();

	// ������, �� ���� ��ư
	DestroyPrefabAndSceneButton();

	/// TODO::���� - �ͽ� 1
	// ImGui::GetContentRegionAvail().x; �� ���ϴ� �������� 
	// ù ��° �����ӿ����� ���� 0���� ���´�.. 
	// �̷��� �ϴ°� �ּ��ϱ�
	// ImGui â ũ�⿡ ���� ���� imgui render �� �ϴ� �������� ���Ǵ� ���ΰ�?
	static bool firstFrame = true;
	if (firstFrame)
	{
		firstFrame = false;
	}
	else
	{
		// ������ ����� ���� ��� ������ + ��� �Լ�
		DragDropTargetMakePrefab();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// ���� â�� x�� ���ϴ� �� ũ�� ��ŭ ������ ��� �� �ٿ� ��� ���� �� �� �ִ��� ����Ѵ�.
		// �׸��� �� �� ��ŭ ���� ������.

		// data ������ �ƴ϶�� �ڷΰ��� ��ư�� ����.
		BackDirectory();

		// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
		
		// ����� ũ��
		float cellSize = m_thumbnailSize + m_padding;

		// â�� x ũ��
		float panelWidth = ImGui::GetContentRegionAvail().x;

		// ������ ���� �����.
		int columnCount = static_cast<int>(panelWidth / cellSize);

		// Columns �Լ��� 0�� ������ ��Ÿ�� ���������. ����ó��
		if (columnCount < 1)
			columnCount = 1;

		// false ���� ���м��� ���شٴ� ��
		ImGui::Columns(columnCount, 0, false);

		// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

		// �˻�â�� ������� �ʴٸ� �˻� ��Ʈ���� ������ â�� ���ҽ����� ã�Ƽ� ����.
		if (m_serchResrouceName.size())
		{
			SearchAndShowResources();
		}
		// �ƴ϶�� ������ ������ �ִ� ��� ���ҽ��� ��ȸ�Ѵ�.
		else
		{
			TraverseCurrentDirectory();
		}

		ImGui::Columns(1);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ThumbnailPaddingSize();
	}
}

void ProjectGui::SerchResource()
{
	// ���ҽ� �˻� â
	if (ImGui::InputText("Search Resrouce", m_serchBuffer, sizeof(m_serchBuffer)))
	{
		size_t size = std::strlen(m_serchBuffer);

		for (int i = 0; i < size; ++i)
		{
			m_serchBuffer[i] = std::tolower(m_serchBuffer[i]);
		}
	}

	// ã����� ���ҽ� �̸�
	m_serchResrouceName = m_serchBuffer;

	// �˻�â �ʱ�ȭ ��ư
	if (!m_serchResrouceName.empty())
	{
		ImGui::SameLine();

		if (ImGui::Button("Clear"))
		{
			m_serchBuffer[0] = '\0';
			m_selectPath.clear();
			m_selectPrefab.reset();
		}
	}
}

void ProjectGui::SerchResourcePath()
{
	m_resourcesPath.clear();

	// �˻� ��� ���� ���� ��� ���ϰ� ������ �ݺ�
	for (const auto& entry : std::filesystem::recursive_directory_iterator(m_dataPath))
	{
		// �θ� ��ΰ� ������, �� ������ �� ��쿡�� ��θ� ������.
		if ((entry.is_regular_file() || entry.is_directory() )
			&& (entry.path().parent_path() == PathManager::GetSceneDataPath() || entry.path().parent_path() == PathManager::GetPrefabDataPath()))
		{
			std::wstring test = entry.path().parent_path();

			std::string fileName = entry.path().filename().string();

			PathManager::RemoveAllExtention(fileName);

			std::string lowerFileName;
			// file �̸� �빮�� -> �ҹ��ڷ� ��ȯ
			for (int j = 0; j < fileName.size(); ++j)
			{
				lowerFileName.push_back(std::tolower(fileName[j]));
			}

			if (lowerFileName.find(m_serchResrouceName) != std::string::npos)
			{
				m_resourcesPath.push_back(entry);
			}
		}
	}
}

void ProjectGui::BackDirectory()
{
	if (m_currentDirectory != std::filesystem::path(m_dataPath))
	{
		// data ������ ���ư��� ��ư�� ����.
		if (ImGui::Button("<---"))
		{
			m_currentDirectory = m_currentDirectory.parent_path();
			m_selectPath.clear();
			m_selectPrefab.reset();
		}
	}
}

void ProjectGui::SearchAndShowResources()
{
	for (auto& entry : m_resourcesPath)
	{
		std::string fileName = entry.filename().string();

		m_currentPath = entry;

		// �������̶�� ������ ����
		if (entry.parent_path() == PathManager::GetPrefabDataPath())
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectPrefabColor);

		// ���̶�� �� ����
		else
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectSceneColor);

		std::string buttonName = "##" + entry.string();

		// ��ư�� �����ٸ� ���� ��θ� ������ ���� ��η� ��ü
		if (ImGui::Button(buttonName.c_str(), { m_thumbnailSize,m_thumbnailSize }))
		{
			m_selectPath = m_currentPath;

			// ������ ��ΰ� ������ �����̰� ���� ���¶�� ������ ������ �������� ���� json ������ �Ľ��Ѵ�.
			if (entry.parent_path() == PathManager::GetPrefabDataPath() && std::filesystem::is_regular_file(entry))
			{
				DeserializePrefabInformation(entry);

				// Hierarcht â���� Ŭ���� ��ư�� ������� �ǵ����� ���� bool ��
				m_isClickProjectPrefab = true;
			}
		}

		ImGui::PopStyleColor();

		// ���� Ȯ���� ����
		PathManager::RemoveAllExtention(fileName);

		// ������ �����͸� �巡�׾ص������ ���� �����.
		if (entry.parent_path() == PathManager::GetPrefabDataPath())
		{
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
			{
				ImGui::Text(fileName.c_str());
				const wchar_t* itemPath = entry.c_str();
				ImGui::SetDragDropPayload("Bring Prefab", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
		}

		// 1. �����ۿ� ȣ�� �Ǿ��ִ���
		// 2. ���콺�� ���� Ŭ�� �ߴ���
		// for���߿� ����Ŭ�� �̺�Ʈ�� �ߵ��Ǿ� ���۵��ϱ⿡ ���� bool ���� ���.
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			// ���� ��ΰ� �� ������ �������� && ��������
			// ������ ������ �� �����Ͷ�� �����Ѵ�..
			if (entry.parent_path() == PathManager::GetSceneDataPath() && std::filesystem::is_directory(entry))
			{
				m_worldGui.lock()->m_isLoadScenePrevSaveScenePopup = true;
				m_worldGui.lock()->m_changeSceneName = fileName;
				m_worldGui.lock()->m_loadScenePath = StringUtils::ToWString(fileName);

				m_worldGui.lock()->LoadScenePrevSaveScenePopup();
				m_worldGui.lock()->LoadScenePopup();
				m_worldGui.lock()->LoadSceneSuccessedPopup();
				m_worldGui.lock()->LoadSceneFailedPopup();
			}
		}

		// ���� �̸� ��ư �Ʒ��� ����
		ImGui::TextWrapped(fileName.c_str());

		// ���� ���� �̵�
		ImGui::NextColumn();
	}
}

void ProjectGui::TraverseCurrentDirectory()
{
	for (auto& entry : std::filesystem::directory_iterator(m_currentDirectory))
	{
		// ���� ������ ��θ� �����´�.
		const auto& path = entry.path();
		m_currentPath = path;

		// �� ��θ� ������ ���� ���� ����η� �����.
		auto relativePath = std::filesystem::relative(path, m_dataPath);
		std::string fileName = relativePath.filename().string();

		if (m_currentDirectory == PathManager::GetPrefabDataPath() || fileName == "PrefabData")
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectPrefabColor);
		else
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectSceneColor);

		std::string buttonName = "##" + path.string();

		// ��ư�� �����ٸ� ���� ��θ� ������ ���� ��η� ��ü
		if (ImGui::Button(buttonName.c_str(), { m_thumbnailSize,m_thumbnailSize }))
		{
			m_selectPath = m_currentPath;

			// ������ ��ΰ� ������ �����̰� ���� ���¶�� ������ ������ �������� ���� json ������ �Ľ��Ѵ�.
			if (m_currentDirectory == PathManager::GetPrefabDataPath() && std::filesystem::is_regular_file(path))
			{
				DeserializePrefabInformation(path);

				// Hierarcht â���� Ŭ���� ��ư�� ������� �ǵ����� ���� bool ��
				m_isClickProjectPrefab = true;
			}
		}

		ImGui::PopStyleColor();

		// ���� Ȯ���� ����
		PathManager::RemoveAllExtention(fileName);

		// ������ �����͸� �巡�׾ص������ ���� �����.
		if (m_currentDirectory == PathManager::GetPrefabDataPath())
		{
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
			{
				ImGui::Text(fileName.c_str());
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("Bring Prefab", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
		}

		// 1. �����ۿ� ȣ�� �Ǿ��ִ���
		// 2. ���콺�� ���� Ŭ�� �ߴ���
		// for���߿� ����Ŭ�� �̺�Ʈ�� �ߵ��Ǿ� ���۵��ϱ⿡ ���� bool ���� ���.
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			// ���� ��ΰ� �� ������ �������� && ��������
			// ������ ������ �� �����Ͷ�� �����Ѵ�..
			if (m_currentDirectory == PathManager::GetSceneDataPath() && std::filesystem::is_directory(path))
			{
				// �ùķ��̼� ���̶�� fail
				if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
				{
					m_worldGui.lock()->m_failedToSimulation = true;
					m_worldGui.lock()->FailedToSimulation();
					return;
				}

				m_worldGui.lock()->m_isLoadScenePrevSaveScenePopup = true;
				m_worldGui.lock()->m_changeSceneName = fileName;
				m_worldGui.lock()->m_loadScenePath = StringUtils::ToWString(fileName);

				m_worldGui.lock()->LoadScenePrevSaveScenePopup();
				m_worldGui.lock()->LoadScenePopup();
				m_worldGui.lock()->LoadSceneSuccessedPopup();
				m_worldGui.lock()->LoadSceneFailedPopup();
			}
			// ���� ��ΰ� ������ ������ �������� && ��������
			// ������ ������ �������̶�� �����Ѵ�..
			else if (m_currentDirectory == PathManager::GetPrefabDataPath() && std::filesystem::is_regular_file(path))
			{

			}
			// ���� ��ΰ� ������ �����̰� ����Ŭ���� �������� ���� ���� Ȯ��
			else if (m_currentDirectory == PathManager::GetDataPath() && entry.is_directory())
			{
				// ���� ��θ� Ŭ���� ������ ����
				m_currentDirectory /= path.filename();
			}
		}

		ImGui::TextWrapped(fileName.c_str());

		// ���� ���� �̵�
		ImGui::NextColumn();
	}
}

void ProjectGui::ThumbnailPaddingSize()
{
	// ����� �� �е� ������ ���� 
	ImGui::SliderFloat("Thumbnail Size", &m_thumbnailSize, 16, 150);
	ImGui::SliderFloat("Padding", &m_padding, 0, 44);
}

void ProjectGui::DragDropTargetMakePrefab()
{
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.9f, 1.f, 1.0f));
	ImGui::TextWrapped(" Make Prefab \n Drag and drop the Entity in the Hierarchy window here");
	ImGui::PopStyleColor();

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("Make Prefab");

		if (payLoad)
		{
			std::shared_ptr<Entity>* entityPtr = static_cast<std::shared_ptr<Entity>*>(payLoad->Data);
			std::weak_ptr<Entity> dragEntity = *entityPtr;

			dragEntity.lock()->SetPrefabID((unsigned long long)dragEntity.lock().get());
			Serialize::SerializePrefab(dragEntity.lock());

			m_currentDirectory = PathManager::GetPrefabDataPath();
		}

		ImGui::EndDragDropTarget();
	}
}

void ProjectGui::DestroyPrefabAndSceneButton()
{
	// ������ ���� ��ư
	static bool destroyPrefab = false;

	std::wstring selectPath = m_selectPath.parent_path();

	StringUtils::Replace(selectPath, L"\\", L"/");

	if (m_selectPrefab)
	{
		destroyPrefab = true;
	}

	// ������ �� ���� ����
	if (selectPath == PathManager::GetSceneDataPath())
	{
		m_destoryButtonName = "Destroy : " + m_selectPath.filename().string();

		if (ImGui::Button(m_destoryButtonName.c_str()))
		{
			std::filesystem::remove_all(m_selectPath);

			m_selectPath.clear();
		}
	}

	// ������ ������ ����
	else if (destroyPrefab)
	{
		if (m_selectPrefab == nullptr)
			return;
		m_destoryButtonName = "Destroy : " + m_selectPrefab->GetName();

		if (ImGui::Button(m_destoryButtonName.c_str()))
		{
			destroyPrefab = false;
			std::filesystem::remove_all(m_selectPath);

			m_selectPrefab.reset();
		}
	}
}

void ProjectGui::DeserializePrefabInformation(const std::filesystem::path& path)
{
	std::string name = path.filename().string();
	StringUtils::RemoveSubstring(name, ".json");

	if (m_selectPrefab)
		m_selectPrefab.reset();

	m_selectPrefab = std::make_shared<Entity>(name);
	m_selectPrefab->DestroyComponent<Transform>();
	m_selectPrefab->SetGraphics(m_graphics);
	m_selectPrefab->SetManager(m_sceneManager);
	m_selectPrefab->SetManager(m_cameraManager);
	m_selectPrefab->SetManager(m_gameResourceManager);

	std::ifstream readData(path);


	if (readData.is_open())
	{
		// ������ ���� ���� ���� �� ������ ����Ű�� �ݺ��ڸ� �����, 
		// �� �ݺ��ڵ� ������ ������ ���ڿ��� ����
		std::string fileContent((std::istreambuf_iterator<char>(readData)),
			std::istreambuf_iterator<char>());

		// JSON �Ľ�
		nlohmann::json jsonData = nlohmann::json::parse(fileContent);

		// ���÷��� �����͸� ���Ͽ� ������Ʈ�� �߰��Ѵ�.
		TypeInfoManager* manager = TypeInfoManager::GetInstance();

		// �ݶ��̴�, fsm ���� ��쿡�� start �Լ��� �����ϰ� �ִ�.
		// ����, �ݶ��̴��� Ʈ������ ������Ʈ���� �����ϰ� �ִ�.
		// �׷��Ƿ� vector�� ��Ƶΰ� start �Լ� ȣ��
		std::vector<std::shared_ptr<Component>> components;

		// Ÿ�� �̸��� (������Ʈ �Ǵ� ��ƼƼ) �׿� �ش��ϴ� ����Ÿ���� ��ȸ
		for (const auto& [typeName, data] : jsonData.items())
		{
			// �ε����� ��ƼƼ�� ���
			if (typeName == "Entity")
			{
				m_selectPrefab->Deserialize(data);
				continue;
			}

			if (typeName == "Components")
			{
				for (const auto& [componentTypeName, data] : data.items())
				{
					std::string typeName = componentTypeName;
					if (typeName == "RayCaster1" ||
						typeName == "RayCaster2" ||
						typeName == "RayCaster3" ||
						typeName == "RayCaster4" ||
						typeName == "RayCaster5")
						typeName = "RayCaster";

					// TypeInfoManager�� ��ϵǾ� �ִ� �̸��� class �� �پ������Ƿ� �߰��Ѵ�.
					std::string fullTypeName = "class " + typeName;

					// Ÿ�� �̸��� �ش��ϴ� �ε����� �����´�.
					std::type_index index = manager->GetTypeIndex(fullTypeName);

					// �Ŵ������� �ε����� Ȱ���Ͽ� Ÿ�� ���� ��ü�� �����´�.
					const TypeInfo* typeInfo = manager->GetTypeInfo(index);

					// ��ü�� ��ϵ� �⺻ �����ڸ� ȣ���Ͽ� ������Ʈ�� �����.
					std::shared_ptr<Component> component = typeInfo->InvokdDefaultConstructor();

					m_selectPrefab->AddComponent(component);

					// json ������ ���� Deserialize
					component->Deserialize(data);

					// ��¿�� ���� �ϵ��ڵ��̴�?
					// ������Ʈ�� �Ŵ����� ����ֱ⿡..
					std::string superName = typeInfo->GetSuper()->GetName();

					if (componentTypeName == "MeshFilter")
					{
						const auto& meshFilterComponent = std::static_pointer_cast<MeshFilter>(component);
						meshFilterComponent->UpdateModelKey();
					}

					// �ݶ��̴��� fsm ������ start �Լ��� Ʈ�������� �����ϰ��ֱ⿡ 
					// �з��ؼ� ��Ƶд�.
					else if (superName == "Collider")
						components.push_back(component);		

					else if (superName == "FSM")	
						components.push_back(component);
				}
			}
		}

	}
	else
	{
		assert(!"Entity.json �б� ���� - ����");
	}

	m_isShowProjectPrefab = true;
	m_inspectorGui.lock()->m_selectEntity = m_selectPrefab;
}