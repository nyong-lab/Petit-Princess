#include "GameEngine_pch.h"
#include "Serialize.h"

#include "Scene.h"
#include "Entity.h"

#include "PathManager.h"
#include "MeshFilter.h"

using json = nlohmann::json;

void Serialize::SerializeSceneInformation(std::weak_ptr<Scene> scene, std::filesystem::path& scenePath)
{
	//// 엔티티의 리소스들을 저장한다.
	//SaveResources(scene, scenePath);

	// 디렉터리가 없다면 해당 경로까지의 디렉터리를 만든다.
	if (!std::filesystem::exists(scenePath))
	{
		std::filesystem::create_directories(scenePath);
	}

	// 기존에 씬에 저장되어 있던 Entity들을 지운다.
	for (const auto& entry : std::filesystem::directory_iterator(scenePath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".json")
		{
			std::filesystem::remove(entry.path());
		}
	}

	// 현재 씬에 등록된 엔티티들을 새로 저장한다.
	for (const auto& entity : scene.lock()->GetEntities())
	{
		// 엔티티 저장
		SaveEntity(scenePath, entity);
	}
}

void Serialize::SaveEntity(std::filesystem::path& path, const std::shared_ptr<Entity> entity)
{
	std::string entityPath = path.string() + "/" + entity->GetName();

	// 현재 씬 디렉터리에 파일들을 순회하며
	// 현재 엔티티 이름과 같은 json 파일이 있다면
	// 추가 번호를 붙여준다. (안하면 덮어씌워지게 됨.)
	for (const auto& entry : PathManager::GetDirectoryList(path))
	{
		std::wstring path = entry;
		PathManager::EraseResourcePath(path);
		if (StringUtils::ToString(path) == entityPath + ".json")
		{
			std::string addID = " (" + std::to_string(entity->GetID()) + ")";
			entityPath += addID;

			entity->SetName(entity->GetName() + addID);

			break;
		}
	}

	if (entity->GetName() == "Minimap")
		int a = 0;

	// 엔티티 시리얼라이즈
	json entityJson;
	entityJson["Entity"] = entity;

	// json 내부에서 복사생성자를 호출하기에 엔티티 이름에 항상 clone이 추가로 붙는다.
	// 그러므로 다시 이름을 설정해주자.
	entityJson["Entity"]["m_name"] = entity->GetName();

	static int i = 1;

	// 컴포넌트 시리얼라이즈
	const auto& components = entity->GetComponents();
	for (auto& [index, component] : components)
	{
		json componentJson;

		component->Serialize(componentJson);
		
		std::string b = component->GetTypeInfo().GetName();
		if (b == "RayCaster")
		{
			std::string a = "RayCaster" + std::to_string(i);
			++i;
			entityJson["Components"][a] = componentJson;
		}
		else
			entityJson["Components"][component->GetTypeInfo().GetName()] = componentJson;
	}

	i = 1;

	// 계층구조 저장
	const auto& transform = entity->GetComponent<Transform>();

	// 부모가 있다면 부모의 이름을 저장한다.
	if (transform->HasParent())
		entityJson["Parent"] = transform->GetParent().lock()->GetOwner().lock()->GetName();

	std::string result = entityJson.dump(5);

	// 파일 저장
	std::ofstream outputFile(entityPath + ".json");
	if (outputFile.is_open())
	{
		outputFile << result;
		outputFile.close();
	}
	else
	{
		assert(outputFile.is_open());
	}
}


void Serialize::SerializePrefab(std::shared_ptr<Entity> entity)
{
	const auto& path = PathManager::GetPrefabDataPath();
	std::string originalName = entity->GetName();
	std::string entityPath = path.string() + "/" + originalName;

	// 현재 프리팹 디렉터리에 파일들을 순회하며
	// 현재 엔티티 이름과 같은 json 파일이 있다면
	// 추가 번호를 붙여준다. (안하면 덮어씌워지게 됨.)
	for (const auto& entry : PathManager::GetDirectoryList(path))
	{
		std::wstring path = entry;
		PathManager::EraseResourcePath(path);
		if (StringUtils::ToString(path) == entityPath + ".json")
		{
			std::string addID = " (" + std::to_string(entity->GetLastEntityIDAndPlusPlus()) + ")";
			entityPath += addID;

			entity->SetName(originalName + addID);

			break;
		}
	}

	// 엔티티 시리얼라이즈
	json entityJson;
	entityJson["Entity"] = entity;

	// json 내부에서 복사생성자를 호출하기에 엔티티 이름에 항상 clone이 추가로 붙는다.
	// 그러므로 다시 이름을 설정해주자.
	entityJson["Entity"]["m_name"] = entity->GetName();

	static int i = 1;

	// 컴포넌트 시리얼라이즈
	const auto& components = entity->GetComponents();
	for (auto& [index, component] : components)
	{
		json componentJson;

		component->Serialize(componentJson);

		std::string b = component->GetTypeInfo().GetName();
		if (b == "RayCaster")
		{
			std::string a = "RayCaster" + std::to_string(i);
			++i;
			entityJson["Components"][a] = componentJson;
		}
		else
			entityJson["Components"][component->GetTypeInfo().GetName()] = componentJson;
	}

	i = 1;

	// 자식이 있다면 자식들도 등록한다.
	for (const auto& childTransform : entity->GetComponent<Transform>()->GetChildren())
	{
		std::shared_ptr<Entity> childEntity = childTransform->GetOwner().lock();
		SavePrefabChildren(entityJson, childEntity);
	}

	m_childNum = 1;

	//// 계층구조 저장
	//const auto& transform = entity->GetComponent<Transform>();

	//// 부모가 있다면 부모의 이름을 저장한다.
	//if (transform->HasParent())
	//	entityJson["Parent"] = transform->GetParent().lock()->GetOwner().lock()->GetName();

	std::string result = entityJson.dump(5);

	// 파일 저장
	std::ofstream outputFile(entityPath + ".json");
	if (outputFile.is_open())
	{
		outputFile << result;
		outputFile.close();
	}
	else
	{
		assert(outputFile.is_open());
	}

	entity->SetName(originalName);
}

void Serialize::SavePrefabChildren(json& entityJson, std::shared_ptr<Entity> childEntity)
{
	// 프리팹의 자식들은 프리팹 아이디를 부모의 고유주소 + 자신의 고유 주소 지정한다.
	childEntity->SetPrefabID(childEntity->GetComponent<Transform>()->GetParent().lock()->
		GetOwner().lock()->GetPrefabID() + childEntity->GetAddressID());

	std::string childContain = "Child Entity " + std::to_string(m_childNum++);

	entityJson[childContain] = childEntity;

	// json 내부에서 복사생성자를 호출하기에 엔티티 이름에 항상 clone이 추가로 붙는다.
	// 그러므로 다시 이름을 설정해주자.
	entityJson[childContain]["m_name"] = childEntity->GetName();

	// 컴포넌트 시리얼라이즈
	const auto& components = childEntity->GetComponents();
	for (auto& [index, component] : components)
	{
		json componentJson;

		component->Serialize(componentJson);

		entityJson[childContain]["Components"][component->GetTypeInfo().GetName()] = componentJson;
	}

	// 자식들도 재귀적으로 저장
	for (const auto& childChildTransform : childEntity->GetComponent<Transform>()->GetChildren())
	{
		SavePrefabChildren(entityJson, childChildTransform->GetOwner().lock());
	}
}

int Serialize::m_childNum = 1;

//void Serialize::SaveResources(std::weak_ptr<Scene> scene, const std::filesystem::path& path)
//{
//	const TypeInfoManager* manager = TypeInfoManager::GetInstance();
//
//	std::vector<std::wstring> pathResources;
//
//	// Entity들이 들고있는 경로 정보를 모두 가져온다.
//	for (const auto& entity : scene.lock()->GetEntities())
//	{
//		for (auto& component : entity->GetComponents())
//		{
//			TypeInfo* typeInfo = manager->GetTypeInfo(component.first);
//
//			if (typeInfo != nullptr)
//			{
//				for (const auto property : typeInfo->GetProperties())
//				{
//					// 경로 정보를 찾는다.
//					FindPathResource(component.second, property, pathResources);
//				}
//			}
//		}
//	}
//
//	// 경로가 비어있다면 저장하지 않는다.
//	pathResources.erase(std::remove_if(pathResources.begin(), pathResources.end(),
//		[](std::wstring path)
//		{
//			if (path.empty())
//			{
//				return true;
//			}
//
//	return false;
//		}),
//		pathResources.end());
//
//	//  "//" 를 "\" 으로 정규화시킨다.
//	for (auto& path : pathResources)
//	{
//		PathManager::EraseResourcePath(path);
//	}
//
//	// 파일 형식에 따라서 분류해서 저장한다.
//	SaveEntityResources(pathResources, path);
//}

//void Serialize::FindPathResource(const std::shared_ptr<Component> component, const Property* property, std::vector<std::wstring> pathResources)
//{
//	if (property->GetPropertyTypeCategory() == PropertyTypeCategory::Primitive)
//	{
//		// wstring은 일단 다 담아온다.
//		if (property->GetName() == "std::wstring")
//		{
//			std::wstring data = property->Get<std::wstring>(component.get()).GetValue();
//			pathResources.push_back(data.c_str());
//		}
//	}
//}
