#pragma once

class Scene;
class Entity;

/// <summary>
/// �� ������, ������ �����͸� �ø�������� �ϱ� ���� ��ƿ Ŭ����
/// ��ø��������� �����͸� ���ؼ� ���Ŵ����� Ȱ���Ѵ�.
/// �ۼ��� : �赿��
/// </summary>
class Serialize
{
public:
	// �� ��ü ����
	static void SerializeSceneInformation(std::weak_ptr<Scene> scene, std::filesystem::path& scenePath);

	// ������ ��ƼƼ ����
	static void SerializePrefab(std::shared_ptr<Entity> entity);

private:
	// ��ƼƼ ����
	static void SaveEntity(std::filesystem::path& path, const std::shared_ptr<Entity> entity);

	// ������ ��ƼƼ�� �ڽĵ� ����
	static void SavePrefabChildren(nlohmann::json& entityJson, std::shared_ptr<Entity> entity);

	// ���� ���� ���������� ���ҽ����� Ű������ ������ Ÿ�ӿ� ������ ��� �ְ� �ϱ⿡ �ʿ䰡 ����.
	//// ��ƼƼ�� ��� �ִ� ���ҽ��� ����
	//static void SaveResources(std::weak_ptr<Scene> scene, const std::filesystem::path& path);

	//// ��ƼƼ�� ��� �ִ� ���ҽ��� ��� ã��
	//static void FindPathResource(const std::shared_ptr<Component> component, const Property* property, std::vector<std::wstring> pathResources);

	static int m_childNum;
};

