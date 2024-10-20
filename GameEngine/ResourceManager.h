#pragma once
#include "IManager.h"

/// <summary>
/// 2024.01.03
/// 리소스(무슨 리소스?)를 관리할 매니저 클래스
/// 2024.01.08
/// 내가 만든 엔티티들, 빌더, 게임 시작시 로드할 파일들을 관리해야할듯
/// 씬데이터들을 로드해서 씬 매니저에 준다?
/// 2024.01.11
/// FBX로더에서 읽은 데이터들도 추가적으로 관리해줘야한다 그래야 그래픽스엔진에 넘겨서 그려준다?
/// 2024.01.15
/// 어떤 데이터 타입이라도 다 받아서 관리를 해야한다
/// 2024.01.16
/// 하나의 매니저에서 여러개의 리소스를 관리할것이다 그게 아니면 각 리소스마다 매니저가 필요할텐데 그건 싫다
/// 리소스가 늘어나는 만큼 매니저가 생기기 때문
/// 2024.01.18
/// 어심프에서 만든 엔티티를 여기서 관리해버리기?
/// 여기서 모든 리소스를 다 갖고 있고 각 팩토리가 얘를 가지고 이곳에 저장하게 만들자
/// 2024.01.23
/// 어심프 데이터는 그래픽스쪽으로 넘어갔다
/// 그래서 오직 게임엔진에서만 쓸 리소스만 관리하면된다
/// 
/// 작성 : 유승운
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

	//함수 템플릿 특수화는 오버로딩으로 해버리면되지
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

	//맵 크기 150 x 150
	int CellData[151][151];
	int DirData[151][151];
	int GameData[151][151];

	///따로 가지고 싶은데 이건 아닌가?
	/*
	//loader에서 읽은 데이터들 일단 가지고 있자
	//std::map<std::string, std::shared_ptr<Mesh>> m_meshList;
	//사운드 리소스
	std::map<std::string, std::shared_ptr<Sound>> m_soundList;
	*/

	///여기에 한번에 다 넣고 찾기?
	std::map< std::string, std::shared_ptr<Resource>> m_resourceList;

	///내가 만든 엔티티들 다 가지고 있기
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




