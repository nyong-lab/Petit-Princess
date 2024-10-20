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

	// 폴더 리스트 띄우기
	DirectoryList();

	ImGui::End();
}

void ProjectGui::DirectoryList()
{
	// 리소스 검색 창
	SerchResource();

	// 검색한 스트링값을 가지고서 일치하는 리소스 경로들을 모아온다.
	if (m_serchResrouceName.size())
		SerchResourcePath();

	ImGui::Spacing();
	ImGui::Spacing();

	// 프리팹, 씬 삭제 버튼
	DestroyPrefabAndSceneButton();

	/// TODO::동일 - 귀신 1
	// ImGui::GetContentRegionAvail().x; 를 구하는 과정에서 
	// 첫 번째 프레임에서는 값이 0으로 들어온다.. 
	// 이렇게 하는게 최선일까
	// ImGui 창 크기에 대한 값은 imgui render 를 하는 시점에서 계산되는 것인가?
	static bool firstFrame = true;
	if (firstFrame)
	{
		firstFrame = false;
	}
	else
	{
		// 프리팹 만들기 위한 노드 아이템 + 드랍 함수
		DragDropTargetMakePrefab();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// 현재 창의 x를 원하는 셀 크기 만큼 나눠서 몇개의 한 줄에 몇개의 셀이 들어갈 수 있는지 계산한다.
		// 그리고 그 수 만큼 열을 나눈다.

		// data 폴더가 아니라면 뒤로가기 버튼을 띄운다.
		BackDirectory();

		// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		
		// 썸네일 크기
		float cellSize = m_thumbnailSize + m_padding;

		// 창의 x 크기
		float panelWidth = ImGui::GetContentRegionAvail().x;

		// 나눠서 열을 만든다.
		int columnCount = static_cast<int>(panelWidth / cellSize);

		// Columns 함수에 0을 넣으면 런타임 에러가뜬다. 예외처리
		if (columnCount < 1)
			columnCount = 1;

		// false 값은 구분선을 없앤다는 뜻
		ImGui::Columns(columnCount, 0, false);

		// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

		// 검색창이 비어있지 않다면 검색 스트링을 가지고서 창에 리소스들을 찾아서 띄운다.
		if (m_serchResrouceName.size())
		{
			SearchAndShowResources();
		}
		// 아니라면 데이터 폴더에 있는 모든 리소스를 순회한다.
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
	// 리소스 검색 창
	if (ImGui::InputText("Search Resrouce", m_serchBuffer, sizeof(m_serchBuffer)))
	{
		size_t size = std::strlen(m_serchBuffer);

		for (int i = 0; i < size; ++i)
		{
			m_serchBuffer[i] = std::tolower(m_serchBuffer[i]);
		}
	}

	// 찾고싶은 리소스 이름
	m_serchResrouceName = m_serchBuffer;

	// 검색창 초기화 버튼
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

	// 검색 대상 폴더 내의 모든 파일과 폴더를 반복
	for (const auto& entry : std::filesystem::recursive_directory_iterator(m_dataPath))
	{
		// 부모 경로가 프리팹, 씬 데이터 인 경우에만 경로를 모은다.
		if ((entry.is_regular_file() || entry.is_directory() )
			&& (entry.path().parent_path() == PathManager::GetSceneDataPath() || entry.path().parent_path() == PathManager::GetPrefabDataPath()))
		{
			std::wstring test = entry.path().parent_path();

			std::string fileName = entry.path().filename().string();

			PathManager::RemoveAllExtention(fileName);

			std::string lowerFileName;
			// file 이름 대문자 -> 소문자로 변환
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
		// data 폴더로 돌아가는 버튼을 띄운다.
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

		// 프리팹이라면 프리팹 색상
		if (entry.parent_path() == PathManager::GetPrefabDataPath())
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectPrefabColor);

		// 씬이라면 씬 색상
		else
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectSceneColor);

		std::string buttonName = "##" + entry.string();

		// 버튼을 누른다면 현재 경로를 선택한 폴더 경로로 교체
		if (ImGui::Button(buttonName.c_str(), { m_thumbnailSize,m_thumbnailSize }))
		{
			m_selectPath = m_currentPath;

			// 선택한 경로가 프리팹 폴더이고 파일 형태라면 프리팹 정보를 가져오기 위해 json 파일을 파싱한다.
			if (entry.parent_path() == PathManager::GetPrefabDataPath() && std::filesystem::is_regular_file(entry))
			{
				DeserializePrefabInformation(entry);

				// Hierarcht 창에서 클릭된 버튼을 흰색으로 되돌리기 위한 bool 값
				m_isClickProjectPrefab = true;
			}
		}

		ImGui::PopStyleColor();

		// 파일 확장자 제거
		PathManager::RemoveAllExtention(fileName);

		// 프리팹 데이터를 드래그앤드랍으로 씬에 만든다.
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

		// 1. 아이템에 호버 되어있는지
		// 2. 마우스를 더블 클릭 했는지
		// for도중에 더블클릭 이벤트가 발동되어 오작동하기에 여러 bool 값을 줬다.
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			// 현재 경로가 씬 데이터 폴더인지 && 파일인지
			// 파일이 무조건 씬 데이터라고 가정한다..
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

		// 파일 이름 버튼 아래에 띄우기
		ImGui::TextWrapped(fileName.c_str());

		// 다음 열로 이동
		ImGui::NextColumn();
	}
}

void ProjectGui::TraverseCurrentDirectory()
{
	for (auto& entry : std::filesystem::directory_iterator(m_currentDirectory))
	{
		// 현재 폴더의 경로를 가져온다.
		const auto& path = entry.path();
		m_currentPath = path;

		// 이 경로를 데이터 폴더 기준 상대경로로 만든다.
		auto relativePath = std::filesystem::relative(path, m_dataPath);
		std::string fileName = relativePath.filename().string();

		if (m_currentDirectory == PathManager::GetPrefabDataPath() || fileName == "PrefabData")
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectPrefabColor);
		else
			ImGui::PushStyleColor(ImGuiCol_Button, m_projectSceneColor);

		std::string buttonName = "##" + path.string();

		// 버튼을 누른다면 현재 경로를 선택한 폴더 경로로 교체
		if (ImGui::Button(buttonName.c_str(), { m_thumbnailSize,m_thumbnailSize }))
		{
			m_selectPath = m_currentPath;

			// 선택한 경로가 프리팹 폴더이고 파일 형태라면 프리팹 정보를 가져오기 위해 json 파일을 파싱한다.
			if (m_currentDirectory == PathManager::GetPrefabDataPath() && std::filesystem::is_regular_file(path))
			{
				DeserializePrefabInformation(path);

				// Hierarcht 창에서 클릭된 버튼을 흰색으로 되돌리기 위한 bool 값
				m_isClickProjectPrefab = true;
			}
		}

		ImGui::PopStyleColor();

		// 파일 확장자 제거
		PathManager::RemoveAllExtention(fileName);

		// 프리팹 데이터를 드래그앤드랍으로 씬에 만든다.
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

		// 1. 아이템에 호버 되어있는지
		// 2. 마우스를 더블 클릭 했는지
		// for도중에 더블클릭 이벤트가 발동되어 오작동하기에 여러 bool 값을 줬다.
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			// 현재 경로가 씬 데이터 폴더인지 && 파일인지
			// 파일이 무조건 씬 데이터라고 가정한다..
			if (m_currentDirectory == PathManager::GetSceneDataPath() && std::filesystem::is_directory(path))
			{
				// 시뮬레이션 중이라면 fail
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
			// 현재 경로가 프리팹 데이터 폴더인지 && 파일인지
			// 파일이 무조건 프리팹이라고 가정한다..
			else if (m_currentDirectory == PathManager::GetPrefabDataPath() && std::filesystem::is_regular_file(path))
			{

			}
			// 현재 경로가 데이터 폴더이고 더블클릭한 아이템이 폴더 인지 확인
			else if (m_currentDirectory == PathManager::GetDataPath() && entry.is_directory())
			{
				// 현재 경로를 클릭한 폴더로 변경
				m_currentDirectory /= path.filename();
			}
		}

		ImGui::TextWrapped(fileName.c_str());

		// 다음 열로 이동
		ImGui::NextColumn();
	}
}

void ProjectGui::ThumbnailPaddingSize()
{
	// 썸네일 및 패딩 사이즈 조절 
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
	// 프리팹 삭제 버튼
	static bool destroyPrefab = false;

	std::wstring selectPath = m_selectPath.parent_path();

	StringUtils::Replace(selectPath, L"\\", L"/");

	if (m_selectPrefab)
	{
		destroyPrefab = true;
	}

	// 선택한 씬 파일 삭제
	if (selectPath == PathManager::GetSceneDataPath())
	{
		m_destoryButtonName = "Destroy : " + m_selectPath.filename().string();

		if (ImGui::Button(m_destoryButtonName.c_str()))
		{
			std::filesystem::remove_all(m_selectPath);

			m_selectPath.clear();
		}
	}

	// 선택한 프리팹 삭제
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
		// 파일의 시작 지점 부터 끝 지점을 가리키는 반복자를 만들고, 
		// 이 반복자들 사이의 내용을 문자열로 저장
		std::string fileContent((std::istreambuf_iterator<char>(readData)),
			std::istreambuf_iterator<char>());

		// JSON 파싱
		nlohmann::json jsonData = nlohmann::json::parse(fileContent);

		// 리플렉션 데이터를 통하여 컴포넌트를 추가한다.
		TypeInfoManager* manager = TypeInfoManager::GetInstance();

		// 콜라이더, fsm 같은 경우에는 start 함수에 의존하고 있다.
		// 또한, 콜라이더는 트랜스폼 컴포넌트에도 의존하고 있다.
		// 그러므로 vector에 담아두고 start 함수 호출
		std::vector<std::shared_ptr<Component>> components;

		// 타입 이름과 (컴포넌트 또는 엔티티) 그에 해당하는 데이타정보 순회
		for (const auto& [typeName, data] : jsonData.items())
		{
			// 인덱스가 엔티티인 경우
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

					// TypeInfoManager에 등록되어 있는 이름은 class 가 붙어있으므로 추가한다.
					std::string fullTypeName = "class " + typeName;

					// 타입 이름에 해당하는 인덱스를 가져온다.
					std::type_index index = manager->GetTypeIndex(fullTypeName);

					// 매니저에서 인덱스를 활용하여 타입 정보 객체를 가져온다.
					const TypeInfo* typeInfo = manager->GetTypeInfo(index);

					// 객체에 등록된 기본 생성자를 호출하여 컴포넌트를 만든다.
					std::shared_ptr<Component> component = typeInfo->InvokdDefaultConstructor();

					m_selectPrefab->AddComponent(component);

					// json 정보를 토대로 Deserialize
					component->Deserialize(data);

					// 어쩔수 없는 하드코딩이다?
					// 컴포넌트가 매니저를 들고있기에..
					std::string superName = typeInfo->GetSuper()->GetName();

					if (componentTypeName == "MeshFilter")
					{
						const auto& meshFilterComponent = std::static_pointer_cast<MeshFilter>(component);
						meshFilterComponent->UpdateModelKey();
					}

					// 콜라이더와 fsm 구조상 start 함수와 트랜스폼에 의존하고있기에 
					// 분류해서 담아둔다.
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
		assert(!"Entity.json 읽기 실패 - 동일");
	}

	m_isShowProjectPrefab = true;
	m_inspectorGui.lock()->m_selectEntity = m_selectPrefab;
}