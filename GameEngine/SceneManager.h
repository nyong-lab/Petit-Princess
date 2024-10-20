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
/// ���� ������ �Ŵ��� Ŭ����
/// �ۼ� : ���¿�
/// ���� : �ռ��� -> ����׷��� �߰�
/// ���� : �赿�� -> ���� �� �̸� ��ȯ �Լ� �߰� 
/// ���� : �赿�� -> ��ο��� �� �����͸� �ε��ϴ� �Լ� �߰�
/// + ��λ����� ���� �ε� �� �� �ش� ���� �����¼¼��� ���ֱ� ���ؼ� SceneManager�� �Ŵ������� �ʿ��ϴ�..
/// ���� : �赿�� -> ��ø�������� �������� ��ƼƼ ���������� ������ִ� �Լ� �߰�
/// </summary>
class SceneManager : public IManager, public std::enable_shared_from_this<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	//���� ����
	SceneManager(const SceneManager& manager) = delete;
	SceneManager& operator=(const SceneManager& manager) = delete;

	//�̵� ����
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

	// ��θ� ���� ���� �ε��Ѵ�.
	void LoadScene(std::wstring sceneName, bool isInitializeSceneData = false);

	// ��ƼƼ ������ �����´�.
	void LoadEntity(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene, bool isInitializeSceneData = false) const;

	// ������ ������ �����´�.
	void LoadPrefab(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const;

	// ������ ������ �ϳ��� ���� ���ս�Ų��.
	void MergeScenes(std::vector<std::string> mergeSceneNameList, std::shared_ptr<Scene> scene);

	// �ε��� �� ���� ������ ����
	void MosterPointerScene(std::shared_ptr<Scene> scene);

private:
	// ��ƼƼ �������� ����
	void MakeEntityHierarchy(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const;

	// ������ �������� ��������� ��ø��������
	void DeserializePrefabChild(const std::string containName, const nlohmann::json jsonData, std::shared_ptr<Scene> loadScene, std::vector<std::shared_ptr<Entity>>& prefabEntities) const;

	//// ���� ���� ������ ����
	//void StorePointerConnection(std::shared_ptr<Scene> scene);

private:
	std::map<std::string, std::shared_ptr<Scene>> m_sceneList;
	std::shared_ptr<Scene> m_curScene;
	std::shared_ptr<Scene> m_preScene;

	std::weak_ptr<ResourceManager> m_resourceManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<IGraphics> m_graphics;
};

