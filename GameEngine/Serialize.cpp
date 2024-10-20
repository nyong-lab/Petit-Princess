#include "GameEngine_pch.h"
#include "Serialize.h"

#include "Scene.h"
#include "Entity.h"

#include "PathManager.h"
#include "MeshFilter.h"

using json = nlohmann::json;

void Serialize::SerializeSceneInformation(std::weak_ptr<Scene> scene, std::filesystem::path& scenePath)
{
	//// ��ƼƼ�� ���ҽ����� �����Ѵ�.
	//SaveResources(scene, scenePath);

	// ���͸��� ���ٸ� �ش� ��α����� ���͸��� �����.
	if (!std::filesystem::exists(scenePath))
	{
		std::filesystem::create_directories(scenePath);
	}

	// ������ ���� ����Ǿ� �ִ� Entity���� �����.
	for (const auto& entry : std::filesystem::directory_iterator(scenePath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".json")
		{
			std::filesystem::remove(entry.path());
		}
	}

	// ���� ���� ��ϵ� ��ƼƼ���� ���� �����Ѵ�.
	for (const auto& entity : scene.lock()->GetEntities())
	{
		// ��ƼƼ ����
		SaveEntity(scenePath, entity);
	}
}

void Serialize::SaveEntity(std::filesystem::path& path, const std::shared_ptr<Entity> entity)
{
	std::string entityPath = path.string() + "/" + entity->GetName();

	// ���� �� ���͸��� ���ϵ��� ��ȸ�ϸ�
	// ���� ��ƼƼ �̸��� ���� json ������ �ִٸ�
	// �߰� ��ȣ�� �ٿ��ش�. (���ϸ� ��������� ��.)
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

	// ��ƼƼ �ø��������
	json entityJson;
	entityJson["Entity"] = entity;

	// json ���ο��� ��������ڸ� ȣ���ϱ⿡ ��ƼƼ �̸��� �׻� clone�� �߰��� �ٴ´�.
	// �׷��Ƿ� �ٽ� �̸��� ����������.
	entityJson["Entity"]["m_name"] = entity->GetName();

	static int i = 1;

	// ������Ʈ �ø��������
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

	// �������� ����
	const auto& transform = entity->GetComponent<Transform>();

	// �θ� �ִٸ� �θ��� �̸��� �����Ѵ�.
	if (transform->HasParent())
		entityJson["Parent"] = transform->GetParent().lock()->GetOwner().lock()->GetName();

	std::string result = entityJson.dump(5);

	// ���� ����
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

	// ���� ������ ���͸��� ���ϵ��� ��ȸ�ϸ�
	// ���� ��ƼƼ �̸��� ���� json ������ �ִٸ�
	// �߰� ��ȣ�� �ٿ��ش�. (���ϸ� ��������� ��.)
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

	// ��ƼƼ �ø��������
	json entityJson;
	entityJson["Entity"] = entity;

	// json ���ο��� ��������ڸ� ȣ���ϱ⿡ ��ƼƼ �̸��� �׻� clone�� �߰��� �ٴ´�.
	// �׷��Ƿ� �ٽ� �̸��� ����������.
	entityJson["Entity"]["m_name"] = entity->GetName();

	static int i = 1;

	// ������Ʈ �ø��������
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

	// �ڽ��� �ִٸ� �ڽĵ鵵 ����Ѵ�.
	for (const auto& childTransform : entity->GetComponent<Transform>()->GetChildren())
	{
		std::shared_ptr<Entity> childEntity = childTransform->GetOwner().lock();
		SavePrefabChildren(entityJson, childEntity);
	}

	m_childNum = 1;

	//// �������� ����
	//const auto& transform = entity->GetComponent<Transform>();

	//// �θ� �ִٸ� �θ��� �̸��� �����Ѵ�.
	//if (transform->HasParent())
	//	entityJson["Parent"] = transform->GetParent().lock()->GetOwner().lock()->GetName();

	std::string result = entityJson.dump(5);

	// ���� ����
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
	// �������� �ڽĵ��� ������ ���̵� �θ��� �����ּ� + �ڽ��� ���� �ּ� �����Ѵ�.
	childEntity->SetPrefabID(childEntity->GetComponent<Transform>()->GetParent().lock()->
		GetOwner().lock()->GetPrefabID() + childEntity->GetAddressID());

	std::string childContain = "Child Entity " + std::to_string(m_childNum++);

	entityJson[childContain] = childEntity;

	// json ���ο��� ��������ڸ� ȣ���ϱ⿡ ��ƼƼ �̸��� �׻� clone�� �߰��� �ٴ´�.
	// �׷��Ƿ� �ٽ� �̸��� ����������.
	entityJson[childContain]["m_name"] = childEntity->GetName();

	// ������Ʈ �ø��������
	const auto& components = childEntity->GetComponents();
	for (auto& [index, component] : components)
	{
		json componentJson;

		component->Serialize(componentJson);

		entityJson[childContain]["Components"][component->GetTypeInfo().GetName()] = componentJson;
	}

	// �ڽĵ鵵 ��������� ����
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
//	// Entity���� ����ִ� ��� ������ ��� �����´�.
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
//					// ��� ������ ã�´�.
//					FindPathResource(component.second, property, pathResources);
//				}
//			}
//		}
//	}
//
//	// ��ΰ� ����ִٸ� �������� �ʴ´�.
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
//	//  "//" �� "\" ���� ����ȭ��Ų��.
//	for (auto& path : pathResources)
//	{
//		PathManager::EraseResourcePath(path);
//	}
//
//	// ���� ���Ŀ� ���� �з��ؼ� �����Ѵ�.
//	SaveEntityResources(pathResources, path);
//}

//void Serialize::FindPathResource(const std::shared_ptr<Component> component, const Property* property, std::vector<std::wstring> pathResources)
//{
//	if (property->GetPropertyTypeCategory() == PropertyTypeCategory::Primitive)
//	{
//		// wstring�� �ϴ� �� ��ƿ´�.
//		if (property->GetName() == "std::wstring")
//		{
//			std::wstring data = property->Get<std::wstring>(component.get()).GetValue();
//			pathResources.push_back(data.c_str());
//		}
//	}
//}
