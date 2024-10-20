#include "GameDemo_pch.h"
#include "SunWoongScene.h"

#include "Entity.h"
#include "Grid.h"
#include "TextureBox.h"
#include "CubeMap.h"
#include "UI.h"
#include "IGraphics.h"

#include "BoxCollider.h"
//#include "SphereCollider.h"
//#include "CapsuleCollider.h"
#include "MeshRenderer.h"
#include "TestingScript.h"

#include "EntityFactory.h"

#include "TransformBulider.h"
#include "MeshRenderBuilder.h"
#include "BoxColiderBulider.h"

#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"

#include "CollisionManager.h"
#include "PlaneCollider.h"

#include "CameraManager.h"
#include "UITestScript.h"
#include "UIGauge.h"
#include "ModelTest.h"
#include "MeshFilter.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "UIRenderer.h"

#include "UIHungry.h"

/// 플레이어
#include "PlayerMoveScript.h"
#include "PlayerFSM.h"
#include "ScoreUI.h"

#include "CollisionManager.h"
#include "GameManager.h"
#include "CapsuleCollider.h"
#include "TextRender.h"
#include "UIMiniMapIcon.h"
#include "UISkill.h"
#include "PetitCamera.h"



SunWoongScene::SunWoongScene(std::wstring sceneName) : Scene(sceneName)
{

}

void SunWoongScene::Start()
{
	m_cameraManager.lock()->GetMainCamera()->SetPosition(Vector3(0, 21, -28));
	m_cameraManager.lock()->GetMainCamera()->AddLookRadian(Vector2(0, -1.123f));
	// 	// 그리드
	// 	auto gridEntity = std::make_shared<Grid>(m_graphics, m_cameraManager.lock()->GetMainCamera());
	// 	Scene::AddEntity(gridEntity);
	/// 게임 메니저
	auto mgr = std::make_shared<Entity>("GameManager");
	Scene::AddEntity(mgr);
	auto manager = mgr->AddComponent<GameManager>();

	// 모델
	auto EricaEntity = std::make_shared<ModelTest>();
	Scene::AddEntity(EricaEntity);
	EricaEntity->GetComponent<Transform>()->SetWorldScale(Vector3(0.05f, 0.05f, 0.05f));
	EricaEntity->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.00f, 0.00f));

	// floor
	auto floor = std::make_shared<Entity>("SampleFloor");
	auto floorRenderer = floor->AddComponent<MeshRenderer>("FloorRenderer");
	auto floorMesh = floor->AddComponent<MeshFilter>("FloorMesh");
	Scene::AddEntity(floor);
	floorMesh->SetModelKey(GraphicsTransfer::ModelKey::Floor);

	/// 플레이어 
	auto player = std::make_shared<Entity>("Player");
	auto playerRenderer = player->AddComponent<MeshRenderer>("PlayerRenderer");
	auto playerMesh = player->AddComponent<MeshFilter>("PlayerMesh");
	player->AddComponent<PlayerMoveScript>();
	player->AddComponent<PlayerFSM>();
	player->AddComponent<RayCaster>();
	player->GetComponent<RayCaster>()->SetUnitDirectionOffset(Vector3(0, 0, 1));
	player->GetComponent<RayCaster>()->SetStartPosOffset(Vector3(0, 0, 0.5f));
	player->GetComponent<RayCaster>()->SetDistance(2);
	player->AddComponent<CapsuleCollider>();
	auto collider = player->GetComponent<CapsuleCollider>();
	auto transform = player->GetComponent<Transform>();
	transform->AddPosition(Vector3(0.f, 0.f, 0.f));
	collider->FreezeRotation(true, true, true);
	collider->SetSize(0.5f, 0.3f);
	collider->SetOffset(Vector3(0.f, 0.8f, 0.f));
	player->AddComponent<PetitCamera>();
	player->GetComponent<PetitCamera>()->SetTarget(player);
	Scene::AddEntity(player);
	playerMesh->SetModelKey(GraphicsTransfer::ModelKey::Player);
	manager->SetPlayer(player);

	/// 플레이어 던지기 조준 게이지바
	auto m_aimGaugeBarUI = std::make_shared<UIGauge>();
	m_aimGaugeBarUI->SetName("AimGaugeBar");
	m_aimGaugeBarUI->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
	m_aimGaugeBarUI->GetComponent<Transform>()->SetLocalScale({ 0.05,0.05,0.05 });
	m_aimGaugeBarUI->AddComponent<UITestScript>(UIType::Jiwon);
	//m_aimGaugeBarUI->GetComponent<UITestScript>()->SetDuration(0.0f);
	m_aimGaugeBarUI->SetRender(true);

	m_aimGaugeBarUI->GetComponent<UITestScript>()->SetPlayerPointer(player); /// 플레이어 포인터 전달
	Scene::AddEntity(m_aimGaugeBarUI);

	//플레이어에 게이지 ui 붙임
	//player->GetComponent<Transform>()->SetChild(m_aimGaugeBarUI->GetComponent<Transform>());

	/// 플레이어의 자식으로 ScoreUI 추가
	auto scoreUI = std::make_shared<Entity>("ScoreUI");
	Scene::AddEntity(scoreUI);
	scoreUI->GetComponent<Transform>()->SetParent(player->GetComponent<Transform>());
	scoreUI->AddComponent<ScoreUI>();
	scoreUI->AddComponent<UIRenderer>();
	scoreUI->AddComponent<MeshFilter>();
	scoreUI->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Score);

	/// -----------------------------------------------------------------------------------------------------------

	/// UI Hungry
	auto HungryUI = std::make_shared<UIHungry>();
	HungryUI->GetComponent<Transform>()->SetWorldPosition({ 800,-400,0 });
	HungryUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	HungryUI->AddComponent<UITestScript>(UIType::HUNGRY);
	HungryUI->AddComponent<TextRender>();
	auto hungryPointer = player->GetComponent<PlayerMoveScript>()->GetHungerPointer();
	HungryUI->GetComponent<UITestScript>()->SetHungry(hungryPointer);
	
	Scene::AddEntity(HungryUI);

	// 라이트
	auto light = std::make_shared<Entity>("DLight");
	Scene::AddEntity(light);
	light->AddComponent<DirectionalLight>();


	// 미니맵 아이콘
 	auto miniMapIcon = std::make_shared<UIMiniMapIcon>();
	miniMapIcon->GetComponent<Transform>()->SetWorldPosition({ -830,-400,0 });
	miniMapIcon->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	miniMapIcon->AddComponent<UITestScript>(UIType::STATIC);
 	Scene::AddEntity(miniMapIcon);

 	// UI 스킬 
 	auto SkillUI = std::make_shared<UISkill>();
	Scene::AddEntity(SkillUI);
	SkillUI->GetComponent<Transform>()->SetWorldPosition({ 830,-50,0 });
	SkillUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	SkillUI->AddComponent<TextRender>();
	SkillUI->AddComponent<UITestScript>(UIType::SKILL);
	SkillUI->GetComponent<UITestScript>()->SetPlayerPointer(player);
 	// UI test2
	// auto TestUI2 = std::make_shared<UI>();
	// TestUI2->GetComponent<Transform>()->SetWorldPosition({ 500,200,0 });
	// TestUI2->GetComponent<Transform>()->SetWorldScale({ 1,2,1 });
	// TestUI2->AddComponent<UITestScript>(UIType::STATIC);
	// Scene::AddEntity(TestUI2);
	// 
	// auto TestUI3 = std::make_shared<UI>();
	// TestUI3->GetComponent<Transform>()->SetWorldPosition({ 500,400,0 });
	// TestUI3->GetComponent<Transform>()->SetWorldScale({ 2,1,1 });
	// TestUI3->AddComponent<UITestScript>(UIType::STATIC);
	// Scene::AddEntity(TestUI3);
	// 
	// auto TestUI4 = std::make_shared<UIGauge>();
	// TestUI4->GetComponent<Transform>()->SetWorldPosition({ 500,-200,0 });
	// TestUI4->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	// TestUI4->AddComponent<UITestScript>(UIType::GAUGE);
	// Scene::AddEntity(TestUI4);


	Scene::Start();
}

void SunWoongScene::Update(double dt)
{
	Scene::Update(dt);

	///----------이벤트 매니저 확인용------------
	if (InputManager::GetInstance()->IsKeyDown(VK_BACK))
	{
		//if (m_sceneManager.lock()->GetCurScene() != shared_from_this())
		{
			EventManager::GetInstance()->SceneChange(m_sceneManager.lock()->GetScene("YeonhaScene"));
		}
	}
}