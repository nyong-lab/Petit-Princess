#include "GameEngine_pch.h"
#include "Scene.h"
#include "Entity.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "../GameDemo/TextureBox.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "CollisionManager.h"
#include "InputManager.h"

#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

Scene::Scene(std::wstring sceneName /*= L""*/)
	: m_sceneName(sceneName)
	//, m_isSimulating(true), m_isPrevSimulating(true),m_isDebug(false)
	, m_isSimulating(false), m_isPrevSimulating(false),m_isDebug(false)
{

}

void Scene::Start()
{
	for (const auto& entity : m_entities)
	{
		entity->Start();
	}
}

void Scene::Initialize(std::shared_ptr<IGraphics> graphics, int count, std::shared_ptr<IManager> manager...)
{
	va_list arg;

	this->SetGraphics(graphics);


	va_start(arg, manager);
	this->SetManager(manager);

	for (int i = 0; i < count - 1; i++)
	{
		this->SetManager((va_arg(arg, std::shared_ptr<IManager>)));
	}

	va_end(arg);

	return;
}

void Scene::Exit()
{
	DestroyAllEntity();
	m_entities.clear();
}

void Scene::Update(double dt)
{
	//componenet 순서
	//UserScript?
	//transform - 부모에서 자식 엔티티의 transform까지 업데이트 해버린다
	//collider
	//render

	//엔티티 안의 컴포넌트 별로 업데이트를 돌려야한다
	//해당 컴포넌트별로 묶어서 가지고 있던가? -> 엔티티 내부에서 업데이트 순서만 맞춰줘도 된다고는 했음
	//업데이트 큐가 있어야할거 같은데?

	if (InputManager::GetInstance()->IsKeyDown(VK_F1))
	{
		m_isDebug = !m_isDebug;
	}


	for (const auto& entity : m_entities)
	{
		entity->Update(dt);
	}

	for (const auto& entity : m_entities)
	{
		entity->GetComponent<Transform>()->SetIsUpdated(false);
	}
}

void Scene::FixedUpdate(double fixedTime)
{
	for (const auto& entity : m_entities)
	{
		entity->FixedUpdate(fixedTime);
	}
}

void Scene::Render(double dt)
{
	for (const auto& entity : m_entities)
	{
		entity->Render(dt);
	}
}

void Scene::DebugRender()
{
	for (const auto& entity : m_entities)
	{
		entity->DebugRender();
	}
}

void Scene::Finalize()
{
	DestroyAllEntity();
	m_entities.clear();
}

// 엔티티를 받아서 씬에 추가해준다.
void Scene::AddEntity(std::weak_ptr<Entity> entity)
{
	auto lockedEntity = entity.lock();
	if (lockedEntity)
	{
		auto transform = lockedEntity->GetComponent<Transform>();
		if (transform->GetOwner().lock() == nullptr)
		{
			transform->SetOwner(lockedEntity);
		}

		/// 괴물 셋셋셋 
		lockedEntity->SetManager(m_sceneManager.lock());
		lockedEntity->SetManager(m_resourceManager.lock());
		lockedEntity->SetManager(m_cameraManager.lock());
		lockedEntity->SetGraphics(m_graphics.lock());

		m_entities.push_back(lockedEntity);
	}
}

void Scene::SetEntityManager(std::weak_ptr<Entity> entity)
{
	auto lockedEntity = entity.lock();
	if (lockedEntity)
	{
		/// 괴물 셋셋셋 
		lockedEntity->SetManager(m_sceneManager.lock());
		lockedEntity->SetManager(m_resourceManager.lock());
		lockedEntity->SetManager(m_cameraManager.lock());
		lockedEntity->SetGraphics(m_graphics.lock());
	}
}

// 매개변수로 이름과 부모 엔티티를 받는다. (유틸 함수)
// 1. 엔티티 만들기
// 2. 계층구조 정리
// 3. 씬에서 관리하는 엔티티 목록에 추가
void Scene::CreateAddHierarchyEntity(std::string name, std::shared_ptr<Entity> parent /*= nullptr*/)
{
	if (parent)
	{
		std::shared_ptr<Entity> child = std::make_shared<Entity>(name);
		child->GetComponent<Transform>()->SetParent(parent->GetComponent<Transform>());

		AddEntity(child);
	}
	else
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
		AddEntity(entity);
	}
}

// 매개변수로 원본 엔티티를 받는다. (유틸 함수)
// 1. 새로운 엔티티 복사생성자 호출
// 2. 씬에서 관리하는 엔티티 목록에 추가
void Scene::CreateCloneAddEntity(std::weak_ptr<Entity> baseEntity)
{
	std::shared_ptr<Entity> cloneEntity;

	// 엔티티를 상속받는 클래스가 늘어날 경우 여기서 늘려줘야 함
	std::shared_ptr<TextureBox> textureBox = std::dynamic_pointer_cast<TextureBox>(baseEntity.lock());

	// 텍스쳐 박스
	if (textureBox)
	{
		// TextureBox로 변환 가능한 경우
		cloneEntity = std::make_shared<TextureBox>(textureBox);
	}

	//else if(엔티티를 상속받는 새로운 클래스)
	//{
	//	cloneEntity = std::make_shared<엔티티를 상속받는 새로운 클래스>();
	//}

	else
	{
		// 기본적인 Entity 클래스 
		cloneEntity = std::make_shared<Entity>(baseEntity.lock());
	}

	for (auto& iter : cloneEntity->GetComponents())
	{
		if (iter.second)
		{
			iter.second->SetOwner(cloneEntity);
		}
		else
		{
			assert(!"컴포넌트 복제가 제대로 이루어지지 않았습니다.");
		}
	}

	// 메쉬 필터를 들고 있다면 해당 모델 키값으로 둔갑 시켜야한다.
	if (cloneEntity->HasComponent<MeshFilter>())
	{
		cloneEntity->GetComponent<MeshFilter>()->UpdateModelKey();
	}

	// 자식 계층구조까지 복사하는 Clone 함수 (아직 미구현)
	//Entity* cloneEntity = baseEntity->Clone(this);

	m_entities.push_back(cloneEntity);
}

// 매개변수로 들어온 엔티티를 파괴
void Scene::DestroyEntity(std::weak_ptr<Entity> entity)
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		if (m_entities[i] == entity.lock())
		{
			// 삭제할 자식들을 별도의 벡터에 엔티티로 저장하는 배열
			std::vector<std::shared_ptr<Entity>> childEntities;

			// 삭제할 엔티티의 자식들을 가져온다.
			const auto& children = entity.lock()->GetComponent<Transform>()->GetChildren();

			// 삭제할 자식들을 순회하며 삭제할 엔티티 배열에 집어 넣는다.
			for (const auto& child : children)
			{
				childEntities.push_back(child->GetOwner().lock());
			}

			// 저장된 자식 엔터티 제거
			for (const auto& childEntity : childEntities)
			{
				DestroyEntity(childEntity);
			}

			// 모든 자식을 제거했다면 자기 자신을 마무리
			entity.lock()->Finalize();
				
			// 엔티티를 그냥 삭제하기 전에 해당 엔티티를 찾아서 m_entities 벡터 끝쪽으로 이동 시킨다.
			auto iter = std::remove_if(m_entities.begin(), m_entities.end(),
				[entity](const std::shared_ptr<Entity>& e) { return e == entity.lock(); });

			// iter 위치에 해당하는 곳부터 벡터의 끝부분 까지 삭제시킨다.
			m_entities.erase(iter, m_entities.end());

			break;
		}
	}
}

// 씬에 등록된 모든 엔티티 파괴
void Scene::DestroyAllEntity()
{
	/*for (auto& entity : m_entities)
	{
		entity->Finalize();
		entity.reset();
	}

	m_entities.clear();*/

	/// 트랜스폼에서 동적으로 child 배열이 바껴서 자꾸 터져서 열심히 고쳐봤다..
	/// 이게 맞나

	// 삭제할 엔티티의 인덱스를 저장하는 배열
	std::vector<size_t> entitiesToRemove;

	// 엔티티들을 역순으로 순회하며 파괴
	for (size_t i = 0; i < m_entities.size(); ++i)
	{
		std::weak_ptr<Entity> entity = m_entities[i];

		// 삭제할 자식들을 별도의 벡터에 엔티티로 저장하는 배열
		std::vector<std::shared_ptr<Entity>> childEntities;

		// 삭제할 엔티티의 자식들을 가져온다.
		const auto& children = entity.lock()->GetComponent<Transform>()->GetChildren();

		// 삭제할 자식들을 순회하며 삭제할 엔티티 배열에 집어 넣는다.
		for (const auto& child : children)
		{
			childEntities.push_back(child->GetOwner().lock());
		}

		// 저장된 자식 엔터티 제거
		for (const auto& childEntity : childEntities)
		{
			DestroyEntity(childEntity);
		}

		// 모든 자식을 제거했다면 해당 엔티티의 인덱스를 기록
		if (!entity.expired()) // weak_ptr이 유효한지 확인
		{
			entity.lock()->Finalize();
			entitiesToRemove.push_back(i);
		}
	}

	// 엔티티를 벡터에서 제거 (역순으로 삭제해야 인덱스 변화에 따른 문제가 줄어듦)
	for (auto it = entitiesToRemove.rbegin(); it != entitiesToRemove.rend(); ++it)
	{
		m_entities.erase(m_entities.begin() + *it);
	}
}

std::shared_ptr<Entity> Scene::GetEntity(std::string name)
{
	for (const auto& entity : m_entities)
	{
		if (entity->GetName() == name)
		{
			return entity;
		}
	}

	return nullptr;
}

void Scene::SetManager(std::shared_ptr<IManager> manager)
{
	//이거 맞냐? 일단 mk.1

	if (std::dynamic_pointer_cast<SceneManager>(manager))
	{
		m_sceneManager = std::dynamic_pointer_cast<SceneManager>(manager);
	}
	else if (std::dynamic_pointer_cast<ResourceManager>(manager))
	{
		m_resourceManager = std::dynamic_pointer_cast<ResourceManager>(manager);
	}
	else if (std::dynamic_pointer_cast<CameraManager>(manager))
	{
		m_cameraManager = std::dynamic_pointer_cast<CameraManager>(manager);
	}
	else
	{
		//해당 매니저가 없음
		assert(true, "this manager is not exist in Entity");
	}
}

void Scene::SetGraphics(std::shared_ptr<IGraphics> graphics)
{
	m_graphics = graphics;
}

void Scene::SetSimulating(bool val)
{
	m_isSimulating = val;
	
	// 어떻게 효과적으로 체크할까
	for (const auto& entity : m_entities)
	{
		const auto& components = entity->GetComponents();

		auto iter = components.find(GetTypeIndex<BoxCollider>());
		if (iter != components.end())
		{
			std::static_pointer_cast<BoxCollider>(iter->second)->UpdatePhysXProperties();
		}
		iter = components.find(GetTypeIndex<SphereCollider>());
		if (iter != components.end())
		{
			std::static_pointer_cast<SphereCollider>(iter->second)->UpdatePhysXProperties();
		}
		iter = components.find(GetTypeIndex<CapsuleCollider>());
		if (iter != components.end())
		{
			std::static_pointer_cast<CapsuleCollider>(iter->second)->UpdatePhysXProperties();
		}
		iter = components.find(GetTypeIndex<PlaneCollider>());
		if (iter != components.end())
		{
			std::static_pointer_cast<PlaneCollider>(iter->second)->UpdatePhysXProperties();
		}
	}

	m_isPrevSimulating = m_isSimulating;
}

void Scene::SetPrevSimulating(bool val)
{
	m_isPrevSimulating = val;
}
