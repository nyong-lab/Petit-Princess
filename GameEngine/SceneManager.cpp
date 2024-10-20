#include "GameEngine_pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "IGraphics.h"
#include "EventManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"

#include "Entity.h"
#include "MeshRenderer.h"
#include "UIRenderer.h"
#include "MeshFilter.h"
#include "Serialize.h"
#include "Collider.h"
#include "../GameDemo/PetitCamera.h"
#include "../GameDemo/GameManager.h"
#include "../GameDemo/UITestScript.h"
#include "../GameDemo/UIDataSyncScript.h"

//#include "../GameDemo/FoodTableScript.h"
//#include "../GameDemo/InterKeyScript.h"
//#include "../GameDemo/ShopKeeperFSM.h"

SceneManager::SceneManager()
{

}


SceneManager::~SceneManager()
{

}

void SceneManager::SetCurScene(std::shared_ptr<Scene> scene)
{
	if (m_preScene == nullptr && m_curScene == nullptr)
	{
		m_curScene = scene;
		m_preScene = scene;
		return;
	}

	m_preScene = m_curScene;
	m_curScene = scene;
}

void SceneManager::Initialize()
{
	//씬 데이터들을 읽어서 모든 씬들을 가지고 있어야한다
	//리소스 매니저가 씬을 읽어오고 가져온다 또는 씬을 만들어 뿌려준다? 
	//-> 이건 씬을 결국 리소스매니저에서 관리한다는건데 그럴거면 씬 매니저가 왜 필요함?


	//일단 testScene 생성
	/*std::shared_ptr<Scene> testScene = std::make_shared<Scene>();

	AddScene("test", testScene);
	SetCurScene(testScene);*/
}

void SceneManager::InitializeManager(std::shared_ptr<IGraphics> graphics, int count, std::shared_ptr<IManager> manager...)
{
	va_list arg;

	m_graphics = graphics;

	va_start(arg, manager);
	this->SetManager(manager);

	for (int i = 0; i < count - 1; i++)
	{
		this->SetManager((va_arg(arg, std::shared_ptr<IManager>)));
	}

	va_end(arg);

	return;
}

void SceneManager::SetManager(std::shared_ptr<IManager> manager)
{
	if (std::dynamic_pointer_cast<ResourceManager>(manager))
	{
		m_resourceManager = std::dynamic_pointer_cast<ResourceManager>(manager);
	}
	else if (std::dynamic_pointer_cast<CameraManager>(manager))
	{
		m_cameraManager = std::dynamic_pointer_cast<CameraManager>(manager);
	}
	else
	{
		//해당 매니저가 없음
		assert(true, "this manager is not exist in Entity");
	}
}

void SceneManager::Start()
{
	if (m_curScene != nullptr)
	{
		m_curScene->Start();
	}
}

void SceneManager::Update(double dt)
{
	if (m_curScene != nullptr && m_curScene == EventManager::GetInstance()->GetCurScene())
	{
		m_curScene->Update(dt);
		return;
	}

	m_curScene = EventManager::GetInstance()->GetCurScene();
	m_curScene->Start();
	m_curScene->Update(dt);
	return;
}

void SceneManager::FixedUpdate(double fixedTime)
{
	m_curScene->FixedUpdate(fixedTime);
	return;
}

void SceneManager::Finalize()
{
	for (auto scene : m_sceneList)
	{
		scene.second->Finalize();
		scene.second.reset();
	}
	m_curScene->Exit();
	m_sceneList.clear();
}

void SceneManager::AddScene(std::string name, std::shared_ptr<Scene> scene)
{
	m_sceneList.insert(std::make_pair(name, scene));
}

void SceneManager::Render(double dt)
{
	if (m_curScene != nullptr)
	{
		m_curScene->Render(dt);
	}
}

void SceneManager::DebugRender()
{
	if (m_curScene->DebugRenderButton())
	{
		if (m_curScene != nullptr)
		{
			m_curScene->DebugRender();
		}
	}
}

void SceneManager::SceneChange(std::string name)
{
	auto it = m_sceneList.find(name);

	if (it == m_sceneList.end())
	{
		assert(!"해당하는 씬이 등록되어 있지 않습니다.");
	}

	EventManager::GetInstance()->SceneChange(m_sceneList[name]);
}

void SceneManager::LoadScene(std::wstring sceneName, bool isInitializeSceneData /*= false*/)
{
	// string 으로 전환
	std::string stringScenename = StringUtils::ToString(sceneName);

	// 매니저에 등록하기 위한 씬 생성
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneName);

	const auto& iter = m_sceneList.find(stringScenename);
	if (iter != m_sceneList.end())
	{
		m_sceneList.erase(iter);
	}

	m_sceneList.insert(std::make_pair(stringScenename, scene));
	//EventManager::GetInstance()->SetCurScene(scene);
	scene->Initialize(m_graphics.lock(), 3, m_cameraManager.lock(), m_resourceManager.lock(), shared_from_this());

	// 데이터를 가져올 씬의 경로 얻기
	std::filesystem::path scenePath = PathManager::GetScenePath(sceneName);

	// 씬에 등록된 엔티티들의 데이터 경로를 얻는다.
	auto entityList = PathManager::GetSceneEntityList(scenePath);

	// 엔티티들 생성
	for (auto& entityPath : entityList)
	{
		LoadEntity(entityPath, scene, isInitializeSceneData);
	}

	// 생성한 엔티티들의 계층구조 형성
	for (auto& entityPath : entityList)
	{
		MakeEntityHierarchy(entityPath, scene);
	}
	
	MosterPointerScene(scene);
	
	if (!isInitializeSceneData)
		SceneChange(stringScenename);
	else
		scene->Start();
}

void SceneManager::LoadEntity(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene, bool isInitializeSceneData /*= false*/) const
{
	// Entity의 이름은 파일명
	std::string name = path.filename().string();
	StringUtils::RemoveSubstring(name, ".json");

	// 엔티티 이름과 동일한 이름을 가지고 있는 엔티티가 씬에 등록되어 있는지 체크한다. 
	// (병합 과정에서 데이터를 덮어 씌워 버리기에)
	std::shared_ptr<Entity> findEntity;

	// 씬 경로상에 데이터로 초기화 단계라면
	if (isInitializeSceneData)
	{
		// 모든 씬들을 순회하며 같은 이름을 가지고 있는 엔티티가 있는지 체크
		for (const auto& iter : m_sceneList)
		{
			findEntity = iter.second->GetEntity(name);

			if (findEntity != nullptr)
				break;
		}
	}

	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	entity->DestroyComponent<Transform>();
	loadScene->SetEntityManager(entity);
	unsigned long long id = entity->GetLastEntityID();

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
				entity->Deserialize(data);

				// 이미 이름이 같은 엔티티가 있었더라면 로드한 엔티티의 이름을 바꾼다.
				if (isInitializeSceneData)
				{
					if (findEntity != nullptr)
					{
						// 있다면 엔티티의 이름을 변경한다.
						std::string addID = "_" + std::to_string(id);
						entity->SetName(name + addID);

						const_cast<std::filesystem::path&>(path).replace_filename(name + addID);
					}
				}

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

					// 엔티티에 컴포넌트 추가
					entity->AddComponent(component);

					// json 정보를 토대로 Desrialize
					component->Deserialize(data);

					// 어쩔수 없는 하드코딩이다?
					// 컴포넌트가 매니저를 들고있기에..
					std::string superName = typeInfo->GetSuper()->GetName();

					if (componentTypeName == "MeshFilter")
					{
						const auto& meshFilterComponent = std::static_pointer_cast<MeshFilter>(component);
						meshFilterComponent->UpdateModelKey();
					}

					// collider, fsm, light -> trasnform, camera, graphics 에 의존하기에 start 함수를 해주는 배열에 집어넣는다. 
					// 분류해서 담아둔다.
					else if (superName == "Collider")
						components.push_back(component);

					else if (superName == "FSM")
						components.push_back(component);

					else if (typeInfo->GetSuper()->GetName() == "DirectionalLight")
						components.push_back(component);

					else if (typeInfo->GetSuper()->GetName() == "PointLight")
						components.push_back(component);
					else if (componentTypeName == "MerchantUI")
						components.push_back(component);
					else if (componentTypeName == "UIDataSyncScript")
						components.push_back(component);
					else if (componentTypeName == "UITestScript")
						components.push_back(component);
				}
			}
		}

		// 콜라이더, fsm - 이벤트 매니저에 등록해서 씬 체인지가 되면 
		// start 함수 호출하게끔 구현 
		if (!isInitializeSceneData)
			EventManager::GetInstance()->NextFrameStartComponents(components);

		// 다만 프로그램 실행 시 씬들을 로드하는 경우에는 
		// 이벤트매니저에 등록되어있는 씬이라던가 주기가 꼬여있는 상황이므로
		// 초기에 씬 초기화인지 여부에 따라서 바로 콜라이더에 씬을 바로 등록시켜준다.
		else
		{
			for (const auto& component : components)
			{
				// 콜라이더인 경우 scene을 새로 배정 해줘야한다.
				if (dynamic_pointer_cast<Collider>(component))
				{
					static_pointer_cast<Collider>(component)->SetScene(loadScene);

					// FSM은 엔진에서 초기에 스타트를 불러준다. 
					component->Start();
				}
			}
		}

		/// 씬에 엔티티 등록
		loadScene->AddEntity(entity);
	}
	else
	{
		assert(!"Entity.json 읽기 실패 - 동일");
	}
}

void SceneManager::LoadPrefab(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const
{
	// Entity의 이름은 파일명
	std::string name = path.filename().string();
	StringUtils::RemoveSubstring(name, ".json");

	// 프리팹 엔티티 이름과 동일한 이름을 가지고 있는 엔티티가 씬에 등록되어 있는지 체크한다.
	std::shared_ptr<Entity> findEntity = m_curScene->GetEntity(name);

	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);

	unsigned long long id = entity->GetLastEntityID();

	entity->DestroyComponent<Transform>();
	loadScene->SetEntityManager(entity);

	entity->SetNewID();

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

		// 프리팹 계층구조를 위한 스태틱 변수
		static std::string childContainName = "Child Entity ";
		static int childNum = 1;

		// 프리팹 계층구조를 형성하기 위한 vector
		std::vector<std::shared_ptr<Entity>> prefabEntities;
		prefabEntities.push_back(entity);

		// 타입 이름과 (컴포넌트 또는 엔티티) 그에 해당하는 데이타정보 순회
		for (const auto& [typeName, data] : jsonData.items())
		{
			// 인덱스가 엔티티인 경우
			if (typeName == "Entity")
			{
				entity->Deserialize(data);

				// 이미 이름이 같은 엔티티가 있었더라면 로드한 프리팹의 이름을 바꾼다.
				if (findEntity != nullptr)
				{
					// 있다면 프리팹으로 만들어질 엔티티의 이름을 변경한다.
					std::string addID = "_" + std::to_string(id);
					entity->SetName(name + addID);
				}

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

					// 엔티티에 컴포넌트 추가
					entity->AddComponent(component);

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

					//// 콜라이더와 fsm 구조상 start 함수와 트랜스폼에 의존하고있기에 
					//// 분류해서 담아둔다.
					//else if (superName == "Collider")
					//	components.push_back(component);

					//else if (superName == "FSM")
					//	components.push_back(component);
					//else if (typeInfo->GetSuper()->GetName() == "DirectionalLight")
					//	components.push_back(component);

					//else if (typeInfo->GetSuper()->GetName() == "PointLight")
					//	components.push_back(component);
					//else if (componentTypeName == "MerchantUI")
					//	components.push_back(component);
					//else if (componentTypeName == "UIDataSyncScript")
					//	components.push_back(component);

					components.push_back(component);
				}
			}

			// 현재 읽고 있는 데이터가 프리팹 데이터라는 뜻이고,
			// 프리팹의 계층구조는 더 있을 수도 있으니 체크해서 엔티티를 생성하고 등록해야한다.
			if (typeName == childContainName + std::to_string(childNum++))
			{
				DeserializePrefabChild(typeName, data, loadScene, prefabEntities);
			}
		}

		// 다음 프리팹을 위한 스태틱 변수 초기화
		childNum = 1;

		// 프리팹 엔티티들을 순회한다. (계층구조가 형성이 안된 계층구조 엔티티들)
		// 사이즈가 1보다 크다면 계층구조로 되어있는 프리팹이라는 뜻
		if (prefabEntities.size() > 1)
		{
			for (const auto& prefabEntity : prefabEntities)
			{
				//// 루트 엔티티라면 패스한다.
				//if (!prefabEntity->GetComponent<Transform>()->HasParent())
				//	continue;

				// 부모가 아니라면 자신의 프리팹 아이디에 자신의 고유 아이디를 뺀다.
				// 이 값은 부모의 프리팹 아이디
				unsigned long long parentPrefabID = prefabEntity->GetPrefabID() - prefabEntity->GetAddressID();

				// 한번 더 계층구조 형성 벡터에 저장되어있는 엔티티들을 순회하며
				// 앞서 구해둔 부모의 프리팹 아이디와 동일한 아이디를 찾아낸다.
				// 찾아낸다면 그 엔티티가 즉 자신의 부모
				for (const auto& findEntity : prefabEntities)
				{
					// 나 자신과 비교한다면 스킵
					if (prefabEntity == findEntity)
						continue;

					unsigned long long findEntityPrefabID = findEntity->GetPrefabID();

					if (parentPrefabID == findEntity->GetPrefabID())
					{
						prefabEntity->GetComponent<Transform>()->SetPrefabParent(findEntity->GetComponent<Transform>());
					}
				}
			}
		}

		//// 콜라이더, fsm - 이벤트 매니저에 등록해서 씬 체인지가 되면 
		//// start 함수 호출하게끔 구현 
		////EventManager::GetInstance()->NextFrameStartComponents(components);

		

		loadScene->AddEntity(entity);

		for (const auto& component : components)
		{
			component->Start();
		}
	}
	else
	{
		assert(!"Entity.json 읽기 실패 - 동일");
	}
}


/// <param name="mergeSceneNameList">병합시킬 씬 리스트</param>
/// <param name="scene">병합 하는 도화지 씬</param>
void SceneManager::MergeScenes(std::vector<std::string> mergeSceneNameList, std::shared_ptr<Scene> scene)
{
	for (const auto& sceneName : mergeSceneNameList)
	{
		// 데이터를 가져올 씬의 경로 얻기
		std::filesystem::path sceneDataPath = PathManager::GetScenePath(StringUtils::ToWString(sceneName));

		// 씬에 등록된 엔티티들의 데이터 경로를 얻는다.
		auto entityList = PathManager::GetSceneEntityList(sceneDataPath);

		// 엔티티들 생성
		for (auto& entityPath : entityList)
		{
			LoadEntity(entityPath, scene);
		}

		// 생성한 엔티티들의 계층구조 형성
		for (auto& entityPath : entityList)
		{
			MakeEntityHierarchy(entityPath, scene);
		}

		//// 상점 관련 포인터 연결
		//StorePointerConnection(scene);
	}

	// 씬 데이터 저장 
	std::filesystem::path scenePath = PathManager::GetSceneDataPath();
	scenePath += "/";
	scenePath += scene->GetSceneName();

	Serialize::SerializeSceneInformation(scene, scenePath);
}

void SceneManager::MosterPointerScene(std::shared_ptr<Scene> scene)
{
	/// 씬 이름 체크 (실제 플레이 하는 씬)
	if (scene->GetSceneName() == L"RealStage" || scene->GetSceneName() == L"STAGE1")
	{
		// 게임매니저가 있는지 체크하고
		const auto& gameManager = scene->GetEntity("GameManager");
		if (gameManager)
		{
			// 플레이어가 있는지 체크하고
			const auto& player = scene->GetEntity("Player");

			if (player)
			{
				// 둘 다 있으면 게임매니저에 연결
				const auto& gameManagerComponent = gameManager->GetComponent<GameManager>();
				if (gameManagerComponent)
				{
					gameManagerComponent->SetPlayer(player);
				}
				else
					assert(!"게임매니저에 게임매니저스크립트 추가하세요 - 동일");

				// 쁘띠 카메라도 연결
				const auto& cameraComponent = player->GetComponent<PetitCamera>();
				if (cameraComponent)
				{
					cameraComponent->SetTarget(player);
				}
				else
					assert(!"플레이어에 쁘띠카메라 컴포넌트 추가하세요 - 동일");

				// 조준 게이지바도 연결
				const auto& aimmingUI = scene->GetEntity("AimmingUI");
				if (aimmingUI)
				{
					aimmingUI->SetRender(false);
					const auto& uiScript = aimmingUI->GetComponent<UITestScript>();
					if (uiScript)
					{
						uiScript->SetPlayerPointer(player);
					}
					else
						assert(!"조준 게이지바에 ui스크립트 넣으세요 - 동일");
				}
				else
					assert(!"조준 게이지바 이름 AimmingUI 넣으세요 - 동일");

				// 데이터 싱크 스크립트에 미니맵과 플레이어 연결
				const auto& uiDataSyncEntity = scene->GetEntity("DataSyncEntity");
				if (uiDataSyncEntity)
				{
					const auto& minimapCurposEntity = scene->GetEntity("MinimapCurpos");
					if (minimapCurposEntity)
					{
						const auto& uiDataSyncScript = uiDataSyncEntity->GetComponent<UIDataSyncScript>();
						if (uiDataSyncScript)
						{
							// 자식 넣는 순서 중요..
							uiDataSyncEntity->GetComponent<Transform>()->SetChild(player->GetComponent<Transform>());
							uiDataSyncEntity->GetComponent<Transform>()->SetChild(minimapCurposEntity->GetComponent<Transform>());
						}
						else
							assert(!"미니맵 데이터 엔티티에 데이터싱크스트립트를 넣으세요- 동일");
					}
					else
						assert(!"미니맵 포지션 엔티티에 DataSyncEntity을 이름으로 지정하세요 - 동일");
				}
				else
					assert(!"미니맵 데이터 엔티티에 DataSyncEntity을 이름으로 지정하세요 - 동일");
			}
			else
				assert(!"플레이어에 이름 Player 넣으세요 - 동일");
		}
		else
			assert(!"게임매니저에 GameManager 이름 넣으세요 - 동일");
	}
}

void SceneManager::MakeEntityHierarchy(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const
{
	// Entity의 이름은 파일명
	std::string name = path.filename().string();
	StringUtils::RemoveSubstring(name, ".json");

	// 씬에 등록된 엔티티에서 현재 json파일에 해당하는 엔티티를 가져온다.
	std::shared_ptr<Entity> entity = loadScene->GetEntity(name);

	if (entity == nullptr)
		assert(!"씬 병합 과정에서 동일한 엔티티의 이름 -> 이름 변경 로직 잘못 됐다? - 동일");

	// 부모가 이미 세팅되어있다면 리턴 
	// 혹여나 자식이 있더라도 자식쪽에서 계층구조를 형성시켜 준다.
	const auto& transform = entity->GetComponent<Transform>();
	if (transform->HasParent())
		return;

	std::ifstream readData(path);

	if (readData.is_open())
	{
		// 파일의 시작 지점 부터 끝 지점을 가리키는 반복자를 만들고, 
		// 이 반복자들 사이의 내용을 문자열로 저장
		std::string fileContent((std::istreambuf_iterator<char>(readData)),
			std::istreambuf_iterator<char>());

		// JSON 파싱
		nlohmann::json jsonData = nlohmann::json::parse(fileContent);

		if (jsonData.contains("Parent"))
		{
			// 현재 씬에서 현재 엔티티에 부모를 꺼내온다.
			std::shared_ptr<Entity> parentEntity = loadScene->GetEntity(jsonData["Parent"]);

			if (parentEntity == nullptr)
				assert(!"계층구조 시리얼라이즈 과정에서 문제가 있습니다.");

			// 계층구조 형성.
			transform->EditorLoadSetParent(parentEntity->GetComponent<Transform>());
		}
	}
}

void SceneManager::DeserializePrefabChild(const std::string containName, const nlohmann::json jsonData, std::shared_ptr<Scene> loadScene, std::vector<std::shared_ptr<Entity>>& prefabEntities) const
{
	// Entity의 이름 등록
	std::string name = jsonData["m_name"];

	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);

	// 엔티티 중복 방지 아이디
	unsigned long long id = entity->GetLastEntityID();

	entity->DestroyComponent<Transform>();
	loadScene->SetEntityManager(entity);

	// 프리팹 계층구조 형성에 필요한 벡터에 담아둔다.
	prefabEntities.push_back(entity);

	entity->Deserialize(jsonData);

	// 엔티티 이름 중복 방지 (프리팹 계층구조에도 신경 써야 함)
	// 이름이 중복되어있으면 추후 씬을 저장할 때 데이터가 덮어 씌워질 수 있다.
	for (const auto& findEntity : loadScene->GetEntities())
	{
		if (name == findEntity->GetName())
		{
			std::string addID = "_" + std::to_string(id);
			entity->SetName(entity->GetName() + addID);

			break;
		}
	}

	// 리플렉션 데이터를 통하여 컴포넌트를 추가한다.
	TypeInfoManager* manager = TypeInfoManager::GetInstance();

	// 콜라이더, fsm 같은 경우에는 start 함수에 의존하고 있다.
	// 또한, 콜라이더는 트랜스폼 컴포넌트에도 의존하고 있다.
	// 그러므로 vector에 담아두고 start 함수 호출
	std::vector<std::shared_ptr<Component>> components;

	// 타입 이름과 (컴포넌트 또는 엔티티) 그에 해당하는 데이타정보 순회
	for (const auto& [typeName, data] : jsonData.items())
	{
		if (typeName == "Components")
		{
			for (const auto& [componentTypeName, data] : data.items())
			{
				// TypeInfoManager에 등록되어 있는 이름은 class 가 붙어있으므로 추가한다.
				std::string fullTypeName = "class " + componentTypeName;

				// 타입 이름에 해당하는 인덱스를 가져온다.
				std::type_index index = manager->GetTypeIndex(fullTypeName);

				// 매니저에서 인덱스를 활용하여 타입 정보 객체를 가져온다.
				const TypeInfo* typeInfo = manager->GetTypeInfo(index);

				// 객체에 등록된 기본 생성자를 호출하여 컴포넌트를 만든다.
				std::shared_ptr<Component> component = typeInfo->InvokdDefaultConstructor();

				entity->AddComponent(component);

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
				else if (componentTypeName == "MerchantUI")
					components.push_back(component);
			}
		}
	}

	// 콜라이더, fsm - 이벤트 매니저에 등록해서 씬 체인지가 되면 
	// start 함수 호출하게끔 구현 
	EventManager::GetInstance()->NextFrameStartComponents(components);

	loadScene->AddEntity(entity);
}

//void SceneManager::StorePointerConnection(std::shared_ptr<Scene> scene)
//{
//	// 상점과 관련된 친구들의 포인터 연결을 하기 위함.
//	std::vector<std::shared_ptr<ShopKeeperFSM>> shopKeeperFSMs;
//	std::vector<std::shared_ptr<FoodTableScript>> foodTableScripts;
//	std::vector<std::shared_ptr<InterKeyScript>> interKeyScripts;
//	for (const auto& entity : scene->GetEntities())
//	{
//		if (entity->HasComponent<ShopKeeperFSM>())
//		{
//			shopKeeperFSMs.push_back(entity->GetComponent<ShopKeeperFSM>());
//		}
//		if (entity->HasComponent<FoodTableScript>())
//		{
//			foodTableScripts.push_back(entity->GetComponent<FoodTableScript>());
//		}
//		if (entity->HasComponent<InterKeyScript>())
//		{
//			interKeyScripts.push_back(entity->GetComponent<InterKeyScript>());
//		}
//	}
//
//	// 상인과 푸드 테이블 연결
//	for (const auto& shopKeeperFSM : shopKeeperFSMs)
//	{
//		for (const auto& foodTableScript : foodTableScripts)
//		{
//			// 상인FSM과 푸드 테이블의 부모가 같다면 동일한 음식가게의 자식이라는 뜻
//			if (shopKeeperFSM->GetTransform()->GetParent().lock() == foodTableScript->GetTransform()->GetParent().lock())
//			{
//				foodTableScript->SetPointerSense(shopKeeperFSM->GetPointerSense());
//				break;
//			}
//		}
//	}
//
//	// 푸드 테이블과 상호작용 키 연결
//	for (const auto& foodTableScript : foodTableScripts)
//	{
//		for (const auto& interKeyScript : interKeyScripts)
//		{
//			// 푸드 테이블과 인터키의 부모가 같다면 동일한 음식가게의 자식이라는 뜻
//			if (foodTableScript->GetTransform()->GetParent().lock() == interKeyScript->GetTransform()->GetParent().lock())
//			{
//				interKeyScript->m_script = foodTableScript;
//				break;
//			}
//		}
//	}
//}

std::shared_ptr<Scene> SceneManager::GetScene(std::string name)
{
	/*
	if (find(m_sceneList.begin(), m_sceneList.end(), name) == m_sceneList.end())
	{
		return;
	}*/

	return m_sceneList[name];
}
