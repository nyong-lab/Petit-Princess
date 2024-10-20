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
	//componenet ����
	//UserScript?
	//transform - �θ𿡼� �ڽ� ��ƼƼ�� transform���� ������Ʈ �ع�����
	//collider
	//render

	//��ƼƼ ���� ������Ʈ ���� ������Ʈ�� �������Ѵ�
	//�ش� ������Ʈ���� ��� ������ �ִ���? -> ��ƼƼ ���ο��� ������Ʈ ������ �����൵ �ȴٰ�� ����
	//������Ʈ ť�� �־���Ұ� ������?

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

// ��ƼƼ�� �޾Ƽ� ���� �߰����ش�.
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

		/// ���� �¼¼� 
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
		/// ���� �¼¼� 
		lockedEntity->SetManager(m_sceneManager.lock());
		lockedEntity->SetManager(m_resourceManager.lock());
		lockedEntity->SetManager(m_cameraManager.lock());
		lockedEntity->SetGraphics(m_graphics.lock());
	}
}

// �Ű������� �̸��� �θ� ��ƼƼ�� �޴´�. (��ƿ �Լ�)
// 1. ��ƼƼ �����
// 2. �������� ����
// 3. ������ �����ϴ� ��ƼƼ ��Ͽ� �߰�
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

// �Ű������� ���� ��ƼƼ�� �޴´�. (��ƿ �Լ�)
// 1. ���ο� ��ƼƼ ��������� ȣ��
// 2. ������ �����ϴ� ��ƼƼ ��Ͽ� �߰�
void Scene::CreateCloneAddEntity(std::weak_ptr<Entity> baseEntity)
{
	std::shared_ptr<Entity> cloneEntity;

	// ��ƼƼ�� ��ӹ޴� Ŭ������ �þ ��� ���⼭ �÷���� ��
	std::shared_ptr<TextureBox> textureBox = std::dynamic_pointer_cast<TextureBox>(baseEntity.lock());

	// �ؽ��� �ڽ�
	if (textureBox)
	{
		// TextureBox�� ��ȯ ������ ���
		cloneEntity = std::make_shared<TextureBox>(textureBox);
	}

	//else if(��ƼƼ�� ��ӹ޴� ���ο� Ŭ����)
	//{
	//	cloneEntity = std::make_shared<��ƼƼ�� ��ӹ޴� ���ο� Ŭ����>();
	//}

	else
	{
		// �⺻���� Entity Ŭ���� 
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
			assert(!"������Ʈ ������ ����� �̷������ �ʾҽ��ϴ�.");
		}
	}

	// �޽� ���͸� ��� �ִٸ� �ش� �� Ű������ �а� ���Ѿ��Ѵ�.
	if (cloneEntity->HasComponent<MeshFilter>())
	{
		cloneEntity->GetComponent<MeshFilter>()->UpdateModelKey();
	}

	// �ڽ� ������������ �����ϴ� Clone �Լ� (���� �̱���)
	//Entity* cloneEntity = baseEntity->Clone(this);

	m_entities.push_back(cloneEntity);
}

// �Ű������� ���� ��ƼƼ�� �ı�
void Scene::DestroyEntity(std::weak_ptr<Entity> entity)
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		if (m_entities[i] == entity.lock())
		{
			// ������ �ڽĵ��� ������ ���Ϳ� ��ƼƼ�� �����ϴ� �迭
			std::vector<std::shared_ptr<Entity>> childEntities;

			// ������ ��ƼƼ�� �ڽĵ��� �����´�.
			const auto& children = entity.lock()->GetComponent<Transform>()->GetChildren();

			// ������ �ڽĵ��� ��ȸ�ϸ� ������ ��ƼƼ �迭�� ���� �ִ´�.
			for (const auto& child : children)
			{
				childEntities.push_back(child->GetOwner().lock());
			}

			// ����� �ڽ� ����Ƽ ����
			for (const auto& childEntity : childEntities)
			{
				DestroyEntity(childEntity);
			}

			// ��� �ڽ��� �����ߴٸ� �ڱ� �ڽ��� ������
			entity.lock()->Finalize();
				
			// ��ƼƼ�� �׳� �����ϱ� ���� �ش� ��ƼƼ�� ã�Ƽ� m_entities ���� �������� �̵� ��Ų��.
			auto iter = std::remove_if(m_entities.begin(), m_entities.end(),
				[entity](const std::shared_ptr<Entity>& e) { return e == entity.lock(); });

			// iter ��ġ�� �ش��ϴ� ������ ������ ���κ� ���� ������Ų��.
			m_entities.erase(iter, m_entities.end());

			break;
		}
	}
}

// ���� ��ϵ� ��� ��ƼƼ �ı�
void Scene::DestroyAllEntity()
{
	/*for (auto& entity : m_entities)
	{
		entity->Finalize();
		entity.reset();
	}

	m_entities.clear();*/

	/// Ʈ���������� �������� child �迭�� �ٲ��� �ڲ� ������ ������ ���ĺô�..
	/// �̰� �³�

	// ������ ��ƼƼ�� �ε����� �����ϴ� �迭
	std::vector<size_t> entitiesToRemove;

	// ��ƼƼ���� �������� ��ȸ�ϸ� �ı�
	for (size_t i = 0; i < m_entities.size(); ++i)
	{
		std::weak_ptr<Entity> entity = m_entities[i];

		// ������ �ڽĵ��� ������ ���Ϳ� ��ƼƼ�� �����ϴ� �迭
		std::vector<std::shared_ptr<Entity>> childEntities;

		// ������ ��ƼƼ�� �ڽĵ��� �����´�.
		const auto& children = entity.lock()->GetComponent<Transform>()->GetChildren();

		// ������ �ڽĵ��� ��ȸ�ϸ� ������ ��ƼƼ �迭�� ���� �ִ´�.
		for (const auto& child : children)
		{
			childEntities.push_back(child->GetOwner().lock());
		}

		// ����� �ڽ� ����Ƽ ����
		for (const auto& childEntity : childEntities)
		{
			DestroyEntity(childEntity);
		}

		// ��� �ڽ��� �����ߴٸ� �ش� ��ƼƼ�� �ε����� ���
		if (!entity.expired()) // weak_ptr�� ��ȿ���� Ȯ��
		{
			entity.lock()->Finalize();
			entitiesToRemove.push_back(i);
		}
	}

	// ��ƼƼ�� ���Ϳ��� ���� (�������� �����ؾ� �ε��� ��ȭ�� ���� ������ �پ��)
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
	//�̰� �³�? �ϴ� mk.1

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
		//�ش� �Ŵ����� ����
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
	
	// ��� ȿ�������� üũ�ұ�
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
