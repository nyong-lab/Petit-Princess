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
	//�� �����͵��� �о ��� ������ ������ �־���Ѵ�
	//���ҽ� �Ŵ����� ���� �о���� �����´� �Ǵ� ���� ����� �ѷ��ش�? 
	//-> �̰� ���� �ᱹ ���ҽ��Ŵ������� �����Ѵٴ°ǵ� �׷��Ÿ� �� �Ŵ����� �� �ʿ���?


	//�ϴ� testScene ����
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
		//�ش� �Ŵ����� ����
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
		assert(!"�ش��ϴ� ���� ��ϵǾ� ���� �ʽ��ϴ�.");
	}

	EventManager::GetInstance()->SceneChange(m_sceneList[name]);
}

void SceneManager::LoadScene(std::wstring sceneName, bool isInitializeSceneData /*= false*/)
{
	// string ���� ��ȯ
	std::string stringScenename = StringUtils::ToString(sceneName);

	// �Ŵ����� ����ϱ� ���� �� ����
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneName);

	const auto& iter = m_sceneList.find(stringScenename);
	if (iter != m_sceneList.end())
	{
		m_sceneList.erase(iter);
	}

	m_sceneList.insert(std::make_pair(stringScenename, scene));
	//EventManager::GetInstance()->SetCurScene(scene);
	scene->Initialize(m_graphics.lock(), 3, m_cameraManager.lock(), m_resourceManager.lock(), shared_from_this());

	// �����͸� ������ ���� ��� ���
	std::filesystem::path scenePath = PathManager::GetScenePath(sceneName);

	// ���� ��ϵ� ��ƼƼ���� ������ ��θ� ��´�.
	auto entityList = PathManager::GetSceneEntityList(scenePath);

	// ��ƼƼ�� ����
	for (auto& entityPath : entityList)
	{
		LoadEntity(entityPath, scene, isInitializeSceneData);
	}

	// ������ ��ƼƼ���� �������� ����
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
	// Entity�� �̸��� ���ϸ�
	std::string name = path.filename().string();
	StringUtils::RemoveSubstring(name, ".json");

	// ��ƼƼ �̸��� ������ �̸��� ������ �ִ� ��ƼƼ�� ���� ��ϵǾ� �ִ��� üũ�Ѵ�. 
	// (���� �������� �����͸� ���� ���� �����⿡)
	std::shared_ptr<Entity> findEntity;

	// �� ��λ� �����ͷ� �ʱ�ȭ �ܰ���
	if (isInitializeSceneData)
	{
		// ��� ������ ��ȸ�ϸ� ���� �̸��� ������ �ִ� ��ƼƼ�� �ִ��� üũ
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
		// ������ ���� ���� ���� �� ������ ����Ű�� �ݺ��ڸ� �����, 
		// �� �ݺ��ڵ� ������ ������ ���ڿ��� ����
		std::string fileContent((std::istreambuf_iterator<char>(readData)),
			std::istreambuf_iterator<char>());

		// JSON �Ľ�
		nlohmann::json jsonData = nlohmann::json::parse(fileContent);

		// ���÷��� �����͸� ���Ͽ� ������Ʈ�� �߰��Ѵ�.
		TypeInfoManager* manager = TypeInfoManager::GetInstance();

		// �ݶ��̴�, fsm ���� ��쿡�� start �Լ��� �����ϰ� �ִ�.
		// ����, �ݶ��̴��� Ʈ������ ������Ʈ���� �����ϰ� �ִ�.
		// �׷��Ƿ� vector�� ��Ƶΰ� start �Լ� ȣ��
		std::vector<std::shared_ptr<Component>> components;

		// Ÿ�� �̸��� (������Ʈ �Ǵ� ��ƼƼ) �׿� �ش��ϴ� ����Ÿ���� ��ȸ
		for (const auto& [typeName, data] : jsonData.items())
		{
			// �ε����� ��ƼƼ�� ���
			if (typeName == "Entity")
			{
				entity->Deserialize(data);

				// �̹� �̸��� ���� ��ƼƼ�� �־������ �ε��� ��ƼƼ�� �̸��� �ٲ۴�.
				if (isInitializeSceneData)
				{
					if (findEntity != nullptr)
					{
						// �ִٸ� ��ƼƼ�� �̸��� �����Ѵ�.
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

					// TypeInfoManager�� ��ϵǾ� �ִ� �̸��� class �� �پ������Ƿ� �߰��Ѵ�.
					std::string fullTypeName = "class " + typeName;

					// Ÿ�� �̸��� �ش��ϴ� �ε����� �����´�.
					std::type_index index = manager->GetTypeIndex(fullTypeName);

					// �Ŵ������� �ε����� Ȱ���Ͽ� Ÿ�� ���� ��ü�� �����´�.
					const TypeInfo* typeInfo = manager->GetTypeInfo(index);

					// ��ü�� ��ϵ� �⺻ �����ڸ� ȣ���Ͽ� ������Ʈ�� �����.
					std::shared_ptr<Component> component = typeInfo->InvokdDefaultConstructor();

					// ��ƼƼ�� ������Ʈ �߰�
					entity->AddComponent(component);

					// json ������ ���� Desrialize
					component->Deserialize(data);

					// ��¿�� ���� �ϵ��ڵ��̴�?
					// ������Ʈ�� �Ŵ����� ����ֱ⿡..
					std::string superName = typeInfo->GetSuper()->GetName();

					if (componentTypeName == "MeshFilter")
					{
						const auto& meshFilterComponent = std::static_pointer_cast<MeshFilter>(component);
						meshFilterComponent->UpdateModelKey();
					}

					// collider, fsm, light -> trasnform, camera, graphics �� �����ϱ⿡ start �Լ��� ���ִ� �迭�� ����ִ´�. 
					// �з��ؼ� ��Ƶд�.
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

		// �ݶ��̴�, fsm - �̺�Ʈ �Ŵ����� ����ؼ� �� ü������ �Ǹ� 
		// start �Լ� ȣ���ϰԲ� ���� 
		if (!isInitializeSceneData)
			EventManager::GetInstance()->NextFrameStartComponents(components);

		// �ٸ� ���α׷� ���� �� ������ �ε��ϴ� ��쿡�� 
		// �̺�Ʈ�Ŵ����� ��ϵǾ��ִ� ���̶���� �ֱⰡ �����ִ� ��Ȳ�̹Ƿ�
		// �ʱ⿡ �� �ʱ�ȭ���� ���ο� ���� �ٷ� �ݶ��̴��� ���� �ٷ� ��Ͻ����ش�.
		else
		{
			for (const auto& component : components)
			{
				// �ݶ��̴��� ��� scene�� ���� ���� ������Ѵ�.
				if (dynamic_pointer_cast<Collider>(component))
				{
					static_pointer_cast<Collider>(component)->SetScene(loadScene);

					// FSM�� �������� �ʱ⿡ ��ŸƮ�� �ҷ��ش�. 
					component->Start();
				}
			}
		}

		/// ���� ��ƼƼ ���
		loadScene->AddEntity(entity);
	}
	else
	{
		assert(!"Entity.json �б� ���� - ����");
	}
}

void SceneManager::LoadPrefab(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const
{
	// Entity�� �̸��� ���ϸ�
	std::string name = path.filename().string();
	StringUtils::RemoveSubstring(name, ".json");

	// ������ ��ƼƼ �̸��� ������ �̸��� ������ �ִ� ��ƼƼ�� ���� ��ϵǾ� �ִ��� üũ�Ѵ�.
	std::shared_ptr<Entity> findEntity = m_curScene->GetEntity(name);

	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);

	unsigned long long id = entity->GetLastEntityID();

	entity->DestroyComponent<Transform>();
	loadScene->SetEntityManager(entity);

	entity->SetNewID();

	std::ifstream readData(path);

	if (readData.is_open())
	{
		// ������ ���� ���� ���� �� ������ ����Ű�� �ݺ��ڸ� �����, 
		// �� �ݺ��ڵ� ������ ������ ���ڿ��� ����
		std::string fileContent((std::istreambuf_iterator<char>(readData)),
			std::istreambuf_iterator<char>());

		// JSON �Ľ�
		nlohmann::json jsonData = nlohmann::json::parse(fileContent);

		// ���÷��� �����͸� ���Ͽ� ������Ʈ�� �߰��Ѵ�.
		TypeInfoManager* manager = TypeInfoManager::GetInstance();

		// �ݶ��̴�, fsm ���� ��쿡�� start �Լ��� �����ϰ� �ִ�.
		// ����, �ݶ��̴��� Ʈ������ ������Ʈ���� �����ϰ� �ִ�.
		// �׷��Ƿ� vector�� ��Ƶΰ� start �Լ� ȣ��
		std::vector<std::shared_ptr<Component>> components;

		// ������ ���������� ���� ����ƽ ����
		static std::string childContainName = "Child Entity ";
		static int childNum = 1;

		// ������ ���������� �����ϱ� ���� vector
		std::vector<std::shared_ptr<Entity>> prefabEntities;
		prefabEntities.push_back(entity);

		// Ÿ�� �̸��� (������Ʈ �Ǵ� ��ƼƼ) �׿� �ش��ϴ� ����Ÿ���� ��ȸ
		for (const auto& [typeName, data] : jsonData.items())
		{
			// �ε����� ��ƼƼ�� ���
			if (typeName == "Entity")
			{
				entity->Deserialize(data);

				// �̹� �̸��� ���� ��ƼƼ�� �־������ �ε��� �������� �̸��� �ٲ۴�.
				if (findEntity != nullptr)
				{
					// �ִٸ� ���������� ������� ��ƼƼ�� �̸��� �����Ѵ�.
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

					// TypeInfoManager�� ��ϵǾ� �ִ� �̸��� class �� �پ������Ƿ� �߰��Ѵ�.
					std::string fullTypeName = "class " + typeName;

					// Ÿ�� �̸��� �ش��ϴ� �ε����� �����´�.
					std::type_index index = manager->GetTypeIndex(fullTypeName);

					// �Ŵ������� �ε����� Ȱ���Ͽ� Ÿ�� ���� ��ü�� �����´�.
					const TypeInfo* typeInfo = manager->GetTypeInfo(index);

					// ��ü�� ��ϵ� �⺻ �����ڸ� ȣ���Ͽ� ������Ʈ�� �����.
					std::shared_ptr<Component> component = typeInfo->InvokdDefaultConstructor();

					// ��ƼƼ�� ������Ʈ �߰�
					entity->AddComponent(component);

					// json ������ ���� Deserialize
					component->Deserialize(data);

					// ��¿�� ���� �ϵ��ڵ��̴�?
					// ������Ʈ�� �Ŵ����� ����ֱ⿡..
					std::string superName = typeInfo->GetSuper()->GetName();

					if (componentTypeName == "MeshFilter")
					{
						const auto& meshFilterComponent = std::static_pointer_cast<MeshFilter>(component);
						meshFilterComponent->UpdateModelKey();
					}

					//// �ݶ��̴��� fsm ������ start �Լ��� Ʈ�������� �����ϰ��ֱ⿡ 
					//// �з��ؼ� ��Ƶд�.
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

			// ���� �а� �ִ� �����Ͱ� ������ �����Ͷ�� ���̰�,
			// �������� ���������� �� ���� ���� ������ üũ�ؼ� ��ƼƼ�� �����ϰ� ����ؾ��Ѵ�.
			if (typeName == childContainName + std::to_string(childNum++))
			{
				DeserializePrefabChild(typeName, data, loadScene, prefabEntities);
			}
		}

		// ���� �������� ���� ����ƽ ���� �ʱ�ȭ
		childNum = 1;

		// ������ ��ƼƼ���� ��ȸ�Ѵ�. (���������� ������ �ȵ� �������� ��ƼƼ��)
		// ����� 1���� ũ�ٸ� ���������� �Ǿ��ִ� �������̶�� ��
		if (prefabEntities.size() > 1)
		{
			for (const auto& prefabEntity : prefabEntities)
			{
				//// ��Ʈ ��ƼƼ��� �н��Ѵ�.
				//if (!prefabEntity->GetComponent<Transform>()->HasParent())
				//	continue;

				// �θ� �ƴ϶�� �ڽ��� ������ ���̵� �ڽ��� ���� ���̵� ����.
				// �� ���� �θ��� ������ ���̵�
				unsigned long long parentPrefabID = prefabEntity->GetPrefabID() - prefabEntity->GetAddressID();

				// �ѹ� �� �������� ���� ���Ϳ� ����Ǿ��ִ� ��ƼƼ���� ��ȸ�ϸ�
				// �ռ� ���ص� �θ��� ������ ���̵�� ������ ���̵� ã�Ƴ���.
				// ã�Ƴ��ٸ� �� ��ƼƼ�� �� �ڽ��� �θ�
				for (const auto& findEntity : prefabEntities)
				{
					// �� �ڽŰ� ���Ѵٸ� ��ŵ
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

		//// �ݶ��̴�, fsm - �̺�Ʈ �Ŵ����� ����ؼ� �� ü������ �Ǹ� 
		//// start �Լ� ȣ���ϰԲ� ���� 
		////EventManager::GetInstance()->NextFrameStartComponents(components);

		

		loadScene->AddEntity(entity);

		for (const auto& component : components)
		{
			component->Start();
		}
	}
	else
	{
		assert(!"Entity.json �б� ���� - ����");
	}
}


/// <param name="mergeSceneNameList">���ս�ų �� ����Ʈ</param>
/// <param name="scene">���� �ϴ� ��ȭ�� ��</param>
void SceneManager::MergeScenes(std::vector<std::string> mergeSceneNameList, std::shared_ptr<Scene> scene)
{
	for (const auto& sceneName : mergeSceneNameList)
	{
		// �����͸� ������ ���� ��� ���
		std::filesystem::path sceneDataPath = PathManager::GetScenePath(StringUtils::ToWString(sceneName));

		// ���� ��ϵ� ��ƼƼ���� ������ ��θ� ��´�.
		auto entityList = PathManager::GetSceneEntityList(sceneDataPath);

		// ��ƼƼ�� ����
		for (auto& entityPath : entityList)
		{
			LoadEntity(entityPath, scene);
		}

		// ������ ��ƼƼ���� �������� ����
		for (auto& entityPath : entityList)
		{
			MakeEntityHierarchy(entityPath, scene);
		}

		//// ���� ���� ������ ����
		//StorePointerConnection(scene);
	}

	// �� ������ ���� 
	std::filesystem::path scenePath = PathManager::GetSceneDataPath();
	scenePath += "/";
	scenePath += scene->GetSceneName();

	Serialize::SerializeSceneInformation(scene, scenePath);
}

void SceneManager::MosterPointerScene(std::shared_ptr<Scene> scene)
{
	/// �� �̸� üũ (���� �÷��� �ϴ� ��)
	if (scene->GetSceneName() == L"RealStage" || scene->GetSceneName() == L"STAGE1")
	{
		// ���ӸŴ����� �ִ��� üũ�ϰ�
		const auto& gameManager = scene->GetEntity("GameManager");
		if (gameManager)
		{
			// �÷��̾ �ִ��� üũ�ϰ�
			const auto& player = scene->GetEntity("Player");

			if (player)
			{
				// �� �� ������ ���ӸŴ����� ����
				const auto& gameManagerComponent = gameManager->GetComponent<GameManager>();
				if (gameManagerComponent)
				{
					gameManagerComponent->SetPlayer(player);
				}
				else
					assert(!"���ӸŴ����� ���ӸŴ�����ũ��Ʈ �߰��ϼ��� - ����");

				// �ڶ� ī�޶� ����
				const auto& cameraComponent = player->GetComponent<PetitCamera>();
				if (cameraComponent)
				{
					cameraComponent->SetTarget(player);
				}
				else
					assert(!"�÷��̾ �ڶ�ī�޶� ������Ʈ �߰��ϼ��� - ����");

				// ���� �������ٵ� ����
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
						assert(!"���� �������ٿ� ui��ũ��Ʈ �������� - ����");
				}
				else
					assert(!"���� �������� �̸� AimmingUI �������� - ����");

				// ������ ��ũ ��ũ��Ʈ�� �̴ϸʰ� �÷��̾� ����
				const auto& uiDataSyncEntity = scene->GetEntity("DataSyncEntity");
				if (uiDataSyncEntity)
				{
					const auto& minimapCurposEntity = scene->GetEntity("MinimapCurpos");
					if (minimapCurposEntity)
					{
						const auto& uiDataSyncScript = uiDataSyncEntity->GetComponent<UIDataSyncScript>();
						if (uiDataSyncScript)
						{
							// �ڽ� �ִ� ���� �߿�..
							uiDataSyncEntity->GetComponent<Transform>()->SetChild(player->GetComponent<Transform>());
							uiDataSyncEntity->GetComponent<Transform>()->SetChild(minimapCurposEntity->GetComponent<Transform>());
						}
						else
							assert(!"�̴ϸ� ������ ��ƼƼ�� �����ͽ�ũ��Ʈ��Ʈ�� ��������- ����");
					}
					else
						assert(!"�̴ϸ� ������ ��ƼƼ�� DataSyncEntity�� �̸����� �����ϼ��� - ����");
				}
				else
					assert(!"�̴ϸ� ������ ��ƼƼ�� DataSyncEntity�� �̸����� �����ϼ��� - ����");
			}
			else
				assert(!"�÷��̾ �̸� Player �������� - ����");
		}
		else
			assert(!"���ӸŴ����� GameManager �̸� �������� - ����");
	}
}

void SceneManager::MakeEntityHierarchy(const std::filesystem::path& path, std::shared_ptr<Scene> loadScene) const
{
	// Entity�� �̸��� ���ϸ�
	std::string name = path.filename().string();
	StringUtils::RemoveSubstring(name, ".json");

	// ���� ��ϵ� ��ƼƼ���� ���� json���Ͽ� �ش��ϴ� ��ƼƼ�� �����´�.
	std::shared_ptr<Entity> entity = loadScene->GetEntity(name);

	if (entity == nullptr)
		assert(!"�� ���� �������� ������ ��ƼƼ�� �̸� -> �̸� ���� ���� �߸� �ƴ�? - ����");

	// �θ� �̹� ���õǾ��ִٸ� ���� 
	// Ȥ���� �ڽ��� �ִ��� �ڽ��ʿ��� ���������� �������� �ش�.
	const auto& transform = entity->GetComponent<Transform>();
	if (transform->HasParent())
		return;

	std::ifstream readData(path);

	if (readData.is_open())
	{
		// ������ ���� ���� ���� �� ������ ����Ű�� �ݺ��ڸ� �����, 
		// �� �ݺ��ڵ� ������ ������ ���ڿ��� ����
		std::string fileContent((std::istreambuf_iterator<char>(readData)),
			std::istreambuf_iterator<char>());

		// JSON �Ľ�
		nlohmann::json jsonData = nlohmann::json::parse(fileContent);

		if (jsonData.contains("Parent"))
		{
			// ���� ������ ���� ��ƼƼ�� �θ� �����´�.
			std::shared_ptr<Entity> parentEntity = loadScene->GetEntity(jsonData["Parent"]);

			if (parentEntity == nullptr)
				assert(!"�������� �ø�������� �������� ������ �ֽ��ϴ�.");

			// �������� ����.
			transform->EditorLoadSetParent(parentEntity->GetComponent<Transform>());
		}
	}
}

void SceneManager::DeserializePrefabChild(const std::string containName, const nlohmann::json jsonData, std::shared_ptr<Scene> loadScene, std::vector<std::shared_ptr<Entity>>& prefabEntities) const
{
	// Entity�� �̸� ���
	std::string name = jsonData["m_name"];

	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);

	// ��ƼƼ �ߺ� ���� ���̵�
	unsigned long long id = entity->GetLastEntityID();

	entity->DestroyComponent<Transform>();
	loadScene->SetEntityManager(entity);

	// ������ �������� ������ �ʿ��� ���Ϳ� ��Ƶд�.
	prefabEntities.push_back(entity);

	entity->Deserialize(jsonData);

	// ��ƼƼ �̸� �ߺ� ���� (������ ������������ �Ű� ��� ��)
	// �̸��� �ߺ��Ǿ������� ���� ���� ������ �� �����Ͱ� ���� ������ �� �ִ�.
	for (const auto& findEntity : loadScene->GetEntities())
	{
		if (name == findEntity->GetName())
		{
			std::string addID = "_" + std::to_string(id);
			entity->SetName(entity->GetName() + addID);

			break;
		}
	}

	// ���÷��� �����͸� ���Ͽ� ������Ʈ�� �߰��Ѵ�.
	TypeInfoManager* manager = TypeInfoManager::GetInstance();

	// �ݶ��̴�, fsm ���� ��쿡�� start �Լ��� �����ϰ� �ִ�.
	// ����, �ݶ��̴��� Ʈ������ ������Ʈ���� �����ϰ� �ִ�.
	// �׷��Ƿ� vector�� ��Ƶΰ� start �Լ� ȣ��
	std::vector<std::shared_ptr<Component>> components;

	// Ÿ�� �̸��� (������Ʈ �Ǵ� ��ƼƼ) �׿� �ش��ϴ� ����Ÿ���� ��ȸ
	for (const auto& [typeName, data] : jsonData.items())
	{
		if (typeName == "Components")
		{
			for (const auto& [componentTypeName, data] : data.items())
			{
				// TypeInfoManager�� ��ϵǾ� �ִ� �̸��� class �� �پ������Ƿ� �߰��Ѵ�.
				std::string fullTypeName = "class " + componentTypeName;

				// Ÿ�� �̸��� �ش��ϴ� �ε����� �����´�.
				std::type_index index = manager->GetTypeIndex(fullTypeName);

				// �Ŵ������� �ε����� Ȱ���Ͽ� Ÿ�� ���� ��ü�� �����´�.
				const TypeInfo* typeInfo = manager->GetTypeInfo(index);

				// ��ü�� ��ϵ� �⺻ �����ڸ� ȣ���Ͽ� ������Ʈ�� �����.
				std::shared_ptr<Component> component = typeInfo->InvokdDefaultConstructor();

				entity->AddComponent(component);

				// json ������ ���� Deserialize
				component->Deserialize(data);

				// ��¿�� ���� �ϵ��ڵ��̴�?
				// ������Ʈ�� �Ŵ����� ����ֱ⿡..
				std::string superName = typeInfo->GetSuper()->GetName();

				if (componentTypeName == "MeshFilter")
				{
					const auto& meshFilterComponent = std::static_pointer_cast<MeshFilter>(component);
					meshFilterComponent->UpdateModelKey();
				}

				// �ݶ��̴��� fsm ������ start �Լ��� Ʈ�������� �����ϰ��ֱ⿡ 
				// �з��ؼ� ��Ƶд�.
				else if (superName == "Collider")
					components.push_back(component);

				else if (superName == "FSM")
					components.push_back(component);
				else if (componentTypeName == "MerchantUI")
					components.push_back(component);
			}
		}
	}

	// �ݶ��̴�, fsm - �̺�Ʈ �Ŵ����� ����ؼ� �� ü������ �Ǹ� 
	// start �Լ� ȣ���ϰԲ� ���� 
	EventManager::GetInstance()->NextFrameStartComponents(components);

	loadScene->AddEntity(entity);
}

//void SceneManager::StorePointerConnection(std::shared_ptr<Scene> scene)
//{
//	// ������ ���õ� ģ������ ������ ������ �ϱ� ����.
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
//	// ���ΰ� Ǫ�� ���̺� ����
//	for (const auto& shopKeeperFSM : shopKeeperFSMs)
//	{
//		for (const auto& foodTableScript : foodTableScripts)
//		{
//			// ����FSM�� Ǫ�� ���̺��� �θ� ���ٸ� ������ ���İ����� �ڽ��̶�� ��
//			if (shopKeeperFSM->GetTransform()->GetParent().lock() == foodTableScript->GetTransform()->GetParent().lock())
//			{
//				foodTableScript->SetPointerSense(shopKeeperFSM->GetPointerSense());
//				break;
//			}
//		}
//	}
//
//	// Ǫ�� ���̺�� ��ȣ�ۿ� Ű ����
//	for (const auto& foodTableScript : foodTableScripts)
//	{
//		for (const auto& interKeyScript : interKeyScripts)
//		{
//			// Ǫ�� ���̺�� ����Ű�� �θ� ���ٸ� ������ ���İ����� �ڽ��̶�� ��
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
