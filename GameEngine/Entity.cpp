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

// children 까지 clone 시키기 위해서는 scene 정보를 entity가 들고있게 만들어야한다.
// 일단 복사생성자에서는 계층구조는 복사시키지 않는다. 
// 
// 이 생성자를 사용한다면 생성이 되고 난 이후에 모든 컴포넌트를 순회를 돌며 SetOwner 함수 호출 해줘야 한다.
// (Component 에서는 Entity를 weak_ptr 로 가지고 있기 때문)
Entity::Entity(const std::shared_ptr<Entity>& other)
{
	m_name = other->m_name + " (clone)";
	m_layer = other->m_layer;
	m_layerCollision = other->m_layerCollision;
	m_activeSelf = other->m_activeSelf;
	m_prefabID = other->m_prefabID;

	// 에디터 프리팹 계층구조 형성을 위한 id이다.
	// json 복사생성자 과정에서 깊은 복사를 한다면 값이 엉망으로 바껴서 
	// 얕은 복사로 관리한다.
	m_addressID = other->m_addressID;

	m_IsDestroy = false;
	m_isRender = other->m_isRender;

	// 계층구조는 복사하지 않는다.
	Entity* m_parent = nullptr;
	std::vector<Entity*> m_children{};

	bool IsDestroy = other->m_IsDestroy;

	m_sceneManager = other->m_sceneManager;
	m_resourceManager = other->m_resourceManager;
	m_cameraManager = other->m_cameraManager;
	m_graphics = other->m_graphics;

	// ID 는 고유한 정보기 때문에 복사하지 않는다.
	m_id = m_lastEntityID++;

	// 깊은복사를 위해 각각의 클론 함수를 호출시켜준다.
	// (각 컴포넌트가 복사본을 생성 후 반환)
	for (const auto& [index, component] : other->m_components)
	{
		m_components.insert({ index, component->Clone() });

		const auto& registeredComponent = m_components.find(index)->second;
		if (registeredComponent == nullptr)
		{
			assert(!"Clone 함수가 제대로 구현이 안된 컴포넌트가 있습니다. Clone 함수 내부에서 자기 자신을 만들어내고 반환해야 합니다.");
		}

		registeredComponent->CloneBaseComponentMember(component);
	}
}


void Entity::Start()
{
	for (const auto& iter : m_components)
	{
		//addcomponent를 하면 바로 start가 불리는데 여기서 필요할까?
		iter.second->Start();
	}
}


void Entity::Update(double dt)
{
	// 콜라이더와 렌더러를 분류할 벡터
	std::vector<std::shared_ptr<Component>> colliderComponents;
	std::vector<std::shared_ptr<Component>> rendererComponents;

	// 콜라이더와 렌더러는 업데이트 하지않고 분류 벡터에 차곡차곡 담는다.
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

	// 콜라이더 업데이트
	for (const auto& collider : colliderComponents)
	{
		collider->Update(dt);
	}

	// 렌더러 업데이트
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
		// Script 다이나믹 캐스팅 성공하면 함수 호출해주기.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc) 
			sc->OnCollisionEnter(inCollision);
	}
}

void Entity::OnCollisionStay(const Collision& inCollision)
{
	for (auto& comp : m_components)
	{
		// Script 다이나믹 캐스팅 성공하면 함수 호출해주기.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnCollisionStay(inCollision);
	}
}

void Entity::OnCollisionExit(const Collision& inCollision)
{
	for (auto& comp : m_components)
	{
		// Script 다이나믹 캐스팅 성공하면 함수 호출해주기.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnCollisionExit(inCollision);
	}
}

void Entity::OnTriggerEnter(Collider* otherCollider)
{
	for (auto& comp : m_components)
	{
		// Script 다이나믹 캐스팅 성공하면 함수 호출해주기.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnTriggerEnter(otherCollider);
	}
}

void Entity::OnTriggerStay(Collider* otherCollider)
{
	for (auto& comp : m_components)
	{
		// Script 다이나믹 캐스팅 성공하면 함수 호출해주기.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnTriggerStay(otherCollider);
	}
}

void Entity::OnTriggerExit(Collider* otherCollider)
{
	for (auto& comp : m_components)
	{
		// Script 다이나믹 캐스팅 성공하면 함수 호출해주기.
		std::shared_ptr<Script> sc = std::dynamic_pointer_cast<Script>(comp.second);
		if (sc)
			sc->OnTriggerExit(otherCollider);
	}
}

void Entity::SetLayerCollision(const LayerCollision n)
{
	m_layerCollision = n;

	// 콜라이더가 여러개 일 것을 대비해 일단 따로따로 처리하는 것으로 해놓았다.
	std::shared_ptr<BoxCollider> boxCollider = GetComponent<BoxCollider>();
	std::shared_ptr<SphereCollider> sphereCollider = GetComponent<SphereCollider>();
	std::shared_ptr<CapsuleCollider> capsuleCollider = GetComponent<CapsuleCollider>();

	if (boxCollider) boxCollider->SetLayerCollision(n);
	if (sphereCollider) sphereCollider->SetLayerCollision(n);
	if (capsuleCollider) capsuleCollider->SetLayerCollision(n);
}

void Entity::AddComponent(std::shared_ptr<Component> component)
{

	//TODO:여기 해야 연하가 할 수 있어
	//런타임이든 컴파일 타임이든 실객체의 타입이 뭔지 알아야 index를 찾을거다
	//std::type_index index = GetTypeIndex<decltype(component)>();
	//m_components.insert(make_pair(index, component));


	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	// Dongil  
	// type_index 는 type_info 클래스를 래핑하는 클래스라고 합니다
	// 그리고 언제 어디서든 클래스에 대해서 type_index 를 얻어온다면 항상 같은 
	// type_info 클래스를 얻게 된다고 합니다.

	// 제가 쓰던 GetTypeIndex 함수 같은 경우 템플릿 인자로 전달된 타입 T를 받아서
	// 컴파일 타임에 type_index 를 생성했지만,
	// 이렇게 인스턴스화된 객체를 typeid에 그대로 집어넣어도 실제 타입에 맞는 
	// type_index 를 런타임 중에 가질 수 있다고 합니다.
	// 즉 컴파일 타임에 타입을 통하여 알 수 있거나
	// 런타임중에 객체의 실제 타입을 통해 알 수 있거나 차이인듯 합니다.
	//std::type_index index = typeid(*component);
	//auto a = index.name(); // a = "class Transform"

	// 그리고 좀 보다보니까 현재 이 함수에서 그냥 독립적으로 처리해도 괜찮을 것 같아요 (빌더전용 함수)
	// 예외처리도 Transform 일 때하고 스크립트 때 뿐인건 AddComponent<T> 함수 하고 똑같아요
	// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	// 
	// 애초에 저기에 들어오는게 런타임에 들어오는거라 템플릿을 할 수 없음
	// 무조건 런타임에 해야함


	// ㅡㅡㅡㅡㅡㅡㅡ  함수 구현 


	std::type_index index = typeid(*component); //이게 해답
	auto iter = m_components.find(index);

	// type_index가 컴포넌트 목록에 등록되어 있지 않거나 스크립트라면 컴포넌트 추가
	// 컴포넌트는 중복되면 안되고 스크립트는 여러개 넣을 수 있기 때문
	// 
	// Mk.1 레이캐스터는 여러개 들어갈 수 있다.
	if (index == GetTypeIndex<RayCaster>())
	{
		component->SetOwner(shared_from_this());
		m_components.insert(std::make_pair(index, component));
	}
	else if(iter == m_components.end() /*|| index == GetTypeIndex<MonoBehaviour>()*/)
	{
		// 트랜스폼은 씬에 등록할 때 owner를 설정
		// 다른 컴포넌트는 추가할 때 바로 owner 설정
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
		// 트랜스폼은 씬에 등록할 때 owner를 설정
		// 다른 컴포넌트는 추가할 때 바로 owner 설정
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

	// Mk.1 레이케스터는 여러개 들어갈 수 있다.
	if (index == GetTypeIndex<RayCaster>())
	{
		component->SetOwner(shared_from_this());
		m_components.insert(std::make_pair(index, component));
	}
	// 이미 생성한 컴포넌트 생성 x 
	else if (iter == m_components.end())
	{
		// 트랜스폼은 씬에 등록할 때 owner를 설정
		// 다른 컴포넌트는 추가할 때 바로 owner 설정
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
	//이거 맞냐? 일단 mk.1

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
		//해당 매니저가 없음
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

/// TODO::동일 - Clone 버튼 (자식 계층구조 까지 Clone 되게 만들기) 
//// Transform은 계층구조까지 Clone 시키고 생성된 Entity들을 Scene에 등록시켜야 하고,
//// 자식들을 클론시키고 자식들의 부모또한 Clone 형태로 만들어진 Entity 이기 때문에 
//// scene, parent 매개변수를 받는다.
//// 이 함수는 Transform::CloneWithHierarchy 에서 재귀적으로 호출시켜 준다.
//Entity* Entity::Clone(Scene* scene, const Transform* parent /*= nullptr*/) const
//{
//	Entity* cloneEntity = new Entity(m_name + " clone");
//	cloneEntity->m_layer = m_layer;
//	cloneEntity->m_activeSelf = m_activeSelf;
//	
//	// 깊은복사를 위해 각각의 클론 함수를 호출시켜준다.
//	// (각 컴포넌트가 복사본을 생성 후 반환)
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
