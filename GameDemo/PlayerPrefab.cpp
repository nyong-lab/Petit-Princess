#include "GameDemo_pch.h"
#include "PlayerPrefab.h"

#include "SceneManager.h"
#include "Scene.h"

#include "MeshRenderer.h"
#include "CapsuleCollider.h"
#include "PlayerFSM.h"
#include "PlayerMoveScript.h"
#include "UI.h"
#include "UIGauge.h"
#include "UITestScript.h"
#include "PetitCamera.h"

PlayerPrefab::PlayerPrefab() : Entity("Player")
{

}

PlayerPrefab::PlayerPrefab(const std::shared_ptr<PlayerPrefab>& other) : Entity(other)
{

}

PlayerPrefab::~PlayerPrefab()
{

}

void PlayerPrefab::Start()
{
	SetName("Player");

	AddComponent<MeshRenderer>("Player");
	AddComponent<MeshFilter>("PlayerMesh");
	GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Player);


	AddComponent<PetitCamera>();
	GetComponent<PetitCamera>()->SetBoundary(30,-30,150,-150);
	GetComponent<PetitCamera>()->SetTarget(shared_from_this());

	AddComponent<CapsuleCollider>()->FreezeRotation(true, true, true);
	GetComponent<CapsuleCollider>()->SetOffset({0.f, 1.f, 0.f});

	AddComponent<MeshRenderer>("PlayerRenderer");
	GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.5f, 0.f));
	AddComponent<PlayerFSM>();
	AddComponent<PlayerMoveScript>();
	AddComponent<RayCaster>();
	GetComponent<RayCaster>()->SetDistance(5);
	GetComponent<RayCaster>()->SetStartPosOffset(Vector3(0, 0, 0.5f));
	GetComponent<RayCaster>()->SetUnitDirectionOffset(Vector3(0, 0, 1));

// 	std::shared_ptr<UI> aimingUI = std::make_shared<UI>();
// 	aimingUI->SetName("AimmingUI");
// 	aimingUI->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
// 	aimingUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
// 	aimingUI->AddComponent<UITestScript>(UIType::Jiwon);
// 	aimingUI->SetRender(false);
// 
// 
// 	//// 플레이어에 ui 붙임
// 	GetComponent<Transform>()->SetChild(aimingUI->GetComponent<Transform>());
// 	aimingUI->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 0));
// 	m_sceneManager.lock()->GetCurScene()->AddEntity(aimingUI);
// 	aimingUI->Start();




	Entity::Start();
}

void PlayerPrefab::SetPos(Vector3 pos)
{

}
