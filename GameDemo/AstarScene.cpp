#include "GameDemo_pch.h"
#include "AstarScene.h"

#include "ChaseRangeScript.h"
#include "IGraphics.h"
#include "UIRenderer.h"
#include "MeshRenderer.h"

#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "RayCaster.h"

#include "PlayerFSM.h"
#include "GuardFSM.h"
#include "ShopKeeperFSM.h"


#include "EventManager.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "GameManager.h"
#include "GuardScript.h"
#include "ShopKeeperDoubtScript.h"
#include "ShopKeeperCallingScript.h"
#include "PlayerMoveScript.h"
#include "FoodTableScript.h"
#include "InterKeyScript.h"

#include "DirectionalLight.h"
#include "RedBox.h"
#include "Entity.h"
#include "Grid.h"
#include "TextureBox.h"

#include "UI.h"
#include "UITestScript.h"
#include "UIGauge.h"

#include "PlayerPrefab.h"
#include "GuardPrefab.h"
#include "autoBush.h"
#include "InteractionBush.h"
#include "ScoreUI.h"



AstarScene::AstarScene(std::wstring sceneName) :Scene(sceneName)
{

}

void AstarScene::Start()
{
	m_cameraManager.lock()->GetMainCamera()->SetPosition(Vector3(10.309, 43.908, 5.134));
	m_cameraManager.lock()->GetMainCamera()->AddLookRadian(Vector2(0, -1.5f));
	Scene::SetSimulating(false);
	Scene::SetPrevSimulating(false);

#pragma region Base
	auto gridEntity1 = std::make_shared<Grid>();
	Scene::AddEntity(gridEntity1);

	//GameManger
	/*auto gameManager= std::make_shared<Entity>("gameManager");
	Scene::AddEntity(gameManager);
	gameManager->AddComponent<GameManager>();
	gameManager->GetComponent<Transform>()->SetLocalPosition(Vector3(75.f, 0.5f, 0.f));
	gameManager->AddComponent<SphereCollider>();
	gameManager->GetComponent<SphereCollider>()->SetRadius(3.f);
	gameManager->GetComponent<SphereCollider>()->SetDisableGravity(true);
	gameManager->GetComponent<SphereCollider>()->SetIsTrigger(true);*/

	// 라이트
	auto light = std::make_shared<Entity>("DLight");
	Scene::AddEntity(light);
	light->AddComponent<DirectionalLight>();

	//Plane
	auto RealPlane = std::make_shared<TextureBox>();
	Scene::AddEntity(RealPlane);
	RealPlane->SetName("RealPlane");
	RealPlane->AddComponent<PlaneCollider>();
#pragma endregion


#pragma region Guard
	//경비병
	auto GuardEntity = std::make_shared<Entity>("");
	Scene::AddEntity(GuardEntity);

	GuardEntity->SetName("Guard");

	GuardEntity->AddComponent<MeshRenderer>("PatrolRenderer");
	GuardEntity->AddComponent<CapsuleCollider>()->FreezeRotation(true, true, true);
	GuardEntity->GetComponent<CapsuleCollider>()->SetOffset({ 0.f, 0.0f, 0.f });
	GuardEntity->GetComponent<CapsuleCollider>()->SetSize(0.5, 0.5);
	//GetComponent<CapsuleCollider>()->SetIsTrigger(true);
	//GetComponent<CapsuleCollider>()->SetDisableGravity(true);

	GuardEntity->AddComponent<MeshFilter>("PatrolMesh");
	GuardEntity->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Patrol);

	GuardEntity->AddComponent<GuardScript>();
	GuardEntity->AddComponent<GuardFSM>();


	int m_range = 10;
	//magic number == tan 15
	float height = 0.5f;

	//정면 ray
	GuardEntity->AddComponent<RayCaster>();
	GuardEntity->GetComponent<RayCaster>(0)->SetUnitDirectionOffset(Vector3(0, 0, 1));
	GuardEntity->GetComponent<RayCaster>(0)->SetStartPosOffset(Vector3(0, height, 1));
	GuardEntity->GetComponent<RayCaster>(0)->SetDistance(m_range);

	//왼쪽 15도
	GuardEntity->AddComponent<RayCaster>();
	GuardEntity->GetComponent<RayCaster>(1)->SetUnitDirectionOffset(Vector3(-0.26, 0, 1));
	GuardEntity->GetComponent<RayCaster>(1)->SetStartPosOffset(Vector3(0, height, 1));
	GuardEntity->GetComponent<RayCaster>(1)->SetDistance(m_range);

	//오른쪽 15도
	GuardEntity->AddComponent<RayCaster>();
	GuardEntity->GetComponent<RayCaster>(2)->SetUnitDirectionOffset(Vector3(0.26, 0, 1));
	GuardEntity->GetComponent<RayCaster>(2)->SetStartPosOffset(Vector3(0, height, 1));
	GuardEntity->GetComponent<RayCaster>(2)->SetDistance(m_range);


	//왼쪽 7.5도
	GuardEntity->AddComponent<RayCaster>();
	GuardEntity->GetComponent<RayCaster>(3)->SetUnitDirectionOffset(Vector3(-0.13, 0, 1));
	GuardEntity->GetComponent<RayCaster>(3)->SetStartPosOffset(Vector3(0, height, 1));
	GuardEntity->GetComponent<RayCaster>(3)->SetDistance(m_range);

	//오른쪽 7.5도
	GuardEntity->AddComponent<RayCaster>();
	GuardEntity->GetComponent<RayCaster>(4)->SetUnitDirectionOffset(Vector3(0.13, 0, 1));
	GuardEntity->GetComponent<RayCaster>(4)->SetStartPosOffset(Vector3(0, height, 1));
	GuardEntity->GetComponent<RayCaster>(4)->SetDistance(m_range);

	GuardEntity->GetComponent<Transform>()->SetWorldPosition(Vector3(13, 0, 30));
	GuardEntity->GetComponent<GuardFSM>()->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(0, 0, 0));
	GuardEntity->GetComponent<GuardFSM>()->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(5, 0, 0));
	GuardEntity->GetComponent<GuardFSM>()->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(5, 0, -22));
	GuardEntity->GetComponent<GuardFSM>()->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(0, 0, -22));
	
	//가드 추적 반경 - 플레이어가 여기에 닿으면 무한 추격
	auto chaseRange = std::make_shared<Entity>("ChaseRange");
	AddEntity(chaseRange);
	chaseRange->AddComponent<SphereCollider>();
	chaseRange->GetComponent<SphereCollider>()->SetRadius(8);
	chaseRange->GetComponent<SphereCollider>()->SetIsTrigger(true);
	chaseRange->GetComponent<SphereCollider>()->SetDisableGravity(true);
	chaseRange->GetComponent<SphereCollider>()->SetBodyType(BodyType::Static);
	chaseRange->AddComponent<ChaseRangeScript>();
	chaseRange->SetLayerCollision(LayerCollision::OnlyPlayer);
	//GuardEntity->GetComponent<Transform>()->SetChild(chaseRange->GetComponent<Transform>());
	chaseRange->GetComponent<Transform>()->SetParent(GuardEntity->GetComponent<Transform>());
	chaseRange->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 0));

#pragma endregion

#pragma region Player
	auto PlayerEntity = std::make_shared<Entity>("Player");
	Scene::AddEntity(PlayerEntity);
	auto playerRenderer = PlayerEntity->AddComponent<MeshRenderer>("Player");
	auto playerMesh = PlayerEntity->AddComponent<MeshFilter>("PlayerMesh");
	PlayerEntity->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Player);
	auto transform = PlayerEntity->GetComponent<Transform>();
	transform->AddPosition(Vector3(0.f, 0.00f, 0.f));
	PlayerEntity->AddComponent<PlayerMoveScript>();
	PlayerEntity->AddComponent<PlayerFSM>();
	PlayerEntity->AddComponent<RayCaster>();
	PlayerEntity->GetComponent<RayCaster>()->SetUnitDirectionOffset(Vector3(0, 0, 1));
	PlayerEntity->GetComponent<RayCaster>()->SetStartPosOffset(Vector3(0, 0, 0.5f));
	PlayerEntity->GetComponent<RayCaster>()->SetDistance(5);
	PlayerEntity->AddComponent<CapsuleCollider>();
	auto collider = PlayerEntity->GetComponent<CapsuleCollider>();
	PlayerEntity->SetLayerCollision(LayerCollision::Player);  ///----------------------------------------------------------- 레이어 설정

	collider->FreezeRotation(true, true, true);
	collider->SetOffset(Vector3(0.f, 1.00f, 0.f));
	collider->SetSize(0.5f, 0.5f);
	collider->SetMaterial(0.5, 0.5, 0.f);

	playerMesh->SetModelKey(GraphicsTransfer::ModelKey::Player);


	/// 플레이어의 자식으로 ScoreUI 추가
	auto scoreUI = std::make_shared<Entity>("ScoreUI");
	Scene::AddEntity(scoreUI);
	scoreUI->GetComponent<Transform>()->SetParent(PlayerEntity->GetComponent<Transform>());
	scoreUI->AddComponent<ScoreUI>();
	scoreUI->AddComponent<UIRenderer>();
	scoreUI->AddComponent<MeshFilter>();
	scoreUI->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Score);

#pragma endregion




	Scene::Start();

	// FSM 상인 눈치 게이지 - 푸드 테이블 스크립트 포인터 연결
	// FSM이 start 함수가 호출 되고 난 이후로 연결 시켜야한다.
}

void AstarScene::Update(double dt)
{
	Scene::Update(dt);
}
