#pragma once

/// <summary>
/// ���̺� �ε� ��ɿ� �ʿ��� ��� ���� Ŭ����
/// �ۼ��� : �赿��
/// </summary>
class PathManager
{
private:
	PathManager() {};
	~PathManager() {};

public:
	// Resource ��� ��ȯ
	static std::filesystem::path GetResourcesPath();

	// Data ��� ��ȯ
	static std::filesystem::path GetDataPath();

	// SceneData ��� ��ȯ
	static std::filesystem::path GetSceneDataPath();

	// PrefabData ��� ��ȯ
	static std::filesystem::path GetPrefabDataPath();

	// TextData ��� ��ȯ
	static std::filesystem::path GetTextDataPath();

	// SceneData ���� ���ϵ��� ��θ� vector�� ��Ƽ� ��ȯ
	static std::vector<std::filesystem::path> GetSceneDataList();

	// ���� �̸����� �� ���� ��� ���
	static std::filesystem::path GetScenePath(const std::wstring& sceneName);

	// PrefabData ������ Prefab ���� ��θ� vector�� ��Ƽ� ��ȯ
	static std::vector<std::filesystem::path> GetPrefabDataList();

	// ���� ��ο� �ִ� ���� ���� ����Ʈ�� �����´�
	static std::vector<std::filesystem::path> GetDirectoryList(const std::filesystem::path& path);

	// Scene �������� �ε��� Entity���� List�� �����´�
	static std::vector<std::filesystem::path> GetSceneEntityList(const std::filesystem::path& scenePath);

	//  "//" -> "\" ���� �ٲ۴�.
	static void EraseResourcePath(std::wstring& path);

	// �ش��ϴ� Ȯ���ڰ� �ִ��� �˻�
	static bool PathHasExtension(const wchar_t* path, const wchar_t* extension);

	// �̸����� ��� Ȯ���� ����
	static void RemoveAllExtention(std::string& main);
	static void RemoveAllExtention(std::wstring& main);
};

