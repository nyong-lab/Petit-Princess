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

	/// �� �κ��� ������ m_SceneQueue �̺�Ʈ�� �ߵ��� �ڿ� ȣ�� �Ǿ�� ��.
	// ��ø������� ���� ���� ������Ʈ���� start �Լ�, �ش��ϴ� scene, Ʈ�������� �����Ѵ�.
	// �׷��Ƿ� �����Ǵ� ģ������ �ùٸ��� ��Ī�Ǵ� ���� �����ӿ��� ��������ش�.
	if (!m_ComponentQueue.empty() && m_SceneQueue.empty())
	{
		for (const auto& component : m_ComponentQueue)
		{
			// �ݶ��̴��� ��� scene�� ���� ���� ������Ѵ�.
			if (dynamic_pointer_cast<Collider>(component))
			{
				static_pointer_cast<Collider>(component)->SetScene(m_curScene);
			}

			component->Start();
		}

		// ���� ���� ������ ����... �̰� ���⼭..
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
	// ������ ���õ� ģ������ ������ ������ �ϱ� ����.
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

	// ���ΰ� Ǫ�� ���̺� ����
	for (const auto& shopKeeperFSM : shopKeeperFSMs)
	{
		for (const auto& foodTableScript : foodTableScripts)
		{
			// ����FSM�� Ǫ�� ���̺��� �θ� ���ٸ� ������ ���İ����� �ڽ��̶�� ��
			if (shopKeeperFSM->GetTransform()->GetParent().lock() == foodTableScript->GetTransform()->GetParent().lock())
			{
				foodTableScript->SetPointerSense(shopKeeperFSM->GetPointerSense());
				break;
			}
		}
	}

	// Ǫ�� ���̺�� ��ȣ�ۿ� Ű ����
	for (const auto& foodTableScript : foodTableScripts)
	{
		for (const auto& interKeyScript : interKeyScripts)
		{
			// Ǫ�� ���̺�� ����Ű�� �θ� ���ٸ� ������ ���İ����� �ڽ��̶�� ��
			if (foodTableScript->GetTransform() == interKeyScript->GetTransform()->GetParent().lock())
			{
				interKeyScript->m_foodTableScript = foodTableScript;
				break;
			}
		}
	}
}
