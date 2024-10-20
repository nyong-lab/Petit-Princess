#include "GameEngine_pch.h"
#include "Entity.h"
#include "Component.h"
#include "IBuilder.h"

#include "Transform.h"
#include "Renderer.h"
#include "Collider.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "Script.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"

unsigned int Entity::m_lastEntityID = 0;


bool Entity::m_cullingOffAllEntity = false;

Entity::Entity(std::string name)
	: m_name(name)
	, m_activeSelf(true)
	, m_layer(Layers::None)
	, m_layerCollision(LayerCollision::Default)
	, m_IsDestroy(false)
	, m_prefabID(0)
	, m_addressID((unsigned long long)this)
	, m_isRender(true)
{
	AddComponent<Transform>();
	m_id = m_lastEntityID++;
}

// children ���� clone ��Ű�� ���ؼ��� scene ������ entity�� ����ְ� �������Ѵ�.
// �ϴ� ��������ڿ����� ���������� �����Ű�� �ʴ´�. 
// 
// �� �����ڸ� ����Ѵٸ� ������ �ǰ� �� ���Ŀ� ��� ������Ʈ�� ��ȸ�� ���� SetOwner �Լ� ȣ�� ����� �Ѵ�.
// (Component ������ Entity�� weak_ptr �� ������ �ֱ� ����)
Entity::Entity(const std::shared_ptr<Entity>& other)
{
	m_name = other->m_name + " (clone)";
	m_layer = other->m_layer;
	m_layerCollision = other->m_layerCollision;
	m_activeSelf = other->m_activeSelf;
	m_prefabID = other->m_prefabID;

	// ������ ������ �������� ������ ���� id�̴�.
	// json ��������� �������� ���� ���縦 �Ѵٸ� ���� �������� �ٲ��� 
	// ���� ����� �����Ѵ�.
	m_addressID = other->m_addressID;

	m_IsDestroy = false;
	m_isRender = other->m_isRender;

	// ���������� �������� �ʴ´�.
	Entity* m_parent = nullptr;
	std::vector<Entity*> m_children{};

	bool IsDestroy = other->m_IsDestroy;

	m_sceneManager = other->m_sceneManager;
	m_resourceManager = other->m_resourceManager;
	m_cameraManager = other->m_cameraManager;
	m_graphics = other->m_graphics;

	// ID �� ������ ������ ������ �������� �ʴ´�.
	m_id = m_lastEntityID++;

	// �������縦 ���� ������ Ŭ�� �Լ��� ȣ������ش�.
	// (�� ������Ʈ�� ���纻�� ���� �� ��ȯ)
	for (const auto& [index, component] : other->m_components)
	{
		m_components.insert({ index, component->Clone() });

		const auto& registeredComponent = m_components.find(index)->second;
		if (registeredComponent == nullptr)
		{
			assert(!"Clone �Լ��� ����� ������ �ȵ� ������Ʈ�� �ֽ��ϴ�. Clone �Լ� ���ο��� �ڱ� �ڽ��� ������ ��ȯ�ؾ� �մϴ�.");
		}

		registeredComponent->CloneBaseComponentMember(component);
	}
}


void Entity::Start()
{
	for (const auto& iter : m_components)
	{
		//addcomponent�� �ϸ� �ٷ� start�� �Ҹ��µ� ���⼭ �ʿ��ұ�?
		iter.second->Start();
	}
}


void Entity::Update(double dt)
{
	// �ݶ��̴��� �������� �з��� ����
	std::vector<std::shared_ptr<Component>> colliderComponents;
	std::vector<std::shared_ptr<Component>> rendererComponents;

	// �ݶ��̴��� �������� ������Ʈ �����ʰ� �з� ���Ϳ� �������� ��´�.
	for (const auto& component : m_components)
	{
		// Mk.1
		if (std::dynamic_pointer_cast<Collider>(component.second) != nullptr)
		{
			colliderComponents.push_back(component.second);
		}
		else if (std::dynamic_pointer_cast<Renderer>(component.second) != nullptr)
		{
			rendererComponents.push_back(component.second);
		}
		else
		{
			component.second->Update(dt);
		}

		// 2.
		//std::string classFureName = component.second->GetTypeInfo().GetSuper()->GetName();
		//
		//if (classFureName == "Collider")
		//{
		//	colliderComponents.push_back(component.second);
		//}
		//else if (classFureName != "Renderer")
		//{
		//	rendererComponents.push_back(component.second);
		//}
		//else
		//{
		//	component.second->Update();
		//}
	}

	// �ݶ��̴� ������Ʈ
	for (const auto& collider : colliderComponents)
	{
		collider->Update(dt);
	}

	// ������ ������Ʈ
	for (const auto& renderer : rendererComponents)
	{
		renderer->Update(dt);
	}
}

void Entity::FixedUpdate(double fixedTime)
{
	for (const auto& component : m_components)
	{
		component.second->FixedUpdate(fixedTime);
	}
}

void Entity::Render(double dt)
{
	if (!m_isRender) return;

	for (const auto& iter : m_components)
	{
		iter.second->Render(dt);
	}
}

void Entity::DebugRender()
{
	for (const auto& iter : m_components)
	{
		iter.second->DebugRender();
	}
}

void Entity::Finalize()
{
	DestroyAllComponents();
}

void Entity::OnCollisionEnter(const Collision& inCollision)
{	
	for (auto& comp : m_components)
	{
		// Script ���̳��� ĳ���� �����ϸ� �Լ� ȣ�����ֱ�.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc) 
			sc->OnCollisionEnter(inCollision);
	}
}

void Entity::OnCollisionStay(const Collision& inCollision)
{
	for (auto& comp : m_components)
	{
		// Script ���̳��� ĳ���� �����ϸ� �Լ� ȣ�����ֱ�.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnCollisionStay(inCollision);
	}
}

void Entity::OnCollisionExit(const Collision& inCollision)
{
	for (auto& comp : m_components)
	{
		// Script ���̳��� ĳ���� �����ϸ� �Լ� ȣ�����ֱ�.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnCollisionExit(inCollision);
	}
}

void Entity::OnTriggerEnter(Collider* otherCollider)
{
	for (auto& comp : m_components)
	{
		// Script ���̳��� ĳ���� �����ϸ� �Լ� ȣ�����ֱ�.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnTriggerEnter(otherCollider);
	}
}

void Entity::OnTriggerStay(Collider* otherCollider)
{
	for (auto& comp : m_components)
	{
		// Script ���̳��� ĳ���� �����ϸ� �Լ� ȣ�����ֱ�.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnTriggerStay(otherCollider);
	}
}

void Entity::OnTriggerExit(Collider* otherCollider)
{
	for (auto& comp : m_components)
	{
		// Script ���̳��� ĳ���� �����ϸ� �Լ� ȣ�����ֱ�.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnTriggerExit(otherCollider);
	}
}

void Entity::SetLayerCollision(const LayerCollision n)
{
	m_layerCollision = n;

	// �ݶ��̴��� ������ �� ���� ����� �ϴ� ���ε��� ó���ϴ� ������ �س��Ҵ�.
	std::shared_ptr<BoxCollider> boxCollider = GetComponent<BoxCollider>();
	std::shared_ptr<SphereCollider> sphereCollider = GetComponent<SphereCollider>();
	std::shared_ptr<CapsuleCollider> capsuleCollider = GetComponent<CapsuleCollider>();

	if (boxCollider) boxCollider->SetLayerCollision(n);
	if (sphereCollider) sphereCollider->SetLayerCollision(n);
	if (capsuleCollider) capsuleCollider->SetLayerCollision(n);
}

void Entity::AddComponent(std::shared_ptr<Component> component)
{

	//TODO:���� �ؾ� ���ϰ� �� �� �־�
	//��Ÿ���̵� ������ Ÿ���̵� �ǰ�ü�� Ÿ���� ���� �˾ƾ� index�� ã���Ŵ�
	//std::type_index index = GetTypeIndex<decltype(component)>();
	//m_components.insert(make_pair(index, component));


	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
	// Dongil  
	// type_index �� type_info Ŭ������ �����ϴ� Ŭ������� �մϴ�
	// �׸��� ���� ��𼭵� Ŭ������ ���ؼ� type_index �� ���´ٸ� �׻� ���� 
	// type_info Ŭ������ ��� �ȴٰ� �մϴ�.

	// ���� ���� GetTypeIndex �Լ� ���� ��� ���ø� ���ڷ� ���޵� Ÿ�� T�� �޾Ƽ�
	// ������ Ÿ�ӿ� type_index �� ����������,
	// �̷��� �ν��Ͻ�ȭ�� ��ü�� typeid�� �״�� ����־ ���� Ÿ�Կ� �´� 
	// type_index �� ��Ÿ�� �߿� ���� �� �ִٰ� �մϴ�.
	// �� ������ Ÿ�ӿ� Ÿ���� ���Ͽ� �� �� �ְų�
	// ��Ÿ���߿� ��ü�� ���� Ÿ���� ���� �� �� �ְų� �����ε� �մϴ�.
	//std::type_index index = typeid(*component);
	//auto a = index.name(); // a = "class Transform"

	// �׸��� �� ���ٺ��ϱ� ���� �� �Լ����� �׳� ���������� ó���ص� ������ �� ���ƿ� (�������� �Լ�)
	// ����ó���� Transform �� ���ϰ� ��ũ��Ʈ �� ���ΰ� AddComponent<T> �Լ� �ϰ� �Ȱ��ƿ�
	// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
	// 
	// ���ʿ� ���⿡ �����°� ��Ÿ�ӿ� �����°Ŷ� ���ø��� �� �� ����
	// ������ ��Ÿ�ӿ� �ؾ���


	// �ѤѤѤѤѤѤ�  �Լ� ���� 


	std::type_index index = typeid(*component); //�̰� �ش�
	auto iter = m_components.find(index);

	// type_index�� ������Ʈ ��Ͽ� ��ϵǾ� ���� �ʰų� ��ũ��Ʈ��� ������Ʈ �߰�
	// ������Ʈ�� �ߺ��Ǹ� �ȵǰ� ��ũ��Ʈ�� ������ ���� �� �ֱ� ����
	// 
	// Mk.1 ����ĳ���ʹ� ������ �� �� �ִ�.
	if (index == GetTypeIndex<RayCaster>())
	{
		component->SetOwner(shared_from_this());
		m_components.insert(std::make_pair(index, component));
	}
	else if(iter == m_components.end() /*|| index == GetTypeIndex<MonoBehaviour>()*/)
	{
		// Ʈ�������� ���� ����� �� owner�� ����
		// �ٸ� ������Ʈ�� �߰��� �� �ٷ� owner ����
		if (index != GetTypeIndex<Transform>())
		{
			component->SetOwner(shared_from_this());
		}

		m_components.insert(std::make_pair(index, component));
	}
}

void Entity::AddComponent(std::shared_ptr<IBuilder<Component>> builder)
{
	std::shared_ptr<Component> component = builder->Create();
	std::type_index index = typeid(*(component.get()));

	auto iter = m_components.find(index);

	if (iter == m_components.end())
	{
		// Ʈ�������� ���� ����� �� owner�� ����
		// �ٸ� ������Ʈ�� �߰��� �� �ٷ� owner ����
		if (index != GetTypeIndex<Transform>())
		{
			component->SetOwner(shared_from_this());
		}

		m_components.insert(std::make_pair(index, component));
	}
}

std::shared_ptr<Component> Entity::AddComponent(std::shared_ptr<Component> component, std::type_index index)
{
	auto iter = m_components.find(index);

	// Mk.1 �����ɽ��ʹ� ������ �� �� �ִ�.
	if (index == GetTypeIndex<RayCaster>())
	{
		component->SetOwner(shared_from_this());
		m_components.insert(std::make_pair(index, component));
	}
	// �̹� ������ ������Ʈ ���� x 
	else if (iter == m_components.end())
	{
		// Ʈ�������� ���� ����� �� owner�� ����
		// �ٸ� ������Ʈ�� �߰��� �� �ٷ� owner ����
		if (index != GetTypeIndex<Transform>())
		{
			component->SetOwner(shared_from_this());
		}

		m_components.insert(std::make_pair(index, component));
	}

	return component;
}

std::shared_ptr<Component> Entity::GetComponent(std::type_index index)
{
	auto iter = m_components.find(index);

	if (iter != m_components.end())
	{
		return iter->second;
	}

	return nullptr;
}

void Entity::SetManager(std::weak_ptr<IManager> manager)
{
	//�̰� �³�? �ϴ� mk.1

	if (std::dynamic_pointer_cast<SceneManager>(manager.lock()))
	{
		m_sceneManager = std::dynamic_pointer_cast<SceneManager>(manager.lock());
	}
	else if (std::dynamic_pointer_cast<ResourceManager>(manager.lock()))
	{
		m_resourceManager = std::dynamic_pointer_cast<ResourceManager>(manager.lock());
	}
	else if (std::dynamic_pointer_cast<CameraManager>(manager.lock()))
	{
		m_cameraManager = std::dynamic_pointer_cast<CameraManager>(manager.lock());
	}
	else
	{
		//�ش� �Ŵ����� ����
		assert(true, "this manager is not exist in Entity");
	}
}

void Entity::SetGraphics(std::weak_ptr<IGraphics> graphics)
{
	m_graphics = graphics;
}

Entity* Entity::GetChild(std::string name)
{
	for (auto& c : m_children)
	{
		if (c->m_name == name)
			return c;
	}

	return nullptr;
}

std::weak_ptr<Entity> Entity::GetSmartChild(std::string name)
{
	const auto& children = GetComponent<Transform>()->GetChildren();
	for (const auto& child : GetComponent<Transform>()->GetChildren())
	{
		const auto& entity = child->GetOwner().lock();
		if (name == entity->GetName())
			return entity;
	}

	return std::weak_ptr<Entity> {};
}

/// TODO::���� - Clone ��ư (�ڽ� �������� ���� Clone �ǰ� �����) 
//// Transform�� ������������ Clone ��Ű�� ������ Entity���� Scene�� ��Ͻ��Ѿ� �ϰ�,
//// �ڽĵ��� Ŭ�н�Ű�� �ڽĵ��� �θ���� Clone ���·� ������� Entity �̱� ������ 
//// scene, parent �Ű������� �޴´�.
//// �� �Լ��� Transform::CloneWithHierarchy ���� ��������� ȣ����� �ش�.
//Entity* Entity::Clone(Scene* scene, const Transform* parent /*= nullptr*/) const
//{
//	Entity* cloneEntity = new Entity(m_name + " clone");
//	cloneEntity->m_layer = m_layer;
//	cloneEntity->m_activeSelf = m_activeSelf;
//	
//	// �������縦 ���� ������ Ŭ�� �Լ��� ȣ������ش�.
//	// (�� ������Ʈ�� ���纻�� ���� �� ��ȯ)
//	for (const auto& [index, component] : m_components)
//	{
//		if (typeid(*component) != typeid(Transform))
//		{
//			cloneEntity->m_components.insert({ index, component->Clone() });
//		}
//		else
//		{
//			cloneEntity->m_components.insert({ index, dynamic_cast<Transform*>(component)->CloneWithHierarchy(scene, parent) });
//		}
//	}
//
//	return cloneEntity;
//}

void Entity::DestroyComponent(std::type_index index)
{
	auto iter = m_components.find(index);

	if (iter != m_components.end())
	{
		auto& component = iter->second;
		component->Finalize();
		component.reset();
		m_components.erase(iter);
	}
}

void Entity::DestroyAllComponents()
{
	for (auto& iter : m_components)
	{
		iter.second->Finalize();
		iter.second.reset();
	}

	m_components.clear();
}
