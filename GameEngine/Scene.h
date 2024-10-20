#pragma once
#include <vector>

#include <stdio.h> //�������� ���
#include <stdarg.h> //�������� ���

class Entity;
class IGraphics;

class IManager;
class SceneManager;
class ResourceManager;
class CameraManager;
class CollisionManager;


/// <summary>
/// �� ���ӿ��� ���� ���ư����ִ� ���
/// ���⿣ ������ ��ƼƼ�� �־��ְ� ������Ʈ ������ �ؾ��Ѵٰ� ������
/// 
/// 24.01.28 [����] �赿��
/// 1. Ʈ������ ������Ʈ�� �ߺ������� ȣ����� �ʵ��� �����ص� 
///		bool �� isUpdated �� �ٽ� false�� �����ϵ��� ����
/// 2. ��ƼƼ�� ��ӹ޴� Ŭ���� (TextureBox) �� �þ ��� Clone ��ư�� �����ϱ� ���� 
///		�ϵ��ڵ� ����. (CreateCloneAddEntity �Լ�)
/// 24.01.28 [����] �ռ��� -> DebugRender �߰�
///                      -> FixedUpdate �߰�
/// 
/// 24.02.05 [����] �赿�� -> ��ø�������� �������� Ʈ�������� ��ƼƼ�� �������ڸ��� �����ְ�..
///							�׷��� �� Ʈ�������� �������ϰ� �߰����ڴ� �� �ణ ���������� 
///							��ƼƼ�� �����¼¼¸� �ϰ� ���� ����� �����ʴ� �Լ��� �������.
///							SetEntityManager();
/// 
///	24.02.06 [����] �赿�� -> DestroyEntity �ڽ��� ������ ���� �� finalize �ϸ鼭 children�� �پ��µ� 
/// range for base for���� �پ�� children�� �������� ���ϰ� empty ���� �����ͼ� ������ ������ �־���.
/// ������ Destroy �Լ� ���θ� �ٲ㼭 �ڽ��� ��ƼƼ�� �����ؼ� ���Ϳ� ��Ƶΰ� ��ƼƼ�� ���� �� remove_if �� Ȱ���Ͽ� ���� �������� �̵���Ű��
/// erase �Լ��� ����Ͽ� ���� �̵��� ��ƼƼ�� ����� �ٲ�.
/// </summary> 

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene(std::wstring sceneName = L"");
	~Scene() = default;

	//Scene(const Scene& scene) = delete;	
	//Scene& operator=(const Scene& scene) = delete;

	//Scene(const Scene&& scene) = delete;
	//Scene&& operator=(const Scene&& scene) = delete;

	virtual void Start();
	virtual void Initialize(std::shared_ptr<IGraphics> graphics, int count, std::shared_ptr<IManager> manager...);
	virtual void Exit();

	virtual void Update(double dt);
	virtual void FixedUpdate(double fixedTime);
	void Render(double dt);
	void DebugRender();
	void Finalize();

	void AddEntity(std::weak_ptr<Entity> entity);
	void SetEntityManager(std::weak_ptr<Entity> entity);
	void CreateAddHierarchyEntity(std::string name, std::shared_ptr<Entity> parent = nullptr);
	void CreateCloneAddEntity(std::weak_ptr<Entity> baseEntity);
	void DestroyEntity(std::weak_ptr<Entity> entity);
	void DestroyAllEntity();

	std::vector<std::shared_ptr<Entity>>& GetEntities() { return m_entities; }
	std::shared_ptr<Entity> GetEntity(std::string name);



	void SetManager(std::shared_ptr<IManager> manager);
	void SetGraphics(std::shared_ptr<IGraphics> graphics);

	const std::wstring& GetSceneName() const { return m_sceneName; }
	void SetSceneName(std::wstring sceneName) { m_sceneName = sceneName; }

	bool IsSimulating() const { return m_isSimulating; }
	bool IsPrevSimulating() const { return m_isPrevSimulating; }
	void SetSimulating(bool val);
	void SetPrevSimulating(bool val);

	bool DebugRenderButton() const { return m_isDebug; }

protected:
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<ResourceManager> m_resourceManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<IGraphics> m_graphics;

private:
	std::vector<std::shared_ptr<Entity>> m_entities; // �� ���� ������Ʈ
	std::wstring m_sceneName;

	bool m_isSimulating;
	bool m_isPrevSimulating;
	bool m_isDebug;
};

