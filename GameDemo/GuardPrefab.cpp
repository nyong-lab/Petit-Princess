#include "GameDemo_pch.h"
#include "GuardPrefab.h"

#include "MeshRenderer.h"
#include "GuardFSM.h"
#include "GuardScript.h"
#include "CapsuleCollider.h"
#include "GuardUI.h"

GuardPrefab::GuardPrefab() : Entity("Guard")
{

}

GuardPrefab::GuardPrefab(const std::shared_ptr<GuardPrefab>& other) : Entity(other)
{

}

GuardPrefab::~GuardPrefab()
{

}

void GuardPrefab::Start()
{
	SetName("Guard");

	AddComponent<MeshRenderer>("PatrolRenderer");
	AddComponent<CapsuleCollider>()->FreezeRotation(true, true, true);
	GetComponent<CapsuleCollider>()->SetOffset({ 0.f, 1.01f, 0.f });
	GetComponent<CapsuleCollider>()->SetSize(0.5, 0.5);
	//GetComponent<CapsuleCollider>()->SetIsTrigger(true);
	//GetComponent<CapsuleCollider>()->SetDisableGravity(true);

	AddComponent<MeshFilter>("PatrolMesh");
	GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Patrol);

	AddComponent<GuardScript>();
	AddComponent<GuardFSM>();

	//순찰 루트 셋팅
	for (auto destination : m_patrolPath)
	{
		GetComponent<GuardFSM>()->SetPatrolPath(destination);
	}

	//magic number == tan 15

	float height = 0.5f;

	//정면 ray
	AddComponent<RayCaster>();
	GetComponent<RayCaster>(0)->SetUnitDirectionOffset(Vector3(0, 0, 1));
	GetComponent<RayCaster>(0)->SetStartPosOffset(Vector3(0, height, 1));
	GetComponent<RayCaster>(0)->SetDistance(m_range);

	//왼쪽 15도
	AddComponent<RayCaster>();
	GetComponent<RayCaster>(1)->SetUnitDirectionOffset(Vector3(-0.26, 0, 1));
	GetComponent<RayCaster>(1)->SetStartPosOffset(Vector3(0, height, 1));
	GetComponent<RayCaster>(1)->SetDistance(m_range);

	//오른쪽 15도
	AddComponent<RayCaster>();
	GetComponent<RayCaster>(2)->SetUnitDirectionOffset(Vector3(0.26, 0, 1));
	GetComponent<RayCaster>(2)->SetStartPosOffset(Vector3(0, height, 1));
	GetComponent<RayCaster>(2)->SetDistance(m_range);


	//왼쪽 7.5도
	AddComponent<RayCaster>();
	GetComponent<RayCaster>(3)->SetUnitDirectionOffset(Vector3(-0.13, 0, 1));
	GetComponent<RayCaster>(3)->SetStartPosOffset(Vector3(0, height, 1));
	GetComponent<RayCaster>(3)->SetDistance(m_range);

	//오른쪽 7.5도
	AddComponent<RayCaster>();
	GetComponent<RayCaster>(4)->SetUnitDirectionOffset(Vector3(0.13, 0, 1));
	GetComponent<RayCaster>(4)->SetStartPosOffset(Vector3(0, height, 1));
	GetComponent<RayCaster>(4)->SetDistance(m_range);

	// 엔티티 Start 호출
	Entity::Start();
}

//경비병 순찰 루트 지정 == 처음 시작점 위치를 기준으로 어디구역을 순회할것인가 또는 직접지정가능
void GuardPrefab::SetPatrolPath(Vector3 destination)
{
	m_patrolPath.push_back(destination);
}

void GuardPrefab::SetPos(Vector3 pos)
{
	GetComponent<Transform>()->SetLocalPosition(Vector3(pos));

}
