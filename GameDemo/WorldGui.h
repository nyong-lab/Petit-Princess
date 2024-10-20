#pragma once

#include "BaseGui.h"

class IGraphics;
class CameraManager;
class ResourceManager;
class SceneManager;
class CollisionManager;
class ProjectGui;

class WorldGui : public BaseGui
{
public:
	// ������ �Ҹ���
	WorldGui();
	~WorldGui() {}

	// �ʱ�ȭ
	void Initialize(
		std::shared_ptr<ProjectGui> projectGui,
		std::weak_ptr<IGraphics> graphics,
		std::weak_ptr<SceneManager> sceneManager,
		std::weak_ptr<CameraManager> cameraManager,
		std::weak_ptr<ResourceManager> gameResourceManager);

	// ������Ʈ
	void Update();

private:
	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� �޴� �� ��ư ���� 
	
	// ���� �޴���
	void MainMenuBar();

	// ���� �޴�
	void FileMenu();

	// Gui �¿��� �޴�
	void GuiOnOffMenu();

	// ������ �׸� ����
	void ChangeTheme();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� �˾� â

	// �ùķ��̼� On ���� ���� ���� �˾� â
	void FailedToSimulation();

	// ���� �޴� ���� �˾� â
	void FileMenuPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤ� �� ����

	// �� ���� �˾� â
	void CreateScenePopup();

	// �� ���� ���� �˾� â
	void CreateSceneSuccessedPopup();

	// �� ���� ���� �˾� â
	void CreateSceneFailedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤ� �� ü����

	// �� ��ȯ �˾� â
	void ChangeScenePopup();

	// �� ��ȯ ���� �˾� â
	void ChangeSceneSuccessedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤ� �� ���̺�

	// �� ���� �˾� â
	void SaveScenePopup();

	// �� ���� ���� �˾� â
	void SaveSceneSuccessedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤ� �� �ε�

	// �� �ε� �� ���̺� �˾� â
	void LoadScenePrevSaveScenePopup();

	// �� �ε� �˾� â
	void LoadScenePopup();

	// �� �ε� ���� �˾� â
	void LoadSceneSuccessedPopup();

	// �� �ε� ���� �˾� â
	void LoadSceneFailedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤ� �� ����

	// �� ���� �˾� â
	void MergeScenePopup();

	// �� ���� ���� �˾� â
	void MergeSceneSuccessedPopup();

	// �� ���� ���� �˾� â
	void MergeSceneFailedPopup();

	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� �������� ��� ���� �Լ�

	// �� �����ϱ�
	void CreateScene();

	// �� ����Ʈ (�ε�)
	void LoadSceneList();

	// �� �����ϱ�
	void SaveScene();

	// �� ����Ʈ (����)
	void MergeSceneList();

private: 
	// �Ŵ���
	std::weak_ptr<IGraphics> m_graphics;
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<ResourceManager> m_gameResourceManager;

	std::weak_ptr<ProjectGui> m_projectGui;

	// �Է��� �� �̸�
	std::string m_inputSceneName;

	// ������ �� �̸�
	std::string m_changeSceneName;

	// ���� �� �̸�
	std::wstring m_currentSceneName;

	// ������ �� ���
	std::wstring m_loadScenePath;

	// ������ ���� �������� ��, ���Ϳ� ��Ƶд�.
	std::vector<std::string> m_selectedMergeSceneList;
	char m_sceneListComboSearchBuffer[256];
	std::string m_searchSceneName;

	// ������ �׸� ����Ʈ
	std::vector<std::string> m_themeList;
	
	// �˾�
	bool m_isCreateScenePopup;
	bool m_isChangeScenePopup;
	bool m_isSaveScenePopup;
	bool m_isLoadScenePopup;
	bool m_isLoadScenePrevSaveScenePopup;
	bool m_isMergeScenePopup;

	// ���� �˾�
	bool m_isCreateSceneSuccessedPopup;
	bool m_isChangeSceneSuccessedPopup;
	bool m_isSaveSceneSuccessedPopup;
	bool m_isLoadSceneSuceesedPopup;
	bool m_isMergeSceneSuccesedPopup;

	// ���� �˾�
	bool m_isCreateSceneFailedPopup;
	bool m_isLoadSceneFailedPopup;
	bool m_failedToSimulation;
	bool m_isMergeSceneFailedPopup;

	// �⺻ if�� bool
	bool m_isChangeNewScene;
	bool m_isMergeSelectScene;
	bool m_isMergeNewScene;

	friend class ProjectGui;
};


//// ȭ�� �� ����� ��� ����
//ImVec2 centerPos = ImVec2((ImGui::GetIO().DisplaySize.x - ImGui::GetWindowSize().x) * 0.5f, (ImGui::GetIO().DisplaySize.y - ImGui::GetWindowSize().y) * 0.5f);
//ImGui::SetWindowPos(centerPos);
//
//// ��ư ��� ����
//float buttonWidth = 120.0f; // ��ư�� �� ����
//ImVec2 buttonPos = ImVec2((ImGui::GetWindowSize().x - buttonWidth) * 0.5f, ImGui::GetCursorPosY());
//ImGui::SetCursorPos(buttonPos);