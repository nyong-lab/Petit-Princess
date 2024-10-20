#include "GameEngine_pch.h"
#include "CollisionManagerSeoYoung.h"
#include "EntityDefine.h"
#include "BoxCollider.h"
#include "Entity.h"

// Layer üũ�� �ͳ����� �浹�Ѵ� 
void CollisionManagerSeoYoung::Initialize()
{
	for(UINT i = 0; i < static_cast<UINT>(LayerCollision::End); ++i)
	{
		for(UINT j = 0; j < static_cast<UINT>(LayerCollision::End); ++j)
		{
			m_layerCollisionMatrix[i][j] = false;
		}
	}

	// TODO: Init���� LayerCollisionMatrix �����ϰ�
	SetLayerCollisionMatrix(LayerCollision::Player, LayerCollision::Ground);
	SetLayerCollisionMatrix(LayerCollision::Player, LayerCollision::Wall);
	SetLayerCollisionMatrix(LayerCollision::Player, LayerCollision::Enemy);
	SetLayerCollisionMatrix(LayerCollision::Player, LayerCollision::Interactable);
	SetLayerCollisionMatrix(LayerCollision::Player, LayerCollision::Coin);
	SetLayerCollisionMatrix(LayerCollision::Player, LayerCollision::Heart);
	
	SetLayerCollisionMatrix(LayerCollision::Enemy, LayerCollision::Ground);
	SetLayerCollisionMatrix(LayerCollision::Enemy, LayerCollision::Wall);
}

void CollisionManagerSeoYoung::FixedUpdate(double fixed_dt)
{
	// Layer ���� �浹 üũ
	for (UINT i = 0; i < static_cast<UINT>(LayerCollision::End); ++i)
	{
		// TODO: ���� ���̾�� �浹 üũ? �ʿ��ϸ� �ϱ�
		for (UINT j = i + 1; j < static_cast<UINT>(LayerCollision::End); ++j)
		{
			if (m_layerCollisionMatrix[i][j])
			{
				FixedUpdateLayer(static_cast<LayerCollision>(i), static_cast<LayerCollision>(j));
			}
		}
	}
}

/// <summary>
/// Entity LayerCollision collider �߰� + ID �ο�
/// </summary>
/// <param name="collider"></param>
void CollisionManagerSeoYoung::AddCollider(class BoxCollider* collider)
{
	// ID �ο�
	collider->SetColliderID(m_colliderIDToAssign++);

	// �ش� Layer�� collider �߰�
	LayerCollision layer = collider->GetOwner().lock()->GetLayerCollision();
	UINT layerIndex = static_cast<UINT>(layer);
	m_boxColliders[layerIndex].push_back(collider);
}

/// <summary>
/// Entity LayerCollision collider ����
/// </summary>
/// <param name="collider"></param>
void CollisionManagerSeoYoung::RemoveCollider(class BoxCollider* collider)
{
	// �ش� Layer�� collider ����
	LayerCollision layer = collider->GetOwner().lock()->GetLayerCollision();
	UINT layerIndex = static_cast<UINT>(layer);
	m_boxColliders[layerIndex].erase(std::remove(m_boxColliders[layerIndex].begin(), m_boxColliders[layerIndex].end(), collider), m_boxColliders[layerIndex].end());
}

void CollisionManagerSeoYoung::SetLayerCollisionMatrix(LayerCollision layer1, LayerCollision layer2)
{
	m_layerCollisionMatrix[static_cast<UINT>(layer1)][static_cast<UINT>(layer2)] = true;
	m_layerCollisionMatrix[static_cast<UINT>(layer2)][static_cast<UINT>(layer1)] = true;
}

void CollisionManagerSeoYoung::FixedUpdateLayer(LayerCollision layer1, LayerCollision layer2)
{
	// Layer1�� Collider��� Layer2�� Collider���� ��
	for (auto& collider1 : m_boxColliders[static_cast<UINT>(layer1)])
	{
		for (auto& collider2 : m_boxColliders[static_cast<UINT>(layer2)])
		{
			CollisionID collisionID = { collider1->GetColliderID(), collider2->GetColliderID() };

			// TODO: Entity isActive == false -> continue;
			// TODO: Entity isDestroyed == true -> continue;

			// #�浹���� ��� && ���� �����ӿ� �浹���� �ʾ��� ��� -> OnCollisionEnter
			if (collider1->IsIntersect(*collider2) == true && m_mapCollisionInfo[collisionID.IDs] == false)
			{
				collider1->OnTriggerEnter(*collider2);
				collider2->OnTriggerEnter(*collider1);
				m_mapCollisionInfo[collisionID.IDs] = true;
			}
			// #�浹���� ��� && ���� �����ӿ� �浹 -> OnCollisionStay
			else if (collider1->IsIntersect(*collider2) == true && m_mapCollisionInfo[collisionID.IDs] == true)
			{
				collider1->OnTriggerStay(*collider2);
				collider2->OnTriggerStay(*collider1);
			}
			// #�浹���� �ʾ��� ��� && ���� �����ӿ� �浹 -> OnTriggerExit
			else if (collider1->IsIntersect(*collider2) == false && m_mapCollisionInfo[collisionID.IDs] == true)
			{
				collider1->OnTriggerExit(*collider2);
				collider2->OnTriggerExit(*collider1);
				m_mapCollisionInfo[collisionID.IDs] = false;
			}
		}
	}
}
