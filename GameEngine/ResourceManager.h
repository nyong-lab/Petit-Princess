#pragma once
#include "IManager.h"

/// <summary>
/// 2024.01.03
/// ���ҽ�(���� ���ҽ�?)�� ������ �Ŵ��� Ŭ����
/// 2024.01.08
/// ���� ���� ��ƼƼ��, ����, ���� ���۽� �ε��� ���ϵ��� �����ؾ��ҵ�
/// �������͵��� �ε��ؼ� �� �Ŵ����� �ش�?
/// 2024.01.11
/// FBX�δ����� ���� �����͵鵵 �߰������� ����������Ѵ� �׷��� �׷��Ƚ������� �Ѱܼ� �׷��ش�?
/// 2024.01.15
/// � ������ Ÿ���̶� �� �޾Ƽ� ������ �ؾ��Ѵ�
/// 2024.01.16
/// �ϳ��� �Ŵ������� �������� ���ҽ��� �����Ұ��̴� �װ� �ƴϸ� �� ���ҽ����� �Ŵ����� �ʿ����ٵ� �װ� �ȴ�
/// ���ҽ��� �þ�� ��ŭ �Ŵ����� ����� ����
/// 2024.01.18
/// ��������� ���� ��ƼƼ�� ���⼭ �����ع�����?
/// ���⼭ ��� ���ҽ��� �� ���� �ְ� �� ���丮�� �긦 ������ �̰��� �����ϰ� ������
/// 2024.01.23
/// ����� �����ʹ� �׷��Ƚ������� �Ѿ��
/// �׷��� ���� ���ӿ��������� �� ���ҽ��� �����ϸ�ȴ�
/// 
/// �ۼ� : ���¿�
/// </summary>

class Resource;
class Entity;
class Sound;

#include "Entity.h"

class ResourceManager : public IManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourceManager(const ResourceManager& manager) = delete;
	ResourceManager& operator=(const ResourceManager& manager) = delete;

	ResourceManager(const ResourceManager&& manager) = delete;
	ResourceManager& operator=(const ResourceManager&& manager) = delete;

	virtual void Initialize() override;
	virtual void Start() override {};
	virtual void Update(double dt) override;
	virtual void Finalize() override;

	template<typename T>
	std::shared_ptr<T> Get(std::string name);

	//�Լ� ���ø� Ư��ȭ�� �����ε����� �ع��������
	std::shared_ptr<Entity> Get(std::string name)
	{
		return m_entity_Pool[name];
	}

	template<typename T>
	void Add(std::string name, T resource);

	void Add(std::string name, std::shared_ptr<Entity> resource)
	{
		m_entity_Pool.insert(std::make_pair(name, resource));
	}

	void ReadMapInfoCSV(std::string path);
	void ReadDirInfoCSV(std::string path);
	void ReadGameInfoCSV(std::string path);

	int (*GetMapInfo())[151] { return CellData; }
	int (*GetDirData())[151] { return DirData; }
	int (*GetGameData())[151] { return GameData; }

private:

	//�� ũ�� 150 x 150
	int CellData[151][151];
	int DirData[151][151];
	int GameData[151][151];

	///���� ������ ������ �̰� �ƴѰ�?
	/*
	//loader���� ���� �����͵� �ϴ� ������ ����
	//std::map<std::string, std::shared_ptr<Mesh>> m_meshList;
	//���� ���ҽ�
	std::map<std::string, std::shared_ptr<Sound>> m_soundList;
	*/

	///���⿡ �ѹ��� �� �ְ� ã��?
	std::map< std::string, std::shared_ptr<Resource>> m_resourceList;

	///���� ���� ��ƼƼ�� �� ������ �ֱ�
	std::map<std::string, std::shared_ptr<Entity>> m_entity_Pool;
};

template<typename T>
std::shared_ptr<T> ResourceManager::Get(std::string name)
{
	return static_pointer_cast<T>(m_resourceList[name]);
}

template<typename T>
void ResourceManager::Add(std::string name, T resource)
{
	m_resourceList.insert(std::make_pair(name, resource));
}




