#pragma once

#include "json.hpp"

#include "IEntity.h"
#include <vector>
#include "EntityDefine.h"
#include "TypeIndex.h"
#include "Transform.h"
#include "RayCaster.h"

class Component;
class Transform;
class Scene;
class IGraphics;
class Script;

class IManager;
class SceneManager;
class ResourceManager;
class CameraManager;
class CollisionManager;

template<typename T>
class IBuilder;

/// <summary>
/// 2024.01.02
/// Component���� ��� �׸��̵Ǵ� Entity Ŭ����
/// �÷��̾�, NPC, ����, ������, ����
/// �ۼ� : ���¿�
/// 1/22 00:48 ���� : �赿�� - �ּ� �߰� �� ������ �ʿ��� Get, AddComponent �߰�
/// 1.28 �ռ��� -> ����� ���� �߰�
///            -> SetLayerCollision �Լ� �ٵ���, 
///            -> �浹 �ݹ� ������ũ��Ʈ�� ����.
///            -> FixedUpdate �߰�
/// </summary>
class Entity : public IEntity, public std::enable_shared_from_this<Entity>
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Entity)
		PROPERTY(m_name)
		PROPERTY(m_id)
		PROPERTY(m_layer)
		PROPERTY(m_layerCollision)
		PROPERTY(m_activeSelf)
		PROPERTY(m_cullingOffAllEntity)
		PROPERTY(m_isRender)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Entity, m_name, m_id, m_layer, m_layerCollision,
			m_activeSelf, m_prefabID, m_addressID, m_isRender)

public:
	//������
	Entity() = delete;
	Entity(std::string name);
	//�Ҹ���
	~Entity() = default;
	//���� ������
	Entity(const std::shared_ptr<Entity>& other);
	//���� ���� ������
	Entity& operator=(const Entity& rhs)
	{
		m_name = rhs.m_name;
		m_id = rhs.m_id;
		m_layer = rhs.m_layer;
		m_activeSelf = rhs.m_activeSelf;
		m_components = rhs.m_components;

		//TODO:Componenet Container�� �����ؾ��Ѵ�

		return *this;
	}

	//������Ʈ���� ������Ʈ�ϴ� �Լ�
	virtual void Start() override;
	virtual void Update(double dt) override final;
	virtual void FixedUpdate(double fixedTime) override final;
	virtual void Render(double dt) override final;
	virtual void DebugRender() override final;
	virtual void Finalize() override final;

	// Serialize
	void Deserialize(const nlohmann::json& json) { if (!json.is_null()) from_json(json, *this); }

	// Collision
	virtual void OnCollisionEnter(const Collision& inCollision);
	virtual void OnCollisionStay(const Collision& inCollision);
	virtual void OnCollisionExit(const Collision& inCollision);
	virtual void OnTriggerEnter(Collider* otherCollider);
	virtual void OnTriggerStay(Collider* otherCollider);
	virtual void OnTriggerExit(Collider* otherCollider);

	//Component* GetComponent(const int UID) const { return m_ComponenetList[UID]; }

	void AddComponent(std::shared_ptr<Component> component);
	void AddComponent(std::shared_ptr<IBuilder<Component>> builder);


	// ��������
	void SetParent(Entity* parent) { m_parent = parent; }
	void AddChild(Entity* child) { m_children.push_back(child); child->SetParent(this); }
	Entity* GetParent() { return m_parent; }
	Entity* GetChild(std::string name);

	std::weak_ptr<Entity> GetSmartChild(std::string name);

	// ������Ʈ �ı�
	void DestroyComponent(std::type_index index);

	// ��� ������Ʈ �ı�
	void DestroyAllComponents();

	// Get - Set
	std::string GetName() const { return m_name; }
	void SetName(std::string name) { m_name = name; }
	bool IsActive() { return m_activeSelf; }
	void SetActive(bool value) { m_activeSelf = value; }
	bool IsRender() const { return m_isRender; }
	void SetRender(bool value) { m_isRender = value; }

	// Editor ������ �������� ������ �ʿ���
	unsigned int GetID() const { return m_id; }
	void SetNewID() { m_id = m_lastEntityID + 1; }
	unsigned long long GetPrefabID() { return m_prefabID; }
	void SetPrefabID(unsigned long long value) { m_prefabID = value; }
	unsigned long long GetAddressID() { return m_addressID; }
	unsigned int GetLastEntityID() { return m_lastEntityID; }
	unsigned int GetLastEntityIDAndPlusPlus() { return m_lastEntityID++; }

	// Layer
	Layers GetLayer() const { return m_layer; }
	void SetLayer(const Layers n) { m_layer = n; }

	// Collision
	LayerCollision GetLayerCollision() const { return m_layerCollision; }
	void SetLayerCollision(const LayerCollision n);

	// Component
	std::shared_ptr<Component> AddComponent(std::shared_ptr<Component> component, std::type_index index);
	std::shared_ptr<Component> GetComponent(std::type_index index);
	bool HasComponent(std::type_index index) { return m_components.find(index) != m_components.end(); }
	const std::unordered_multimap<std::type_index, std::shared_ptr<Component>>& GetComponents()const { return m_components; }

	// Template - Component
	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args);

	template <typename T>
	std::shared_ptr<T> GetComponent();

	template <typename T>
	std::shared_ptr<T> GetComponent(size_t index);

	template<typename T>
	bool HasComponent();

	template<typename T>
	void DestroyComponent();

	void SetManager(std::weak_ptr<IManager> manager);
	std::weak_ptr<SceneManager> GetSceneManager() { return m_sceneManager; }
	void SetGraphics(std::weak_ptr<IGraphics> graphics);
	std::weak_ptr<CameraManager> GetCameraManager() { return m_cameraManager; }
	std::weak_ptr<IGraphics> GetGraphics() { return m_graphics; }
	bool IsCullingOffAll() { return m_cullingOffAllEntity; }
	void SetCullingOffAll(bool value) { m_cullingOffAllEntity = value; }

	std::weak_ptr<ResourceManager> GetResourceManager() { return m_resourceManager; }

protected:
	static unsigned int m_lastEntityID;

	std::string m_name;
	int m_id;
	Layers m_layer;
	LayerCollision m_layerCollision;
	bool m_activeSelf;
	static bool m_cullingOffAllEntity;
	unsigned long long m_prefabID;
	unsigned long long m_addressID;
	std::unordered_multimap<std::type_index, std::shared_ptr<Component>> m_components;

	Entity* m_parent;
	std::vector<Entity*> m_children;

	bool m_IsDestroy;
	bool m_isRender; // ����: ���� �̰� �ִ°� ���ڴٰ��ؼ� ���⸸������

	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<ResourceManager> m_resourceManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<IGraphics> m_graphics;
};

template <typename T>
std::shared_ptr<T> Entity::GetComponent()
{
	std::type_index index = GetTypeIndex<T>();

	auto iter = m_components.find(index);

	if (iter == m_components.end())
 		return nullptr;

	std::shared_ptr<T> component = std::static_pointer_cast<T>(iter->second);

	return component;
}

// ������ Ű������ index��° ������Ʈ�� ������ ���� �Լ� 
// ����� RayCaster �Լ��� ���� ���ø��̴�.
template <typename T>
std::shared_ptr<T> Entity::GetComponent(size_t index)
{
	std::type_index typeIndex = GetTypeIndex<T>();

	auto range = m_components.equal_range(typeIndex);

	size_t currentIndex = 0;

	// �ش� Ű���� ���� ù ��° ~ ������ �� ���� ����
	for (auto iter = range.first; iter != range.second; ++iter)
	{
		if (currentIndex == index)
		{
			// ã�� �ε����� ��ġ�ϴ� ������Ʈ�� ��ȯ
			return std::static_pointer_cast<T>(iter->second);
		}

		++currentIndex;
	}
	return nullptr;
}

template<typename T>
bool Entity::HasComponent()
{
	std::type_index index = GetTypeIndex<T>();

	return m_components.find(index) != m_components.end();
}

template<typename T>
void Entity::DestroyComponent()
{
	std::type_index index = GetTypeIndex<T>();
	auto iter = m_components.find(index);

	if (iter != m_components.end())
	{
		auto& component = iter->second;
		component->Finalize();
		component.reset();
		m_components.erase(iter);
	}
}

template <typename T, typename ...Args>
std::shared_ptr<T> Entity::AddComponent(Args&&... args)
{
	std::type_index index = GetTypeIndex<T>();

	// Mk.1 ����ĳ���ʹ� ������ �� �� �ִ�.
	if (index == GetTypeIndex<RayCaster>())
	{
		int a = 0;
	}
	// �̹� ������ ������Ʈ ���� x 
	else if (std::shared_ptr<Component> component = GetComponent<T>(); component != nullptr)
	{
		return std::dynamic_pointer_cast<T>(component);
	}

	// �������� ���ø��� ����� ������
	std::shared_ptr<T> component = std::make_shared<T>(args...);

	// Ʈ�������� ���� ����� �� owner�� ������ �ش�.
	if (index != GetTypeIndex<Transform>())
	{
		component->SetOwner(shared_from_this());
	}

	m_components.insert(make_pair(index, component));
	//component->Start();

	return component;
}

