#pragma once

class Scene;
class Entity;

/// <summary>
/// 씬 데이터, 프리팹 데이터를 시리얼라이즈 하기 위한 유틸 클래스
/// 디시리얼라이즈는 에디터를 통해서 씬매니저를 활용한다.
/// 작성자 : 김동일
/// </summary>
class Serialize
{
public:
	// 씬 전체 저장
	static void SerializeSceneInformation(std::weak_ptr<Scene> scene, std::filesystem::path& scenePath);

	// 프리팹 엔티티 저장
	static void SerializePrefab(std::shared_ptr<Entity> entity);

private:
	// 엔티티 저장
	static void SaveEntity(std::filesystem::path& path, const std::shared_ptr<Entity> entity);

	// 프리팹 엔티티의 자식들 저장
	static void SavePrefabChildren(nlohmann::json& entityJson, std::shared_ptr<Entity> entity);

	// 현재 엔진 구조에서는 리소스들을 키값으로 컴파일 타임에 모조리 들고 있게 하기에 필요가 없다.
	//// 엔티티가 들고 있는 리소스들 저장
	//static void SaveResources(std::weak_ptr<Scene> scene, const std::filesystem::path& path);

	//// 엔티티가 들고 있는 리소스들 경로 찾기
	//static void FindPathResource(const std::shared_ptr<Component> component, const Property* property, std::vector<std::wstring> pathResources);

	static int m_childNum;
};

