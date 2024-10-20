#include "GameEngine_pch.h"
#include "EventManager.h"
#include "Scene.h"
#include "Component.h"
#include "Collider.h"
#include "Entity.h"

#include "../GameDemo/FoodTableScript.h"
#include "../GameDemo/InterKeyScript.h"
#include "../GameDemo/ShopKeeperFSM.h"
#include "../GameDemo/UITestScript.h"

void EventManager::Start()
{

}

void EventManager::Update()
{
	if (!m_createQueue.empty())
	{
		for (auto entity : m_createQueue)
		{
			m_curScene->AddEntity(entity);
			entity->Start();
		}

		m_createQueue.clear();
	}

	if (!m_destroyQueue.empty())
	{
		for (auto entity : m_destroyQueue)
		{
			m_curScene->DestroyEntity(entity);
		}

		m_destroyQueue.clear();
	}

	if (!m_waitDestroyQueue.empty())
	{
		for (auto entity : m_waitDestroyQueue)
		{
			if (entity->GetComponent<Collider>() != nullptr)
			{
				//entity->GetComponent<Collider>()->is
			}

			m_destroyQueue.emplace_back(entity);
			m_waitDestroyQueue.erase(entity);
		}

		m_waitDestroyQueue.clear();
	}


	if (!m_SceneQueue.empty())
	{
		for (auto scene : m_SceneQueue)
		{
			m_curScene->Exit();
			m_curScene = scene;
			//m_curScene->Start();
		}

		m_SceneQueue.clear();
	}

	/// 이 부분은 무조건 m_SceneQueue 이벤트가 발동한 뒤에 호출 되어야 함.
	// 디시리얼라이즈를 통해 만들어낸 컴포넌트들은 start 함수, 해당하는 scene, 트랜스폼에 의존한다.
	// 그러므로 의존되는 친구들을 올바르게 매칭되는 다음 프레임에서 실행시켜준다.
	if (!m_ComponentQueue.empty() && m_SceneQueue.empty())
	{
		for (const auto& component : m_ComponentQueue)
		{
			// 콜라이더인 경우 scene을 새로 배정 해줘야한다.
			if (dynamic_pointer_cast<Collider>(component))
			{
				static_pointer_cast<Collider>(component)->SetScene(m_curScene);
			}

			component->Start();
		}

		// 상점 관련 포인터 연결... 이걸 여기서..
		StoreAndAnyPointerConnection(m_curScene);

		m_ComponentQueue.clear();
	}
}

void EventManager::AddEvent(std::shared_ptr<Scene> scene, std::shared_ptr <Entity> event, EventType type)
{
	m_curScene = scene;

	switch (type)
	{
	case EventType::WaitCreate:
		break;
	case EventType::Create:
		m_createQueue.push_back(event);
		break;
	case EventType::WaitDestory:
		m_waitDestroyQueue.insert(event);
		break;
	case EventType::Destory:
		m_destroyQueue.push_back(event);
		break;
	case EventType::SceneChange:
		break;
	case EventType::GameOver:
		break;

	default:
		break;
	}
}

void EventManager::AddEvent(std::shared_ptr <Entity> event, EventType type)
{
	AddEvent(m_curScene, event, type);
}

void EventManager::NextFrameStartComponents(std::vector<std::shared_ptr<Component>> components)
{
	for (const auto& component : components)
	{
		m_ComponentQueue.push_back(component);
	}
}

void EventManager::SceneChange(std::shared_ptr<Scene> scene)
{
	m_SceneQueue.push_back(scene);
}

void EventManager::StoreAndAnyPointerConnection(std::shared_ptr<Scene> scene)
{
	// 상점과 관련된 친구들의 포인터 연결을 하기 위함.
	std::vector<std::shared_ptr<ShopKeeperFSM>> shopKeeperFSMs;
	std::vector<std::shared_ptr<FoodTableScript>> foodTableScripts;
	std::vector<std::shared_ptr<InterKeyScript>> interKeyScripts;
	for (const auto& entity : scene->GetEntities())
	{
		if (entity->HasComponent<ShopKeeperFSM>())
		{
			shopKeeperFSMs.push_back(entity->GetComponent<ShopKeeperFSM>());
		}
		if (entity->HasComponent<FoodTableScript>())
		{
			foodTableScripts.push_back(entity->GetComponent<FoodTableScript>());
		}
		if (entity->HasComponent<InterKeyScript>())
		{
			interKeyScripts.push_back(entity->GetComponent<InterKeyScript>());
		}
		if (entity->HasComponent<UITestScript>())
		{
			entity->GetComponent<UITestScript>()->SetPlayerPointer(scene->GetEntity("Player"));
		}
	}

	// 상인과 푸드 테이블 연결
	for (const auto& shopKeeperFSM : shopKeeperFSMs)
	{
		for (const auto& foodTableScript : foodTableScripts)
		{
			// 상인FSM과 푸드 테이블의 부모가 같다면 동일한 음식가게의 자식이라는 뜻
			if (shopKeeperFSM->GetTransform()->GetParent().lock() == foodTableScript->GetTransform()->GetParent().lock())
			{
				foodTableScript->SetPointerSense(shopKeeperFSM->GetPointerSense());
				break;
			}
		}
	}

	// 푸드 테이블과 상호작용 키 연결
	for (const auto& foodTableScript : foodTableScripts)
	{
		for (const auto& interKeyScript : interKeyScripts)
		{
			// 푸드 테이블과 인터키의 부모가 같다면 동일한 음식가게의 자식이라는 뜻
			if (foodTableScript->GetTransform() == interKeyScript->GetTransform()->GetParent().lock())
			{
				interKeyScript->m_foodTableScript = foodTableScript;
				break;
			}
		}
	}
}
