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
	// 생성자 소멸자
	WorldGui();
	~WorldGui() {}

	// 초기화
	void Initialize(
		std::shared_ptr<ProjectGui> projectGui,
		std::weak_ptr<IGraphics> graphics,
		std::weak_ptr<SceneManager> sceneManager,
		std::weak_ptr<CameraManager> cameraManager,
		std::weak_ptr<ResourceManager> gameResourceManager);

	// 업데이트
	void Update();

private:
	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 메뉴 및 버튼 관리 
	
	// 메인 메뉴바
	void MainMenuBar();

	// 파일 메뉴
	void FileMenu();

	// Gui 온오프 메뉴
	void GuiOnOffMenu();

	// 에디터 테마 변경
	void ChangeTheme();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 팝업 창

	// 시뮬레이션 On 으로 인한 실패 팝업 창
	void FailedToSimulation();

	// 파일 메뉴 관련 팝업 창
	void FileMenuPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 씬 생성

	// 씬 생성 팝업 창
	void CreateScenePopup();

	// 씬 생성 성공 팝업 창
	void CreateSceneSuccessedPopup();

	// 씬 생성 실패 팝업 창
	void CreateSceneFailedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 씬 체인지

	// 씬 전환 팝업 창
	void ChangeScenePopup();

	// 씬 전환 성공 팝업 창
	void ChangeSceneSuccessedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 씬 세이브

	// 씬 저장 팝업 창
	void SaveScenePopup();

	// 씬 저장 성공 팝업 창
	void SaveSceneSuccessedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 씬 로드

	// 씬 로드 전 세이브 팝업 창
	void LoadScenePrevSaveScenePopup();

	// 씬 로드 팝업 창
	void LoadScenePopup();

	// 씬 로드 성공 팝업 창
	void LoadSceneSuccessedPopup();

	// 씬 로드 실패 팝업 창
	void LoadSceneFailedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 씬 병합

	// 씬 병합 팝업 창
	void MergeScenePopup();

	// 씬 병합 성공 팝업 창
	void MergeSceneSuccessedPopup();

	// 씬 병합 실패 팝업 창
	void MergeSceneFailedPopup();

	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 실질적인 기능 구현 함수

	// 씬 생성하기
	void CreateScene();

	// 씬 리스트 (로드)
	void LoadSceneList();

	// 씬 저장하기
	void SaveScene();

	// 씬 리스트 (병합)
	void MergeSceneList();

private: 
	// 매니저
	std::weak_ptr<IGraphics> m_graphics;
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<ResourceManager> m_gameResourceManager;

	std::weak_ptr<ProjectGui> m_projectGui;

	// 입력한 씬 이름
	std::string m_inputSceneName;

	// 변경할 씬 이름
	std::string m_changeSceneName;

	// 현재 씬 이름
	std::wstring m_currentSceneName;

	// 가져올 씬 경로
	std::wstring m_loadScenePath;

	// 병합할 씬을 선택했을 때, 벡터에 모아둔다.
	std::vector<std::string> m_selectedMergeSceneList;
	char m_sceneListComboSearchBuffer[256];
	std::string m_searchSceneName;

	// 변경할 테마 리스트
	std::vector<std::string> m_themeList;
	
	// 팝업
	bool m_isCreateScenePopup;
	bool m_isChangeScenePopup;
	bool m_isSaveScenePopup;
	bool m_isLoadScenePopup;
	bool m_isLoadScenePrevSaveScenePopup;
	bool m_isMergeScenePopup;

	// 성공 팝업
	bool m_isCreateSceneSuccessedPopup;
	bool m_isChangeSceneSuccessedPopup;
	bool m_isSaveSceneSuccessedPopup;
	bool m_isLoadSceneSuceesedPopup;
	bool m_isMergeSceneSuccesedPopup;

	// 실패 팝업
	bool m_isCreateSceneFailedPopup;
	bool m_isLoadSceneFailedPopup;
	bool m_failedToSimulation;
	bool m_isMergeSceneFailedPopup;

	// 기본 if문 bool
	bool m_isChangeNewScene;
	bool m_isMergeSelectScene;
	bool m_isMergeNewScene;

	friend class ProjectGui;
};


//// 화면 정 가운데에 모달 고정
//ImVec2 centerPos = ImVec2((ImGui::GetIO().DisplaySize.x - ImGui::GetWindowSize().x) * 0.5f, (ImGui::GetIO().DisplaySize.y - ImGui::GetWindowSize().y) * 0.5f);
//ImGui::SetWindowPos(centerPos);
//
//// 버튼 가운데 정렬
//float buttonWidth = 120.0f; // 버튼의 폭 조절
//ImVec2 buttonPos = ImVec2((ImGui::GetWindowSize().x - buttonWidth) * 0.5f, ImGui::GetCursorPosY());
//ImGui::SetCursorPos(buttonPos);