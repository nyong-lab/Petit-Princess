#include "GameEngine_pch.h"
#include "CollisionManagerSeoYoung.h"
#include "EntityDefine.h"
#include "BoxCollider.h"
#include "Entity.h"

// Layer 체크한 것끼리만 충돌한다 
void CollisionManagerSeoYoung::Initialize()
{
	for(UINT i = 0; i < static_cast<UINT>(LayerCollision::End); ++i)
	{
		for(UINT j = 0; j < static_cast<UINT>(LayerCollision::End); ++j)
		{
			m_layerCollisionMatrix[i][j] = false;
		}
	}

	// TODO: Init에서 LayerCollisionMatrix 설정하가
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
	// Layer 별로 충돌 체크
	for (UINT i = 0; i < static_cast<UINT>(LayerCollision::End); ++i)
	{
		// TODO: 같은 레이어끼리 충돌 체크? 필요하면 하기
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
/// Entity LayerCollision collider 추가 + ID 부여
/// </summary>
/// <param name="collider"></param>
void CollisionManagerSeoYoung::AddCollider(class BoxCollider* collider)
{
	// ID 부여
	collider->SetColliderID(m_colliderIDToAssign++);

	// 해당 Layer에 collider 추가
	LayerCollision layer = collider->GetOwner().lock()->GetLayerCollision();
	UINT layerIndex = static_cast<UINT>(layer);
	m_boxColliders[layerIndex].push_back(collider);
}

/// <summary>
/// Entity LayerCollision collider 삭제
/// </summary>
/// <param name="collider"></param>
void CollisionManagerSeoYoung::RemoveCollider(class BoxCollider* collider)
{
	// 해당 Layer에 collider 삭제
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
	// Layer1의 Collider들과 Layer2의 Collider들을 비교
	for (auto& collider1 : m_boxColliders[static_cast<UINT>(layer1)])
	{
		for (auto& collider2 : m_boxColliders[static_cast<UINT>(layer2)])
		{
			CollisionID collisionID = { collider1->GetColliderID(), collider2->GetColliderID() };

			// TODO: Entity isActive == false -> continue;
			// TODO: Entity isDestroyed == true -> continue;

			// #충돌했을 경우 && 이전 프레임에 충돌하지 않았을 경우 -> OnCollisionEnter
			if (collider1->IsIntersect(*collider2) == true && m_mapCollisionInfo[collisionID.IDs] == false)
			{
				collider1->OnTriggerEnter(*collider2);
				collider2->OnTriggerEnter(*collider1);
				m_mapCollisionInfo[collisionID.IDs] = true;
			}
			// #충돌했을 경우 && 이전 프레임에 충돌 -> OnCollisionStay
			else if (collider1->IsIntersect(*collider2) == true && m_mapCollisionInfo[collisionID.IDs] == true)
			{
				collider1->OnTriggerStay(*collider2);
				collider2->OnTriggerStay(*collider1);
			}
			// #충돌하지 않았을 경우 && 이전 프레임에 충돌 -> OnTriggerExit
			else if (collider1->IsIntersect(*collider2) == false && m_mapCollisionInfo[collisionID.IDs] == true)
			{
				collider1->OnTriggerExit(*collider2);
				collider2->OnTriggerExit(*collider1);
				m_mapCollisionInfo[collisionID.IDs] = false;
			}
		}
	}
}
