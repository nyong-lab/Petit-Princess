#pragma once

/// <summary>
/// 세이브 로드 기능에 필요한 경로 관리 클래스
/// 작성자 : 김동일
/// </summary>
class PathManager
{
private:
	PathManager() {};
	~PathManager() {};

public:
	// Resource 경로 반환
	static std::filesystem::path GetResourcesPath();

	// Data 경로 반환
	static std::filesystem::path GetDataPath();

	// SceneData 경로 반환
	static std::filesystem::path GetSceneDataPath();

	// PrefabData 경로 반환
	static std::filesystem::path GetPrefabDataPath();

	// TextData 경로 반환
	static std::filesystem::path GetTextDataPath();

	// SceneData 내부 파일들의 경로를 vector에 담아서 반환
	static std::vector<std::filesystem::path> GetSceneDataList();

	// 씬의 이름으로 씬 파일 경로 얻기
	static std::filesystem::path GetScenePath(const std::wstring& sceneName);

	// PrefabData 내부의 Prefab 들의 경로를 vector에 담아서 반환
	static std::vector<std::filesystem::path> GetPrefabDataList();

	// 현재 경로에 있는 내부 파일 리스트를 가져온다
	static std::vector<std::filesystem::path> GetDirectoryList(const std::filesystem::path& path);

	// Scene 시작전에 로드할 Entity들의 List를 가져온다
	static std::vector<std::filesystem::path> GetSceneEntityList(const std::filesystem::path& scenePath);

	//  "//" -> "\" 으로 바꾼다.
	static void EraseResourcePath(std::wstring& path);

	// 해당하는 확장자가 있는지 검사
	static bool PathHasExtension(const wchar_t* path, const wchar_t* extension);

	// 이름에서 모든 확장자 삭제
	static void RemoveAllExtention(std::string& main);
	static void RemoveAllExtention(std::wstring& main);
};

