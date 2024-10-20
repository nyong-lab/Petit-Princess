#pragma once
#include <vector>

#include <stdio.h> //가변인자 사용
#include <stdarg.h> //가변인자 사용

class Entity;
class IGraphics;

class IManager;
class SceneManager;
class ResourceManager;
class CameraManager;
class CollisionManager;


/// <summary>
/// 인 게임에서 현재 돌아가고있는 장면
/// 여기엔 유저가 엔티티만 넣어주고 업데이트 렌더만 해야한다고 생각함
/// 
/// 24.01.28 [수정] 김동일
/// 1. 트랜스폼 업데이트가 중복적으로 호출되지 않도록 설정해둔 
///		bool 값 isUpdated 를 다시 false로 설정하도록 구현
/// 2. 엔티티를 상속받는 클래스 (TextureBox) 가 늘어날 경우 Clone 버튼에 대응하기 위한 
///		하드코딩 구현. (CreateCloneAddEntity 함수)
/// 24.01.28 [수정] 손서희 -> DebugRender 추가
///                      -> FixedUpdate 추가
/// 
/// 24.02.05 [수정] 김동일 -> 디시리얼라이즈 과정에서 트랜스폼은 엔티티가 생성되자마자 갖고있고..
///							그래서 그 트랜스폼을 삭제를하고 추가하자니 또 약간 꼬여가지고 
///							엔티티에 괴물셋셋셋만 하고 씬에 등록은 하지않는 함수를 만들었다.
///							SetEntityManager();
/// 
///	24.02.06 [수정] 김동일 -> DestroyEntity 자식이 여러개 있을 때 finalize 하면서 children이 줄어드는데 
/// range for base for문이 줄어든 children에 대응하지 못하고 empty 값을 가져와서 터지는 현상이 있었다.
/// 때문에 Destroy 함수 내부를 바꿔서 자식의 엔티티를 복사해서 벡터에 담아두고 엔티티를 지울 때 remove_if 를 활용하여 벡터 끝쪽으로 이동시키고
/// erase 함수를 사용하여 끝에 이동된 엔티티를 지우게 바꿈.
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
	std::vector<std::shared_ptr<Entity>> m_entities; // 씬 내부 오브젝트
	std::wstring m_sceneName;

	bool m_isSimulating;
	bool m_isPrevSimulating;
	bool m_isDebug;
};

