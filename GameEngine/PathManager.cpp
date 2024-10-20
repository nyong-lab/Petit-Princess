#include "GameEngine_pch.h"
#include "PathManager.h"

std::filesystem::path PathManager::GetResourcesPath()
{
	std::filesystem::path path = "../Resource";

	/*std::filesystem::path path = std::filesystem::current_path();
	path += "\\Resource";*/

	return 	path;
}

std::filesystem::path PathManager::GetDataPath()
{
	std::filesystem::path path = "../Resource/Data";

	return path;
}

std::filesystem::path PathManager::GetSceneDataPath()
{
	std::filesystem::path path = "../Resource/Data/SceneData";

	//std::filesystem::path path = std::filesystem::current_path();
	//path += "\\Resource\\JsonData\\SceneData";

	return 	path;
}

std::filesystem::path PathManager::GetPrefabDataPath()
{
	std::filesystem::path path = "../Resource/Data/PrefabData";

	/*std::filesystem::path path = std::filesystem::current_path();
	path += "/Resource/JsonData/EntityData";*/

	return 	path;
}

std::filesystem::path PathManager::GetTextDataPath()
{
	std::filesystem::path path = "../Resource/Data/TextData";

	return 	path;
}

std::vector<std::filesystem::path> PathManager::GetSceneDataList()
{
	std::filesystem::path scenePath = GetSceneDataPath();
	std::vector<std::filesystem::path> vectorPath;

	for (const auto& path : std::filesystem::directory_iterator(scenePath))
	{
		vectorPath.push_back(path.path());
	}

	return vectorPath;
}

std::filesystem::path PathManager::GetScenePath(const std::wstring& sceneName)
{
	std::filesystem::path path = GetSceneDataPath();
	path += "/";
	path += sceneName;

	return path;
}

std::vector<std::filesystem::path> PathManager::GetPrefabDataList()
{
	std::filesystem::path path = GetPrefabDataPath();
	std::vector<std::filesystem::path> paths;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		paths.push_back(entry.path());
	}

	return paths;
}

std::vector<std::filesystem::path> PathManager::GetDirectoryList(const std::filesystem::path& path)
{
	std::vector<std::filesystem::path> paths;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		paths.push_back(entry.path());
	}

	return paths;
}

std::vector<std::filesystem::path> PathManager::GetSceneEntityList(const std::filesystem::path& scenePath)
{
	std::vector<std::filesystem::path> entityList;

	for (const auto& entry : std::filesystem::directory_iterator(scenePath))
	{
		entityList.push_back(entry.path());
	}

	return entityList;
}

/// <summary>
/// 찾아보니 절대경로를 상대경로로 바꿔주는 relative 함수가 있었다.
/// 첫번째 매개변수를 두번째 매개변수 기준으로 상대 경로로 변환해준다.
/// 다음엔 그거 쓰자
/// </summary>
/// <param name="path"></param>
void PathManager::EraseResourcePath(std::wstring& path)
{
	// 혹시모를 / \\ 등같은 경로가있을 수 있으니 정규화한다.
	std::filesystem::path resouresPath = GetResourcesPath();
	std::filesystem::path currentPath(path);


	StringUtils::Replace(path, L"\\", L"/");
	/*std::wstring::size_type pos = 0;
	while ((pos = path.find(L"//", pos)) != std::wstring::npos)
	{
		path.replace(pos, 2, L"/");
		pos += 1;
	}*/
}

bool PathManager::PathHasExtension(const wchar_t* path, const wchar_t* extension)
{
	std::wstring pathStr(path);
	std::wstring extensionStr(extension);

	return std::filesystem::path(pathStr).extension() == extensionStr;
}

void PathManager::RemoveAllExtention(std::wstring& main)
{
	size_t pos = main.find(L".");
	if (pos != std::wstring::npos)
	{
		main.erase(pos);
	}
}

void PathManager::RemoveAllExtention(std::string& main)
{
	size_t pos = main.find(".");
	if (pos != std::wstring::npos)
	{
		main.erase(pos);
	}
}