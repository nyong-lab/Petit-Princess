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

	// �׸� ����Ʈ �ۼ� 
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
		// ���� �޴�
		FileMenu();

		// Gui �¿��� �޴�
		GuiOnOffMenu();

		// �׸� ���� �޴�
		ChangeTheme();

		ImGui::EndMainMenuBar();
	}

	// ���� �޴� ���� �˾� â
	FileMenuPopup();
}

void WorldGui::FileMenu()
{
	// ���ϸ޴� ����
	if (ImGui::BeginMenu("File"))
	{
		// ���ο� �� �����
		if (ImGui::MenuItem("Create Scene"))
		{
			// �ùķ��̼� ���̶�� fail
			if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
			{
				m_failedToSimulation = true;
				ImGui::EndMenu();
				return;
			}

			m_isCreateScenePopup = true;
		}

		// �� ����
		if (ImGui::MenuItem("Save Scene"))
		{
			// �ùķ��̼� ���̶�� fail
			if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
			{
				m_failedToSimulation = true;
				ImGui::EndMenu();
				return;
			}

			m_isSaveScenePopup = true;
		}

		// �� �ε� 
		if (ImGui::BeginMenu("Load Scene"))
		{
			LoadSceneList();

			ImGui::EndMenu();
		}

		// �� ����
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
			// �� ����Ʈ 
			for (int i = 0; i < m_themeList.size(); ++i)
			{
				// �� �׸� ���� ���� ������ ��ư�� ����
				const bool isSeleted = (index == i);

				// �����ߴٸ� ������ �ε����� ����
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

				// ���õ� �׸� üũ ��ũ ǥ��
				if (isSeleted)
				{
					ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
				}
			}

			ImGui::EndCombo();
		}

		ImGui::EndMenu();
	}
}

void WorldGui::FileMenuPopup()
{
	// �ùķ��̼� On ���� ���� ���� �˾�
	FailedToSimulation();

	// ���ο� �� ���� �˾�
	CreateScenePopup();

	// ���ο� �� ���� ���� �˾�
	CreateSceneSuccessedPopup();

	// �� ���� ���� �˾�
	CreateSceneFailedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

	// �� �ε� �� ���̺� �˾�
	LoadScenePrevSaveScenePopup();

	// �� �ε� �˾�
	LoadScenePopup();

	// �� �ε� ���� �˾�
	LoadSceneSuccessedPopup();

	// �� �ε� ���� �˾�
	LoadSceneFailedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

	// �� ���� �˾�
	SaveScenePopup();

	// �� ���� ���� �˾�
	SaveSceneSuccessedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

	// �� ��ȯ �˾�
	ChangeScenePopup();

	// �� ��ȯ ���� �˾�
	ChangeSceneSuccessedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

	// �� ���� �˾�
	MergeScenePopup();

	// �� ���� ���� �˾�
	MergeSceneSuccessedPopup();

	// �� ���� ���� �˾�
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

			// ���� ��ư
			if (ImGui::Button("Create", ImVec2(130, 30)))
			{
				// �� ����
				CreateScene();

				m_isCreateScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			// ��� ��ư
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

			// �� ��ü ��ư
			if (ImGui::Button("Yes", ImVec2(100, 30)))
			{
				m_isChangeScenePopup = false;
				m_currentSceneName = std::filesystem::path(m_changeSceneName);

				m_graphics.lock()->ReleaseModel();
				m_sceneManager.lock()->SceneChange(StringUtils::ToString(m_currentSceneName));

				m_isChangeSceneSuccessedPopup = true;

				ImGui::CloseCurrentPopup();
			}

			// ��� ��ư
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

			// ���� ��ư
			if (ImGui::Button("Save", ImVec2(80, 30)))
			{
				//if()
				SaveScene();
				m_isSaveSceneSuccessedPopup = true;
				m_isSaveScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			// ��� ��ư
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
		// ���� �ε��ϱ� ���� ���� ���� ������ �������� ���� �˾�
		std::string popupString = "Save current scene : " + StringUtils::ToString(m_currentSceneName);
		CreateOpenPopup(popupString.c_str());

		if (ImGui::BeginPopupModal(popupString.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Save the current scene before loading the scene?");

			ImGui::Spacing();
			ImGui::Spacing();

			// �ε� ��ư
			if (ImGui::Button("Yes", ImVec2(80, 30)))
			{
				// ����
				SaveScene();
				m_isLoadScenePopup = true;
				m_isSaveSceneSuccessedPopup = true;
				m_isLoadScenePrevSaveScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			// ��� ��ư
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

		// �� �ε� �˾�
		std::string popupString = "Load Scene : " + m_changeSceneName;
		CreateOpenPopup(popupString.c_str());

		if (ImGui::BeginPopupModal(popupString.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			// �ε� ��ư
			if (ImGui::Button("Load", ImVec2(80, 30)))
			{
				m_isLoadScenePopup = false;
				m_graphics.lock()->ReleaseModel();
				m_sceneManager.lock()->LoadScene(m_loadScenePath);
				m_isLoadSceneSuceesedPopup = true;
				ImGui::CloseCurrentPopup();
			}

			// ��� ��ư
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
		// �� ���� �˾�
		std::string popupString = "Merge Scene";

		CreateOpenPopup(popupString.c_str());

		if (ImGui::BeginPopupModal(popupString.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// ���ս�ų ���̽� ���� �����ϱ� ���� ����Ʈ�� ����ƽ ����
			static int index = 0;
			std::vector<std::string> sceneComboList;

			// ���ο� ���� �����ϴ���
			if (ImGui::Checkbox("Merge into a new scene", &m_isMergeNewScene))
			{
				m_isMergeSelectScene = !m_isMergeSelectScene;
			}

			// ������ ���� �����ϴ���
			if (ImGui::Checkbox("Merge into the select scene", &m_isMergeSelectScene))
			{
				m_isMergeNewScene = !m_isMergeNewScene;
			}

			ImGui::Spacing();
			ImGui::Spacing();

			if (m_isMergeNewScene)
			{
				// ������ ���ο� ��ȭ�� ���� �̸�
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
					// �� ����Ʈ �˻� â
					// Combo Box �˻��� ���� �ؽ�Ʈ �Է� ������ �߰�
					if (ImGui::InputText("Search scene", m_sceneListComboSearchBuffer, sizeof(m_sceneListComboSearchBuffer)))
					{
						size_t size = std::strlen(m_sceneListComboSearchBuffer);

						for (int i = 0; i < size; ++i)
						{
							m_sceneListComboSearchBuffer[i] = std::tolower(m_sceneListComboSearchBuffer[i]);
						}
					}

					m_searchSceneName = m_sceneListComboSearchBuffer;

					// �˻�â �ʱ�ȭ ��ư
					if (!m_searchSceneName.empty())
					{
						ImGui::SameLine();

						if (ImGui::Button("Clear"))
						{
							m_sceneListComboSearchBuffer[0] = '\0';
						}
					}

					// �� ����Ʈ 
					for (int i = 0; i < sceneComboList.size(); ++i)
					{
						// �� �׸� ���� ���� ������ ��ư�� ����
						const bool isSeleted = (index == i);

						// �� �̸� �빮�� -> �ҹ��ڷ� ��ȯ
						std::string lowerName;
						for (int j = 0; j < sceneComboList[i].size(); ++j)
						{
							lowerName.push_back(std::tolower(sceneComboList[i][j]));
						}
						// �ҹ��ڷ� ��ȯ��Ų �� �̸����� �˻��� �̸��� �ִ��� ã��
						if (lowerName.find(m_searchSceneName) == std::string::npos)
						{
							continue;
						}

						// �����ߴٸ� ������ �ε����� ����
						if (ImGui::Selectable(sceneComboList[i].c_str(), isSeleted))
						{
							index = i;
						}

						// ���õ� �׸� üũ ��ũ ǥ��
						if (isSeleted)
						{
							ImGui::SetItemDefaultFocus(); // ���õ� �׸� ������ ����
						}
					}

					ImGui::EndCombo();
				}
			}

			ImGui::Spacing();
			ImGui::Text("---------- Merge List ----------");
			ImGui::Spacing();

			// ���� �� �� ����Ʈ�� ��ȸ�ϸ� �̸��� �ؽ�Ʈ�� ����.
			for (const auto& sceneName : m_selectedMergeSceneList)
			{
				ImGui::Text(sceneName.c_str());
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// ���� ��ư
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

				// ���ο� ���� �����Ѵ�.
				if (m_isMergeNewScene)
				{
					// �� ����
					CreateScene();
					m_isCreateSceneSuccessedPopup = false;

					// ���� ���� �����ٰ� ������ ������ �����Ѵ�.
					std::shared_ptr<Scene> scene = m_sceneManager.lock()->GetScene(m_inputSceneName);
					m_sceneManager.lock()->MergeScenes(m_selectedMergeSceneList, scene);
				}
				else
				{
					// ���ս�ų ���̽� ���� ������ ������ �����Ѵ�.
					std::shared_ptr<Scene> scene = m_sceneManager.lock()->GetScene(sceneComboList[index]);

					// ������ �� �̸�
					m_inputSceneName = StringUtils::ToString(scene->GetSceneName());

					// ���� ����Ʈ�� ���� �� �̸��� �ִٸ� ����Ʈ���� ����. 
					// ������ ���� ��ü�� ���� ���̱� ����
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

			// ��� ��ư
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

	// path = �������� ��� + ������ �� �̸� 
	auto path = PathManager::GetSceneDataPath();
	path += "/" + m_inputSceneName;

	// ��ο� ���� ���� 
	std::filesystem::create_directory(path);

	std::wstring newSceneName = StringUtils::ToWString(m_inputSceneName);

	// �� ���� �� ���
	std::shared_ptr<Scene> newScene = std::make_shared<Scene>(newSceneName);
	newScene->Initialize(m_graphics.lock(), 3,
		m_cameraManager.lock(),
		m_gameResourceManager.lock(),
		m_sceneManager.lock());

	// ��ϵǾ� �ִ� �� ����Ʈ���� �Ȱ��� �̸��� ���� �ִٸ� ���� �����.
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
			// �ùķ��̼� ���̶�� fail
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
				// ��ƼƼ���� ���� �ٸ��ٸ� ���ʿ� �ٸ� ���̹Ƿ� ���
				if (entityPaths.size() != entities.size())
				{
					isBreak = true;
					break;
				}

				std::ifstream readData(entityPath);

				if (readData.is_open())
				{
					// ������ ���� ���� ���� �� ������ ����Ű�� �ݺ��ڸ� �����, 
					// �� �ݺ��ڵ� ������ ������ ���ڿ��� ����
					std::string fileContent((std::istreambuf_iterator<char>(readData)),
						std::istreambuf_iterator<char>());

					// JSON �Ľ�
					nlohmann::json jsonData = nlohmann::json::parse(fileContent);

					// 1. ����Ǿ��ִ� ���� ��ƼƼ �̸��� �����´�.
					// 2. ���� ���� �ش� ��ƼƼ �̸��� �ִ��� Ȯ���Ѵ�.
					// 3. ���� ���� �ִ� ��� ��ƼƼ�� ��ȸ�ϸ鼭 �� �ѹ��̶� ���� �ʴٸ� 
					// ���� ���� ������ ���� �ٸ� ���̴�.
					// 
					// !! �����Ͱ� �ٸ� �������� ���� �̸��� ���� ��쿡�� �׳� ������.
					std::string name = jsonData["Entity"]["m_name"];
					loadSceneEntitiesName.push_back(name);
				}
			}

			bool foundDifference = false;

			// ����ϴ� ���̶�� for���� ���ʿ䰡 ����.
			if (isBreak)
				goto skipA;

			for (const auto& entity : entities)
			{
				bool entityFoundInLoadScene = false;

				for (const auto& loadEntityName : loadSceneEntitiesName)
				{
					// ��ġ�ϴ� ��ƼƼ�� ã���� break;
					if (entity->GetName() == loadEntityName)
					{
						entityFoundInLoadScene = true;

						// loadSceneEntitiesName���� �ش� ��Ҹ� ����
						auto it = std::find(loadSceneEntitiesName.begin(), loadSceneEntitiesName.end(), loadEntityName);
						if (it != loadSceneEntitiesName.end())
							loadSceneEntitiesName.erase(it);

						break;
					}
				}

				// for���� ���� ���Ҵµ� ��ġ�ϴ� ��ƼƼ�� ã�� ���ߴٸ� 
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
	// �� ��� ����
	std::filesystem::path scenePath = PathManager::GetSceneDataPath();
	scenePath += "/";
	scenePath += m_currentSceneName;

	Serialize::SerializeSceneInformation(m_sceneManager.lock()->GetCurScene(), scenePath);
}

void WorldGui::MergeSceneList()
{
	// �� ���� ��ư
	if (ImGui::Button("Merge Selected Scenes"))
	{
		// �ùķ��̼� ���̶�� fail
		if (m_sceneManager.lock()->GetCurScene()->IsSimulating())
		{
			m_failedToSimulation = true;
			return;
		}

		// ���� �� �˾� ��ư
		m_isMergeScenePopup = true;
	}

	// ���� ����Ʈ �ʱ�ȭ
	if (ImGui::Button("Clear"))
	{
		m_selectedMergeSceneList.clear();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// ��� ������ ��ȸ�Ѵ�.
	for (const auto& entry : PathManager::GetSceneDataList())
	{
		// ��Ʈ�� ���� �̸�
		std::string fileName = entry.filename().string();

		// �� �̸��� ����Ʈ�� ���� ���� ���õǾ� �ִ��� üũ�Ѵ�.
		bool isSelected = std::find(m_selectedMergeSceneList.begin(), m_selectedMergeSceneList.end(), fileName) != m_selectedMergeSceneList.end();

		if (isSelected)
			ImGui::PushStyleColor(ImGuiCol_Header, m_selectPrefabAndMergeSceneColor);

		// Selectable�� ����Ͽ� �� �׸��� ǥ��
		if (ImGui::Selectable(fileName.c_str(), isSelected, ImGuiSelectableFlags_DontClosePopups))
		{
			// �׸��� ���õǰų� ������ ���� ����
			if (isSelected)
			{
				// �̹� ���õ� �׸��̸� ����
				m_selectedMergeSceneList.erase(std::remove(m_selectedMergeSceneList.begin(), m_selectedMergeSceneList.end(), fileName), m_selectedMergeSceneList.end());
			}
			else
			{
				// ���õ��� ���� �׸��̸� �߰�
				m_selectedMergeSceneList.push_back(fileName);
			}
		}

		if (isSelected)
			ImGui::PopStyleColor();
	}
}
