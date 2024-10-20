#include "GameDemo_pch.h"
#include "WorldGui.h"

#include "IGraphics.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "PathManager.h"
#include "EventManager.h"

#include "Entity.h"
#include "Serialize.h"
#include "ProjectGui.h"

#include "candy.h"

WorldGui::WorldGui()
	: m_isCreateScenePopup(false)
	, m_isChangeScenePopup(false)
	, m_isSaveScenePopup(false)
	, m_isCreateSceneSuccessedPopup(false)
	, m_isChangeSceneSuccessedPopup(false)
	, m_isSaveSceneSuccessedPopup(false)
	, m_isCreateSceneFailedPopup(false)
	, m_isChangeNewScene(false)
	, m_isLoadSceneFailedPopup(false)
	, m_isLoadScenePopup(false)
	, m_isLoadScenePrevSaveScenePopup(false)
	, m_failedToSimulation(false)
	, m_isLoadSceneSuceesedPopup(false)
	, m_isMergeScenePopup(false)
	, m_isMergeSceneSuccesedPopup(false)
	, m_isMergeSelectScene(true)
	, m_isMergeNewScene(false)
	, m_sceneListComboSearchBuffer{ 0 }
	, m_isMergeSceneFailedPopup(false)
{

}

void WorldGui::Initialize(
	std::shared_ptr<ProjectGui> projectGui,
	std::weak_ptr<IGraphics> graphics,
	std::weak_ptr<SceneManager> sceneManager,
	std::weak_ptr<CameraManager> cameraManager,
	std::weak_ptr<ResourceManager> gameResourceManager)
{
	m_projectGui = projectGui;

	m_graphics = graphics;
	m_sceneManager = sceneManager;
	m_cameraManager = cameraManager;
	m_gameResourceManager = gameResourceManager;

	// 테마 리스트 작성 
	m_themeList = { "Nord" , "MoonLight", "Blender", "Cyberpunk", "BasicDark" };
}

void WorldGui::Update()
{
	if(m_sceneManager.lock()->GetCurScene())
		m_currentSceneName = m_sceneManager.lock()->GetCurScene()->GetSceneName();
	MainMenuBar();
}

void WorldGui::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		// 파일 메뉴
		FileMenu();

		// Gui 온오프 메뉴
		GuiOnOffMenu();

		// 테마 변경 메뉴
		ChangeTheme();

		ImGui::EndMainMenuBar();
	}

	// 파일 메뉴 관련 팝업 창
	FileMenuPopup();
}

void WorldGui::FileMenu()
{
	// 파일메뉴 시작
	if (ImGui::BeginMenu("File"))
	{
		// 새로운 씬 만들기
		if (ImGui::MenuItem("Create Scene"))
		{
			// 시뮬레이션 중이라면 fail
			if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
			{
				m_failedToSimulation = true;
				ImGui::EndMenu();
				return;
			}

			m_isCreateScenePopup = true;
		}

		// 씬 저장
		if (ImGui::MenuItem("Save Scene"))
		{
			// 시뮬레이션 중이라면 fail
			if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
			{
				m_failedToSimulation = true;
				ImGui::EndMenu();
				return;
			}

			m_isSaveScenePopup = true;
		}

		// 씬 로드 
		if (ImGui::BeginMenu("Load Scene"))
		{
			LoadSceneList();

			ImGui::EndMenu();
		}

		// 씬 병합
		if (ImGui::BeginMenu("Merge Scene"))
		{
			MergeSceneList();

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void WorldGui::GuiOnOffMenu()
{
	if (ImGui::BeginMenu("Gui On/Off"))
	{
		static bool showAllGui = true;

		ImGui::Checkbox("All", &showAllGui);
		ImGui::Checkbox("Hierarchy", &m_isShowHierarchyGui);
		ImGui::Checkbox("Inspector", &m_isShowInspectorGui);
		ImGui::Checkbox("Project", &m_isShowProjectGui);
		ImGui::Checkbox("Debug", &m_isShowDebugGui);
		ImGui::Checkbox("Camera", &m_isShowCameraGui);

		if (!showAllGui)
		{
			static bool changeAll = true;
			changeAll = !changeAll;

			showAllGui = !showAllGui;
			m_isShowHierarchyGui = changeAll;
			m_isShowInspectorGui = changeAll;
			m_isShowProjectGui = changeAll;
			m_isShowCameraGui = changeAll;
			m_isShowDebugGui = changeAll;
		}

		ImGui::EndMenu();
	}
}

void WorldGui::ChangeTheme()
{
	if (ImGui::BeginMenu("Themes"))
	{
		static int index = 0;

		if (ImGui::BeginCombo("##MergeSceneCombo", m_themeList[index].c_str()))
		{
			// 씬 리스트 
			for (int i = 0; i < m_themeList.size(); ++i)
			{
				// 각 항목에 대한 선택 가능한 버튼을 생성
				const bool isSeleted = (index == i);

				// 선택했다면 선택한 인덱스값 세팅
				if (ImGui::Selectable(m_themeList[i].c_str(), isSeleted))
				{
					index = i;

					std::string themeStr = m_themeList[i];

					if (themeStr == "BasicDark")
					{
						ImGui::StyleColorsDark();
					}
					if (themeStr == "MoonLight")
					{
						ImCandy::Theme_MoonLight();
					}
					if (themeStr == "Blender")
					{
						ImCandy::Theme_Blender();
					}
					if (themeStr == "Cyberpunk")
					{
						ImCandy::Theme_Cyberpunk();
					}
					if (themeStr == "Nord")
					{
						ImCandy::Theme_Nord();
					}
				}

				// 선택된 항목에 체크 마크 표시
				if (isSeleted)
				{
					ImGui::SetItemDefaultFocus(); // 선택된 항목에 초점을 맞춤
				}
			}

			ImGui::EndCombo();
		}

		ImGui::EndMenu();
	}
}

void WorldGui::FileMenuPopup()
{
	// 시뮬레이션 On 으로 인한 실패 팝업
	FailedToSimulation();

	// 새로운 씬 생성 팝업
	CreateScenePopup();

	// 새로운 씬 생성 성공 팝업
	CreateSceneSuccessedPopup();

	// 씬 생성 실패 팝업
	CreateSceneFailedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 씬 로드 전 세이브 팝업
	LoadScenePrevSaveScenePopup();

	// 씬 로드 팝업
	LoadScenePopup();

	// 씬 로드 성공 팝업
	LoadSceneSuccessedPopup();

	// 씬 로드 실패 팝업
	LoadSceneFailedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 씬 저장 팝업
	SaveScenePopup();

	// 씬 저장 성공 팝업
	SaveSceneSuccessedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 씬 전환 팝업
	ChangeScenePopup();

	// 씬 전환 성공 팝업
	ChangeSceneSuccessedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	// 씬 병합 팝업
	MergeScenePopup();

	// 씬 병합 성공 팝업
	MergeSceneSuccessedPopup();

	// 씬 병합 실패 팝업
	MergeSceneFailedPopup();
}

void WorldGui::CreateScenePopup()
{
	if (m_isCreateScenePopup)
	{
		CreateOpenPopup("New Scene Name");

		if (ImGui::BeginPopupModal("New Scene Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::InputText("Scene Name", &m_inputSceneName))
			{
				m_changeSceneName = m_inputSceneName;
			}
			ImGui::Spacing();
			ImGui::Spacing();

			// 생성 버튼
			if (ImGui::Button("Create", ImVec2(130, 30)))
			{
				// 씬 생성
				CreateScene();

				m_isCreateScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			// 취소 버튼
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 30)))
			{
				m_isCreateScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::CreateSceneSuccessedPopup()
{
	if (m_isCreateSceneSuccessedPopup)
	{
		CreateOpenPopup("Create Scene Successed");

		if (ImGui::BeginPopupModal("Create Scene Successed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Close", ImVec2(150, 30)))
			{
				m_projectGui.lock()->m_currentDirectory = PathManager::GetSceneDataPath();

				m_isCreateSceneSuccessedPopup = false;
				m_isChangeScenePopup = true;
				m_isChangeNewScene = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::CreateSceneFailedPopup()
{
	if (m_isCreateSceneFailedPopup)
	{
		CreateOpenPopup("Create Scene Failed");

		if (ImGui::BeginPopupModal("Create Scene Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("The scene name is empty");

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Dummy({ 5.f , 0.f });
			ImGui::SameLine();
			if (ImGui::Button("Close", ImVec2(130, 30)))
			{
				m_isCreateSceneFailedPopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return;
	}
}

void WorldGui::ChangeScenePopup()
{
	if (m_isChangeScenePopup)
	{
		std::string popupName = "Change Scene";

		popupName += " : " + m_changeSceneName;

		CreateOpenPopup(popupName.c_str());

		if (ImGui::BeginPopupModal(popupName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (m_isChangeNewScene)
			{
				ImGui::Text("Do you want to replace it with the scene you created?");
			}

			ImGui::Spacing();

			// 씬 교체 버튼
			if (ImGui::Button("Yes", ImVec2(100, 30)))
			{
				m_isChangeScenePopup = false;
				m_currentSceneName = std::filesystem::path(m_changeSceneName);

				m_graphics.lock()->ReleaseModel();
				m_sceneManager.lock()->SceneChange(StringUtils::ToString(m_currentSceneName));

				m_isChangeSceneSuccessedPopup = true;

				ImGui::CloseCurrentPopup();
			}

			// 취소 버튼
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(100, 30)))
			{
				m_isChangeScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::ChangeSceneSuccessedPopup()
{
	if (m_isChangeSceneSuccessedPopup)
	{
		CreateOpenPopup("Change Scene Successed");

		if (ImGui::BeginPopupModal("Change Scene Successed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Close", ImVec2(150, 30)))
			{
				m_isChangeSceneSuccessedPopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::SaveScenePopup()
{
	if (m_isSaveScenePopup)
	{
		std::string popupString = "Save Scene : " + StringUtils::ToString(m_currentSceneName);
		CreateOpenPopup(popupString.c_str());

		if (ImGui::BeginPopupModal(popupString.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			// 저장 버튼
			if (ImGui::Button("Save", ImVec2(80, 30)))
			{
				//if()
				SaveScene();
				m_isSaveSceneSuccessedPopup = true;
				m_isSaveScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			// 취소 버튼
			ImGui::SameLine();
			if (ImGui::Button("Cancle", ImVec2(80, 30)))
			{
				m_isSaveScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::SaveSceneSuccessedPopup()
{
	if (m_isSaveSceneSuccessedPopup)
	{
		CreateOpenPopup("Save Scene Successed");

		if (ImGui::BeginPopupModal("Save Scene Successed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string textStr = "Saved scene name : " + StringUtils::ToString(m_currentSceneName);
			ImGui::Text(textStr.c_str());

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Dummy({ 10, 0 });
			ImGui::SameLine();
			if (ImGui::Button("Close", ImVec2(110, 30)))
			{
				m_projectGui.lock()->m_currentDirectory = PathManager::GetSceneDataPath();
				m_isSaveSceneSuccessedPopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			ImGui::Dummy({ 10, 0 });

			ImGui::EndPopup();
		}
	}
}

void WorldGui::LoadScenePrevSaveScenePopup()
{
	if (m_isLoadScenePrevSaveScenePopup)
	{
		// 씬을 로드하기 전에 현재 씬을 저장할 것인지에 대한 팝업
		std::string popupString = "Save current scene : " + StringUtils::ToString(m_currentSceneName);
		CreateOpenPopup(popupString.c_str());

		if (ImGui::BeginPopupModal(popupString.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Save the current scene before loading the scene?");

			ImGui::Spacing();
			ImGui::Spacing();

			// 로드 버튼
			if (ImGui::Button("Yes", ImVec2(80, 30)))
			{
				// 저장
				SaveScene();
				m_isLoadScenePopup = true;
				m_isSaveSceneSuccessedPopup = true;
				m_isLoadScenePrevSaveScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			// 취소 버튼
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(80, 30)))
			{
				m_isLoadScenePrevSaveScenePopup = false;
				m_isLoadScenePopup = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::LoadScenePopup()
{
	if (m_isLoadScenePopup)
	{
		if (m_isSaveSceneSuccessedPopup)
			return;

		// 씬 로드 팝업
		std::string popupString = "Load Scene : " + m_changeSceneName;
		CreateOpenPopup(popupString.c_str());

		if (ImGui::BeginPopupModal(popupString.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			// 로드 버튼
			if (ImGui::Button("Load", ImVec2(80, 30)))
			{
				m_isLoadScenePopup = false;
				m_graphics.lock()->ReleaseModel();
				m_sceneManager.lock()->LoadScene(m_loadScenePath);
				m_isLoadSceneSuceesedPopup = true;
				ImGui::CloseCurrentPopup();
			}

			// 취소 버튼
			ImGui::SameLine();
			if (ImGui::Button("Cancle", ImVec2(80, 30)))
			{
				m_isLoadScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::LoadSceneSuccessedPopup()
{
	if (m_isLoadSceneSuceesedPopup)
	{
		CreateOpenPopup("Load Scene Successed");

		if (ImGui::BeginPopupModal("Load Scene Successed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Dummy({ 10, 0 });
			ImGui::SameLine();
			if (ImGui::Button("Close", ImVec2(110, 30)))
			{
				m_isLoadSceneSuceesedPopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			ImGui::Dummy({ 10, 0 });

			ImGui::EndPopup();
		}
	}
}

void WorldGui::LoadSceneFailedPopup()
{
	if (m_isLoadSceneFailedPopup)
	{
		CreateOpenPopup("Load Scene Failed");

		if (ImGui::BeginPopupModal("Load Scene Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("The current scene and the loaded scene are the same.");

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("The criteria for two scenes to be the same are as follows");

			ImGui::Spacing();

			ImGui::TextWrapped("If the names of the entities in the current scene are the same as the names of the entities in the scene to be loaded, the two scenes are equal.");

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Dummy({ 80, 0 });
			ImGui::SameLine();

			if (ImGui::Button("Close", ImVec2(150, 30)))
			{
				m_isLoadSceneFailedPopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::MergeScenePopup()
{
	if (m_isMergeScenePopup)
	{
		// 씬 병합 팝업
		std::string popupString = "Merge Scene";

		CreateOpenPopup(popupString.c_str());

		if (ImGui::BeginPopupModal(popupString.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// 병합시킬 베이스 씬을 선택하기 위한 리스트와 스태틱 변수
			static int index = 0;
			std::vector<std::string> sceneComboList;

			// 새로운 씬에 병합하는지
			if (ImGui::Checkbox("Merge into a new scene", &m_isMergeNewScene))
			{
				m_isMergeSelectScene = !m_isMergeSelectScene;
			}

			// 선택한 씬에 병합하는지
			if (ImGui::Checkbox("Merge into the select scene", &m_isMergeSelectScene))
			{
				m_isMergeNewScene = !m_isMergeNewScene;
			}

			ImGui::Spacing();
			ImGui::Spacing();

			if (m_isMergeNewScene)
			{
				// 병합할 새로운 도화지 씬의 이름
				ImGui::InputText("New Scene Name", &m_inputSceneName);

			}
			else
			{
				const auto& sceneList = m_sceneManager.lock()->GetSceneList();
				for (const auto& iter : sceneList)
				{
					sceneComboList.push_back(iter.first);
				}

				if (ImGui::BeginCombo("##MergeSceneCombo", sceneComboList[index].c_str()))
				{
					// 씬 리스트 검색 창
					// Combo Box 검색을 위한 텍스트 입력 위젯을 추가
					if (ImGui::InputText("Search scene", m_sceneListComboSearchBuffer, sizeof(m_sceneListComboSearchBuffer)))
					{
						size_t size = std::strlen(m_sceneListComboSearchBuffer);

						for (int i = 0; i < size; ++i)
						{
							m_sceneListComboSearchBuffer[i] = std::tolower(m_sceneListComboSearchBuffer[i]);
						}
					}

					m_searchSceneName = m_sceneListComboSearchBuffer;

					// 검색창 초기화 버튼
					if (!m_searchSceneName.empty())
					{
						ImGui::SameLine();

						if (ImGui::Button("Clear"))
						{
							m_sceneListComboSearchBuffer[0] = '\0';
						}
					}

					// 씬 리스트 
					for (int i = 0; i < sceneComboList.size(); ++i)
					{
						// 각 항목에 대한 선택 가능한 버튼을 생성
						const bool isSeleted = (index == i);

						// 씬 이름 대문자 -> 소문자로 변환
						std::string lowerName;
						for (int j = 0; j < sceneComboList[i].size(); ++j)
						{
							lowerName.push_back(std::tolower(sceneComboList[i][j]));
						}
						// 소문자로 변환시킨 씬 이름에서 검색한 이름이 있는지 찾기
						if (lowerName.find(m_searchSceneName) == std::string::npos)
						{
							continue;
						}

						// 선택했다면 선택한 인덱스값 세팅
						if (ImGui::Selectable(sceneComboList[i].c_str(), isSeleted))
						{
							index = i;
						}

						// 선택된 항목에 체크 마크 표시
						if (isSeleted)
						{
							ImGui::SetItemDefaultFocus(); // 선택된 항목에 초점을 맞춤
						}
					}

					ImGui::EndCombo();
				}
			}

			ImGui::Spacing();
			ImGui::Text("---------- Merge List ----------");
			ImGui::Spacing();

			// 병합 할 씬 리스트를 순회하며 이름을 텍스트로 띄운다.
			for (const auto& sceneName : m_selectedMergeSceneList)
			{
				ImGui::Text(sceneName.c_str());
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// 병합 버튼
			if (ImGui::Button("Merge", ImVec2(80, 30)))
			{
				if (m_isMergeSelectScene)
				{
					if (m_selectedMergeSceneList.size() == 0)
					{
						m_isMergeSceneFailedPopup = true;
						m_isMergeScenePopup = false;
						ImGui::CloseCurrentPopup();
						ImGui::EndPopup();
						return;
					}
				}
				else
				{
					if (m_inputSceneName.size() == 0)
					{
						m_isMergeSceneFailedPopup = true;
						m_isMergeScenePopup = false;
						ImGui::CloseCurrentPopup();
						ImGui::EndPopup();
						return;
					}
				}

				// 새로운 씬에 병합한다.
				if (m_isMergeNewScene)
				{
					// 씬 생성
					CreateScene();
					m_isCreateSceneSuccessedPopup = false;

					// 새로 만들어낸 씬에다가 선택한 씬들을 병합한다.
					std::shared_ptr<Scene> scene = m_sceneManager.lock()->GetScene(m_inputSceneName);
					m_sceneManager.lock()->MergeScenes(m_selectedMergeSceneList, scene);
				}
				else
				{
					// 병합시킬 베이스 씬에 선택한 씬들을 병합한다.
					std::shared_ptr<Scene> scene = m_sceneManager.lock()->GetScene(sceneComboList[index]);

					// 선택한 씬 이름
					m_inputSceneName = StringUtils::ToString(scene->GetSceneName());

					// 병합 리스트에 현재 씬 이름이 있다면 리스트에서 뺀다. 
					// 병합할 씬의 주체가 현재 씬이기 때문
					for (const auto& sceneName : m_selectedMergeSceneList)
					{
						if (sceneName == m_inputSceneName)
						{
							m_selectedMergeSceneList.erase(std::remove(m_selectedMergeSceneList.begin(), m_selectedMergeSceneList.end(), sceneName), m_selectedMergeSceneList.end());
						}
					}

					m_sceneManager.lock()->MergeScenes(m_selectedMergeSceneList, scene);
				}

				m_isMergeScenePopup = false;
				m_selectedMergeSceneList.clear();

				m_changeSceneName = m_inputSceneName;
				m_isMergeSceneSuccesedPopup = true;


				ImGui::CloseCurrentPopup();
			}

			// 취소 버튼
			ImGui::SameLine();
			if (ImGui::Button("Cancle", ImVec2(80, 30)))
			{
				m_isMergeScenePopup = false;
				m_selectedMergeSceneList.clear();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::MergeSceneSuccessedPopup()
{
	if (m_isMergeSceneSuccesedPopup)
	{
		CreateOpenPopup("Merge Scene Successed");

		if (ImGui::BeginPopupModal("Merge Scene Successed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string textStr = "Merged scene name : " + m_inputSceneName;
			ImGui::Text(textStr.c_str());

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Dummy({ 10, 0 });
			ImGui::SameLine();
			if (ImGui::Button("Close", ImVec2(110, 30)))
			{
				m_isMergeSceneSuccesedPopup = false;

				if (m_sceneManager.lock()->GetCurScene()->GetSceneName() != StringUtils::ToWString(m_inputSceneName))
				{
					m_loadScenePath = StringUtils::ToWString(m_inputSceneName);
					m_isLoadScenePrevSaveScenePopup = true;
				}

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			ImGui::Dummy({ 10, 0 });

			ImGui::EndPopup();
		}
	}
}

void WorldGui::MergeSceneFailedPopup()
{
	if (m_isMergeSceneFailedPopup)
	{
		CreateOpenPopup("Merge Scene Failed");

		if (ImGui::BeginPopupModal("Merge Scene Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (m_isMergeSelectScene)
				ImGui::Text("Failed because there are no scenes selected.");
			else
				ImGui::Text("Failed because you didn't include a scene name");

			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Close", ImVec2(150, 30)))
			{
				m_isMergeSceneFailedPopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::FailedToSimulation()
{
	if (m_failedToSimulation)
	{
		CreateOpenPopup("Failed Action");

		if (ImGui::BeginPopupModal("Failed Action", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Fails when the pshx simulation is in motion.");

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Dummy({ 80, 0 });
			ImGui::SameLine();

			if (ImGui::Button("Close", ImVec2(150, 30)))
			{
				m_failedToSimulation = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void WorldGui::CreateScene()
{
	if (m_inputSceneName == "")
	{
		m_isCreateSceneFailedPopup = true;
		return;
	}

	// path = 씬데이터 경로 + 생성할 씬 이름 
	auto path = PathManager::GetSceneDataPath();
	path += "/" + m_inputSceneName;

	// 경로에 폴더 생성 
	std::filesystem::create_directory(path);

	std::wstring newSceneName = StringUtils::ToWString(m_inputSceneName);

	// 씬 생성 및 등록
	std::shared_ptr<Scene> newScene = std::make_shared<Scene>(newSceneName);
	newScene->Initialize(m_graphics.lock(), 3,
		m_cameraManager.lock(),
		m_gameResourceManager.lock(),
		m_sceneManager.lock());

	// 등록되어 있는 씬 리스트에서 똑같은 이름의 씬이 있다면 덮어 씌운다.
	auto& sceneList = m_sceneManager.lock()->GetSceneList();
	const auto& iter = sceneList.find(m_inputSceneName);
	if (iter != sceneList.end())
	{
		sceneList.erase(iter);
	}

	m_sceneManager.lock()->AddScene(m_inputSceneName, newScene);

	m_isCreateSceneSuccessedPopup = true;
}

void WorldGui::LoadSceneList()
{
	for (const auto& entry : PathManager::GetSceneDataList())
	{
		std::string fileName = entry.filename().string();
		if (ImGui::MenuItem(fileName.c_str()))
		{
			// 시뮬레이션 중이라면 fail
			if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
			{
				m_failedToSimulation = true;
				return;
			}

			const auto& entityPaths = PathManager::GetSceneEntityList(entry);
			std::vector<std::shared_ptr<Entity>> entities = m_sceneManager.lock()->GetCurScene()->GetEntities();

			bool isBreak = false;

			std::vector<std::string> loadSceneEntitiesName;
			for (const auto& entityPath : entityPaths)
			{
				// 엔티티들의 수가 다르다면 애초에 다른 씬이므로 통과
				if (entityPaths.size() != entities.size())
				{
					isBreak = true;
					break;
				}

				std::ifstream readData(entityPath);

				if (readData.is_open())
				{
					// 파일의 시작 지점 부터 끝 지점을 가리키는 반복자를 만들고, 
					// 이 반복자들 사이의 내용을 문자열로 저장
					std::string fileContent((std::istreambuf_iterator<char>(readData)),
						std::istreambuf_iterator<char>());

					// JSON 파싱
					nlohmann::json jsonData = nlohmann::json::parse(fileContent);

					// 1. 저장되어있는 씬에 엔티티 이름을 가져온다.
					// 2. 현재 씬에 해당 엔티티 이름이 있는지 확인한다.
					// 3. 현재 씬에 있는 모든 엔티티를 순회하면서 단 한번이라도 같지 않다면 
					// 현재 씬과 가져올 씬은 다른 씬이다.
					// 
					// !! 데이터가 다른 씬이지만 씬의 이름이 같은 경우에는 그냥 덮어씌운다.
					std::string name = jsonData["Entity"]["m_name"];
					loadSceneEntitiesName.push_back(name);
				}
			}

			bool foundDifference = false;

			// 통과하는 중이라면 for문을 돌필요가 없다.
			if (isBreak)
				goto skipA;

			for (const auto& entity : entities)
			{
				bool entityFoundInLoadScene = false;

				for (const auto& loadEntityName : loadSceneEntitiesName)
				{
					// 일치하는 엔티티를 찾으면 break;
					if (entity->GetName() == loadEntityName)
					{
						entityFoundInLoadScene = true;

						// loadSceneEntitiesName에서 해당 요소를 삭제
						auto it = std::find(loadSceneEntitiesName.begin(), loadSceneEntitiesName.end(), loadEntityName);
						if (it != loadSceneEntitiesName.end())
							loadSceneEntitiesName.erase(it);

						break;
					}
				}

				// for문을 전부 돌았는데 일치하는 엔티티를 찾지 못했다면 
				if (!entityFoundInLoadScene)
				{
					foundDifference = true;
					break;
				}
			}

			if (!foundDifference)
			{
				m_isLoadSceneFailedPopup = true;
				continue;
			}

		skipA:

			m_isLoadScenePrevSaveScenePopup = true;
			m_changeSceneName = fileName;
			m_loadScenePath = entry.filename().wstring();
		}
	}
}

void WorldGui::SaveScene()
{
	// 씬 경로 생성
	std::filesystem::path scenePath = PathManager::GetSceneDataPath();
	scenePath += "/";
	scenePath += m_currentSceneName;

	Serialize::SerializeSceneInformation(m_sceneManager.lock()->GetCurScene(), scenePath);
}

void WorldGui::MergeSceneList()
{
	// 씬 병합 버튼
	if (ImGui::Button("Merge Selected Scenes"))
	{
		// 시뮬레이션 중이라면 fail
		if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
		{
			m_failedToSimulation = true;
			return;
		}

		// 병합 씬 팝업 버튼
		m_isMergeScenePopup = true;
	}

	// 병합 리스트 초기화
	if (ImGui::Button("Clear"))
	{
		m_selectedMergeSceneList.clear();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// 모든 씬들을 순회한다.
	for (const auto& entry : PathManager::GetSceneDataList())
	{
		// 엔트리 씬의 이름
		std::string fileName = entry.filename().string();

		// 씬 이름을 리스트에 띄우기 전에 선택되어 있는지 체크한다.
		bool isSelected = std::find(m_selectedMergeSceneList.begin(), m_selectedMergeSceneList.end(), fileName) != m_selectedMergeSceneList.end();

		if (isSelected)
			ImGui::PushStyleColor(ImGuiCol_Header, m_selectPrefabAndMergeSceneColor);

		// Selectable을 사용하여 각 항목을 표시
		if (ImGui::Selectable(fileName.c_str(), isSelected, ImGuiSelectableFlags_DontClosePopups))
		{
			// 항목이 선택되거나 해제될 때의 동작
			if (isSelected)
			{
				// 이미 선택된 항목이면 제거
				m_selectedMergeSceneList.erase(std::remove(m_selectedMergeSceneList.begin(), m_selectedMergeSceneList.end(), fileName), m_selectedMergeSceneList.end());
			}
			else
			{
				// 선택되지 않은 항목이면 추가
				m_selectedMergeSceneList.push_back(fileName);
			}
		}

		if (isSelected)
			ImGui::PopStyleColor();
	}
}
