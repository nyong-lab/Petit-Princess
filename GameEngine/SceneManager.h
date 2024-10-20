#pragma once
#include "Scene.h"
#include "IManager.h"

class Scene;
class IGraphics;
class InputManager;
class CollisionManager;
class CameraManager;
class ResourceManager;

/// <summary>
/// 2024.01.03e
/// 씬을 관리할 매니저 클래스
/// 작성 : 유승운
/// 수정 : 손서희 -> 디버그랜더 추가
/// 수정 : 김동일 -> 현재 씬 이름 반환 함수 추가 
/// 수정 : 김동일 -> 경로에서 씬 데이터를 로드하는 함수 추가
/// + 경로상으로 씬을 로드 할 때 해당 씬에 괴물셋셋셋을 해주기 위해서 SceneManager에 매니저들이 필요하다..
/// 수정 : 김동일 -> 디시리얼라이즈 과정에서 엔티티 계층구조를 만들어주는 함수 추가
/// </summary>
class SceneManager : public IManager, public std::enable_shared_from_this<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	//복사 금지
	SceneManager(const SceneManager& manager) = delete;
	SceneManager& operator=(const SceneManager& manager) = delete;

	//이동 금지
	SceneManager(const SceneManager&& manager) = delete;
	SceneManager& operator=(const SceneManager&& manager) = delete;

	std::shared_ptr<Scene> GetCurScene() { return m_curScene; }
	void SetCurScene(std::shared_ptr<Scene> scene);

	virtual void Initialize() override;
	void InitializeManager(std::shared_ptr<IGraphics> graphics, int count, std::shared_ptr<IManager> manager...);
	void SetManager(std::shared_ptr<IManager> manager);
	virtual void Start() override;
	virtual void Update(double dt) override;
	void FixedUpdate(double fixedTime);
	virtual void Finalize() override;
	void AddScene(std::string name, std::shared_ptr<Scene> scene);

	void Render(double dt);
	void DebugRender();

	void SceneChange(std::string name);

	std::shared_ptr<Scene> GetScene(std::string name);
	std::map<std::string, std::shared_ptr<Scene>>& GetSceneList() { return m_sceneList; }

	// 경로를 통해 씬을 로드한다.
	void LoadScene(std::wstring sceneName, bool isInitializeSceneData = false);

	// 엔티티 정보를 가져온다.
	void LoadEntity(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene, bool isInitializeSceneData = false) const;

	// 프리팹 정보를 가져온다.
	void LoadPrefab(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const;

	// 선택한 씬들을 하나의 씬에 병합시킨다.
	void MergeScenes(std::vector<std::string> mergeSceneNameList, std::shared_ptr<Scene> scene);

	// 로드한 씬 괴물 포인터 연결
	void MosterPointerScene(std::shared_ptr<Scene> scene);

private:
	// 엔티티 계층구조 형성
	void MakeEntityHierarchy(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const;

	// 프리팹 계층구조 재귀적으로 디시리얼라이즈
	void DeserializePrefabChild(const std::string containName, const nlohmann::json jsonData, std::shared_ptr<Scene> loadScene, std::vector<std::shared_ptr<Entity>>& prefabEntities) const;

	//// 상점 관련 포인터 연결
	//void StorePointerConnection(std::shared_ptr<Scene> scene);

private:
	std::map<std::string, std::shared_ptr<Scene>> m_sceneList;
	std::shared_ptr<Scene> m_curScene;
	std::shared_ptr<Scene> m_preScene;

	std::weak_ptr<ResourceManager> m_resourceManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<IGraphics> m_graphics;
};

