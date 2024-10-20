#include "GameDemo_pch.h"
#include "STAGE1.h"

#include <random>

//괴물 전역변수 ㅋㅎ
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> ranInt(0, 1);


///Entity
#include "Entity.h"

///Prefab
#include "PlayerPrefab.h"
#include "PlayerPrefab.h"
#include "GuardPrefab.h"
#include "Grid.h"
#include "RedBox.h"
#include "TextureBox.h"
#include "UI.h"
#include "UIGauge.h"
#include "UIhungry.h"
#include "UIMiniMapIcon.h"
#include "UISkill.h"
#include "UIBlack.h"


///Compoenent
#include "DirectionalLight.h"
#include "SunLight.h"
#include "TimerUI.h"
#include "TextRender.h"
#include "MeshFilter.h"
#include "UIRenderer.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "MeshRenderer.h"
#include "ShopKeeperFSM.h"
#include "CapsuleCollider.h"
#include "BoxCollider.h"
#include "PointLight.h"
#include "UIMinimapCurPos.h"
#include "MerchantUI.h"
#include "PlayerMoveScript.h"
#include "PlayerFSM.h"
#include "PetitCamera.h"

///Script
#include "GameManager.h"
#include "TechniquesScript.h"
#include "InterKeyScript.h"
#include "ShopKeeperDoubtScript.h"
#include "FoodTableScript.h"
#include "ScoreUI.h"
#include "HideAutomaticScript.h"
#include "HideInteractionScript.h"
#include "UITestScript.h"
#include "PopUpUI.h"
#include "ShopKeeperCallingScript.h"
#include "UIDataSyncScript.h"
#include "UIMinimap.h"
#include "TorchLight.h"
#include "HideInterKeyScript.h"
#include "ChaseRangeScript.h"
#include "GuardUI.h"

///Manager
#include "ResourceManager.h"
#include "MinigameDefine.h"
#include "CameraManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "CameraModule.h"

STAGE1::STAGE1(std::wstring sceneName)
	: Scene(sceneName)
{
	m_shopData =
	{
		{Vector3(7.5f, 0.f, 5.5f), EntityDirection::RIGHT},			//1
		{Vector3(34.5f, 0.f, 9.5f), EntityDirection::RIGHT},		//2
		{Vector3(17.5f, 0.f, 20.5f), EntityDirection::RIGHT},		//3
		{Vector3(40.0f, 0.f, 30.5f), EntityDirection::DOWN},		//4
		{Vector3(19.5f, 0.f, 41.5f), EntityDirection::RIGHT},		//5
		{Vector3(60.0f, 0.f, 27.5f), EntityDirection::DOWN},		//6
		{Vector3(6.5f, 0.f, 56.5f), EntityDirection::DOWN},			//7
		{Vector3(75.5f, 0.f, 14.5f), EntityDirection::DOWN},		//8
		{Vector3(26.5f, 0.f, 71.5f), EntityDirection::DOWN},		//9
		{Vector3(42.5f, 0.f, 58.5f), EntityDirection::RIGHT},		//10

		{Vector3(60.5f, 0.f, 47.5f), EntityDirection::RIGHT},		//11
		{Vector3(79.5f, 0.f, 35.5f), EntityDirection::DOWN},		//12
		{Vector3(97.5f, 0.f, 28.5f), EntityDirection::LEFT},		//13
		{Vector3(112.5f, 0.f, 19.5f), EntityDirection::LEFT},		//14
		{Vector3(121.5f, 0.f, 4.5f), EntityDirection::RIGHT},		//15
		{Vector3(133.5f, 0.f, 18.5f), EntityDirection::LEFT},		//16
		{Vector3(144.5f, 0.f, 5.5f), EntityDirection::LEFT},		//17
		{Vector3(125.5f, 0.f, 32.5f), EntityDirection::LEFT},		//18
		{Vector3(136.5f, 0.f, 44.5f), EntityDirection::LEFT},		//19
		{Vector3(98.5f, 0.f, 48.5f), EntityDirection::LEFT},		//20

		{Vector3(114.5f, 0.f, 55.5f), EntityDirection::LEFT},		//21
		{Vector3(128.5f, 0.f, 65.5f), EntityDirection::RIGHT},		//22
		{Vector3(143.5f, 0.f, 75.5f), EntityDirection::LEFT},		//23
		{Vector3(130.5f, 0.f, 84.5f), EntityDirection::RIGHT},		//24
		{Vector3(110.5f, 0.f, 94.5f), EntityDirection::LEFT},		//25
		{Vector3(128.5f, 0.f, 102.5f), EntityDirection::DOWN},		//26
		{Vector3(143.5f, 0.f, 102.5f), EntityDirection::DOWN},		//27
		{Vector3(123.5f, 0.f, 118.5f), EntityDirection::LEFT},		//28
		{Vector3(139.5f, 0.f, 130.5f), EntityDirection::LEFT},		//29
		{Vector3(142.5f, 0.f, 143.5f), EntityDirection::LEFT},		//30

		{Vector3(121.5f, 0.f, 141.5f), EntityDirection::DOWN},		//31
		{Vector3(104.5f, 0.f, 122.5f), EntityDirection::RIGHT},		//32
		{Vector3(99.5f, 0.f, 138.5f), EntityDirection::LEFT},		//33
		{Vector3(75.5f, 0.f, 139.5f), EntityDirection::DOWN},		//34
		{Vector3(82.5f, 0.f, 123.5f), EntityDirection::DOWN},		//35
		{Vector3(49.5f, 0.f, 138.5f), EntityDirection::RIGHT},		//36
		{Vector3(55.5f, 0.f, 121.5f), EntityDirection::LEFT},		//37
		{Vector3(29.5f, 0.f, 131.5f), EntityDirection::LEFT},		//38
		{Vector3(8.5f, 0.f, 140.5f), EntityDirection::RIGHT},		//39
		{Vector3(12.5f, 0.f, 119.5f), EntityDirection::DOWN},		//40

		{Vector3(20.5f, 0.f, 100.5f), EntityDirection::DOWN},		//41
		{Vector3(33.5f, 0.f, 113.5f), EntityDirection::LEFT},		//42
		{Vector3(38.5f, 0.f, 88.5f), EntityDirection::RIGHT},		//43
		{Vector3(52.5f, 0.f, 101.5f), EntityDirection::DOWN},		//44
		{Vector3(56.5f, 0.f, 78.5f), EntityDirection::RIGHT},		//45
		{Vector3(68.5f, 0.f, 61.5f), EntityDirection::DOWN},		//46
		{Vector3(90.5f, 0.f, 63.5f), EntityDirection::LEFT},		//47
		{Vector3(112.5f, 0.f, 74.5f), EntityDirection::DOWN},		//48
		{Vector3(96.5f, 0.f, 79.5f), EntityDirection::LEFT},		//49
		{Vector3(96.5f, 0.f, 108.5f), EntityDirection::LEFT},		//50

		{Vector3(71.5f, 0.f, 110.5f), EntityDirection::RIGHT},		//51
		{Vector3(75.5f, 0.f, 93.5f), EntityDirection::DOWN},		//52
		{Vector3(15.5f, 0.f, 84.5f), EntityDirection::LEFT},		//53
	};

	m_guardData =
	{
		{Vector3(25.5f, 0.f, 30.5f), EntityDirection::DOWN},		//1
		{Vector3(43.5f, 0.f, 18.5f), EntityDirection::LEFT},		//2
		{Vector3(16.5f, 0.f, 52.5f), EntityDirection::UP},			//3
		{Vector3(10.5f, 0.f, 67.5f), EntityDirection::UP},			//4
		{Vector3(47.5f, 0.f, 81.5f), EntityDirection::LEFT},		//5

		{Vector3(5.5f, 0.f, 93.5f), EntityDirection::RIGHT},		//6
		{Vector3(41.5f, 0.f, 97.5f), EntityDirection::LEFT},		//7
		{Vector3(24.5f, 0.f, 111.5f), EntityDirection::LEFT},		//8
		{Vector3(8.5f, 0.f, 131.5f), EntityDirection::RIGHT},		//9
		{Vector3(34.5f, 0.f, 145.5f), EntityDirection::RIGHT},		//10

		{Vector3(46.5f, 0.f, 124.5f), EntityDirection::LEFT},		//11
		{Vector3(48.5f, 0.f, 38.5f), EntityDirection::RIGHT},		//12
		{Vector3(91.5f, 0.f, 29.5f), EntityDirection::LEFT},		//13
		{Vector3(113.5f, 0.f, 10.5f), EntityDirection::LEFT},		//14
		{Vector3(114.5f, 0.f, 35.5f), EntityDirection::UP},			//15

		{Vector3(122.5f, 0.f, 25.5f), EntityDirection::DOWN},		//16
		{Vector3(61.5f, 0.f, 54.5f), EntityDirection::LEFT},		//17
		{Vector3(53.5f, 0.f, 131.5f), EntityDirection::RIGHT},		//18
		{Vector3(64.5f, 0.f, 111.5f), EntityDirection::LEFT},		//19
		{Vector3(69.5f, 0.f, 147.5f), EntityDirection::LEFT},		//20

		{Vector3(82.5f, 0.f, 115.5f), EntityDirection::RIGHT},		//21
		{Vector3(78.5f, 0.f, 85.5f), EntityDirection::LEFT},		//22
		{Vector3(70.5f, 0.f, 68.5f), EntityDirection::RIGHT},		//23
		{Vector3(88.5f, 0.f, 137.5f), EntityDirection::UP},			//24
		{Vector3(99.5f, 0.f, 101.5f), EntityDirection::LEFT},		//25

		{Vector3(100.5f, 0.f, 64.5f), EntityDirection::RIGHT},		//26
		{Vector3(93.5f, 0.f, 53.5f), EntityDirection::LEFT},		//27
		{Vector3(103.5f, 0.f, 132.5f), EntityDirection::LEFT},		//28
		{Vector3(114.5f, 0.f, 148.5f), EntityDirection::RIGHT},		//29
		{Vector3(132.5f, 0.f, 131.5f), EntityDirection::LEFT},		//30

		{Vector3(122.5f, 0.f, 77.5f), EntityDirection::RIGHT},		//21
		{Vector3(124.5f, 0.f, 53.5f), EntityDirection::UP},			//22
		{Vector3(139.5f, 0.f, 112.5f), EntityDirection::LEFT},		//23
		{Vector3(137.5f, 0.f, 94.5f), EntityDirection::LEFT},		//24
	};
}

void STAGE1::Start()
{
	m_cameraManager.lock()->GetMainCamera()->SetPosition(Vector3(0, 21, -28));
	m_cameraManager.lock()->GetMainCamera()->AddLookRadian(Vector2(0, -1.123f));

	/// 물리 적용
	Scene::SetSimulating(false);
	Scene::SetPrevSimulating(false);

	/// floor
	auto floor = std::make_shared<Entity>("SampleFloor");
	auto floorRenderer = floor->AddComponent<MeshRenderer>("FloorRenderer");
	auto floorMesh = floor->AddComponent<MeshFilter>("FloorMesh");
	floor->AddComponent<PlaneCollider>();
	floor->GetComponent<Transform>()->SetLocalScale(Vector3(10.0f, 1.f, 10.f));
	Scene::AddEntity(floor);
	floorMesh->SetModelKey(GraphicsTransfer::ModelKey::Floor);
	floorMesh->m_pbrSet.useBaseColorMap = false;
	floorMesh->m_pbrSet.SetBaseColor({ 201, 148, 130, 255 });

	floorMesh->m_cullingOff = true; // 바닥은 컬링하지 않는다.

	/*///Grid
	auto gridEntity1 = std::make_shared<Grid>();
	Scene::AddEntity(gridEntity1);*/

	CreateDeco(DecoType::Flag, 74, 5);

	///GameManager
	auto mgr = std::make_shared<Entity>("GameManager");
	Scene::AddEntity(mgr);
	auto manager = mgr->AddComponent<GameManager>();

	// 게임 메니저의 자식으로 추가 -> 시간
	auto timemgr = std::make_shared<Entity>("TimeUI");
	Scene::AddEntity(timemgr);
	timemgr->GetComponent<Transform>()->SetParent(mgr->GetComponent<Transform>());
	//timemgr->AddComponent<SunLight>();
	auto light = timemgr->AddComponent<DirectionalLight>();
	light->SetIntensity(0.14);
	light->SetAmbient255(Vector4(255, 255, 255, 247));
	timemgr->AddComponent<TimerUI>();
	timemgr->AddComponent<TextRender>();
	timemgr->AddComponent<MeshFilter>();
	timemgr->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Timer);
	timemgr->AddComponent<UIRenderer>();

	mgr->AddComponent<SphereCollider>();
	mgr->GetComponent<SphereCollider>()->SetRadius(3.f);
	mgr->GetComponent<SphereCollider>()->SetDisableGravity(true);
	mgr->GetComponent<SphereCollider>()->SetIsTrigger(true);
	mgr->GetComponent<Transform>()->SetWorldPosition(Vector3(75, 0, 2));

	///Player
	//플레이어
	auto PlayerEntity = std::make_shared<Entity>("Player");
	Scene::AddEntity(PlayerEntity);
	auto playerRenderer = PlayerEntity->AddComponent<MeshRenderer>("Player");
	auto playerMesh = PlayerEntity->AddComponent<MeshFilter>("PlayerMesh");
	PlayerEntity->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Player);
	auto transform = PlayerEntity->GetComponent<Transform>();
	transform->AddPosition(Vector3(70.f, 0.0f, 5.f));   m_cameraManager.lock()->GetMainCamera()->SetPosition(Vector3(74, 30, -10));
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

	PlayerEntity->AddComponent<PetitCamera>()->SetBoundary(120, -10, 135, 10);
	PlayerEntity->GetComponent<PetitCamera>()->SetTarget(PlayerEntity);

	manager->SetPlayer(PlayerEntity);

	/// 플레이어의 자식으로 ScoreUI 추가
	auto scoreUI = std::make_shared<Entity>("ScoreUI");
	Scene::AddEntity(scoreUI);
	scoreUI->GetComponent<Transform>()->SetParent(PlayerEntity->GetComponent<Transform>());
	scoreUI->AddComponent<ScoreUI>();
	scoreUI->AddComponent<UIRenderer>();
	scoreUI->AddComponent<MeshFilter>();
	scoreUI->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Score);


	/// 플레이어 던지기 조준 게이지바
	auto aimingUI = std::make_shared<UIGauge>();
	Scene::AddEntity(aimingUI);
	aimingUI->SetName("AimmingUI");
	aimingUI->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
	aimingUI->GetComponent<Transform>()->SetLocalScale({ 0.05,0.05,0.05 });
	aimingUI->AddComponent<UITestScript>(UIType::Jiwon);
	//aimingUI->GetComponent<UITestScript>()->SetDuration(0.0f);
	aimingUI->SetRender(true);
	aimingUI->GetComponent<UITestScript>()->SetPlayerPointer(PlayerEntity); /// 플레이어 포인터 전달


	/// 미니맵 아이콘
	auto miniMapIcon = std::make_shared<UIMiniMapIcon>();
	miniMapIcon->GetComponent<Transform>()->SetWorldPosition({ -830,-430,0 });
	miniMapIcon->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	miniMapIcon->AddComponent<UITestScript>(UIType::STATIC);
	Scene::AddEntity(miniMapIcon);

	// 맛집 방문 리스트
	auto eatSpotList = std::make_shared<Entity>("EatSpotList");
	eatSpotList->AddComponent<UIRenderer>();
	Scene::AddEntity(eatSpotList);
	eatSpotList->AddComponent<MeshFilter>()->SetModelKey(ModelKey::UI_EatSpot);

	/// UI Hungry
	auto HungryUI = std::make_shared<UIHungry>();
	Scene::AddEntity(HungryUI);
	HungryUI->GetComponent<Transform>()->SetWorldPosition({ 830,-400,0 });
	HungryUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	HungryUI->AddComponent<UITestScript>(UIType::HUNGRY);
	HungryUI->AddComponent<TextRender>();
	auto hungryPointer = PlayerEntity->GetComponent<PlayerMoveScript>()->GetHungerPointer();
	HungryUI->GetComponent<UITestScript>()->SetHungry(hungryPointer);

	/// UI 스킬 
	auto SkillUI = std::make_shared<UISkill>();
	Scene::AddEntity(SkillUI);
	SkillUI->GetComponent<Transform>()->SetWorldPosition({ 850,-50,0 });
	SkillUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	SkillUI->AddComponent<TextRender>();
	SkillUI->AddComponent<UITestScript>(UIType::SKILL);
	SkillUI->GetComponent<UITestScript>()->SetPlayerPointer(PlayerEntity);


	///맵의 만들기
	int(*grid)[151] = m_resourceManager.lock()->GetMapInfo();
	int(*direction)[151] = m_resourceManager.lock()->GetDirData();
	int(*minigame)[151] = m_resourceManager.lock()->GetGameData();

	/// 플레이어가 못나가는 투명 벽
	{
		// 아래
		auto bottomWall = std::make_shared<Entity>("bottomWall");
		Scene::AddEntity(bottomWall);
		bottomWall->GetComponent<Transform>()->SetLocalPosition(Vector3(80, 1, -5));
		auto bwc = bottomWall->AddComponent<BoxCollider>();
		bwc->SetSize(Vector3(200, 2, 1));
		bwc->SetBodyType(BodyType::Static);

		// 위
		auto topWall = std::make_shared<Entity>("topWall");
		Scene::AddEntity(topWall);
		topWall->GetComponent<Transform>()->SetLocalPosition(Vector3(75, 1, 155));
		auto twc = topWall->AddComponent<BoxCollider>();
		twc->SetSize(Vector3(200, 2, 1));
		twc->SetBodyType(BodyType::Static);

		// 좌
		auto leftWall = std::make_shared<Entity>("leftWall");
		Scene::AddEntity(leftWall);
		leftWall->GetComponent<Transform>()->SetLocalPosition(Vector3(-10, 1, 80));
		auto lwc = leftWall->AddComponent<BoxCollider>();
		lwc->SetSize(Vector3(1, 2, 200));
		lwc->SetBodyType(BodyType::Static);

		// 우
		auto rightWall = std::make_shared<Entity>("rightWall");
		Scene::AddEntity(rightWall);
		rightWall->GetComponent<Transform>()->SetLocalPosition(Vector3(160, 1, 80));
		auto rwc = rightWall->AddComponent<BoxCollider>();
		rwc->SetSize(Vector3(1, 2, 200));
		rwc->SetBodyType(BodyType::Static);
	}

	//Wall
	for (int i = 0; i < 150; i++)
	{
		for (int j = 0; j < 150; j++)
		{
			CreateMap(grid[i][j], j, i, direction[i][j], minigame[i][j]);
		}
	}

	/// 사운드
	SoundManager::GetInstance()->StopAllBGM();
	SoundManager::GetInstance()->ResumeBGM("InGame_Default.wav");
	SoundManager::GetInstance()->PlayBGM("InGame_Default.wav");

	/// 미니맵(탭키)
	std::shared_ptr<Entity> minimap = std::make_shared<UIMinimap>();
	minimap->SetName("Minimap");
	minimap->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
	minimap->AddComponent<UITestScript>(UIType::MINIMAP);
	Scene::AddEntity(minimap);
	minimap->SetRender(false);

	// 미니맵 X버튼
	auto minimapClose = std::make_shared<UIMinimapCurPos>();
	minimapClose->SetName("MinimapCloseButton");
	minimapClose->GetComponent<Transform>()->SetWorldPosition({ 488,190,0 });
	minimapClose->AddComponent<UITestScript>(UIType::MINIMAPCURPOS);
	Scene::AddEntity(minimapClose);
	minimapClose->SetRender(false);




	//// ESC 눌렀을 때 뜰 팝업 UI ---------------------- 제일 뒤에 추가해야 합니다
	//auto popUp = std::make_shared<Entity>("PopUpUI");
	//Scene::AddEntity(popUp);
	//popUp->AddComponent<PopUpUI>();
	//popUp->AddComponent<UIRenderer>();
	//popUp->AddComponent<MeshFilter>();
	//popUp->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_IngamePopUp);


	/// fadeIN/OUT   ------------ 제가 제일 뒤에
	auto BlackUI = std::make_shared<UIBlack>();
	Scene::AddEntity(BlackUI);
	BlackUI->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
	BlackUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	BlackUI->AddComponent<UITestScript>(UIType::FADE);

	Scene::Start();

	if (m_tableScripts.size() == m_shopkeeperFSMs.size())
	{
		for (int i = 0; i < m_tableScripts.size(); ++i)
		{
			m_tableScripts[i]->SetPointerSense(m_shopkeeperFSMs[i]->GetPointerSense());
		}
	}
	else
		assert(!"이 둘의 사이즈가 다르다는건 잘못됐다.");
}

void STAGE1::Update(double dt)
{
	Scene::Update(dt);
}



void STAGE1::CreateWall(int x, int y)
{
	auto wall = std::make_shared<PinkBox>();
	wall->SetName("wall");
	wall->GetComponent<Transform>()->SetLocalPosition(Vector3(x, 0.5, y));
	AddEntity(wall);
	// 	wall->SetLayerCollision(LayerCollision::Wall);  ///----------------------------------------------------------- 레이어 설정
	// 	wall->AddComponent<BoxCollider>()->SetSize(Vector3(1, 2, 1));
	// 	wall->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);
}

void STAGE1::CreateAutoBush(int x, int y)
{
	auto m_hideAutomaticObject = std::make_shared<Entity>("Grass");
	m_hideAutomaticObject->SetName("HideAutomaticObject");
	Scene::AddEntity(m_hideAutomaticObject);
	m_hideAutomaticObject->AddComponent<MeshRenderer>();
	m_hideAutomaticObject->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Bush);
	m_hideAutomaticObject->GetComponent<Transform>()->SetLocalPosition(Vector3(x, 0.0f, y));
	m_hideAutomaticObject->AddComponent<SphereCollider>();
	m_hideAutomaticObject->GetComponent<SphereCollider>()->SetRadius(1.5f);
	m_hideAutomaticObject->GetComponent<SphereCollider>()->SetDisableGravity(true);
	m_hideAutomaticObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	m_hideAutomaticObject->AddComponent<HideAutomaticScript>();

	m_hideAutomaticObject->SetLayerCollision(LayerCollision::Interactable);  ///----------------------------------------------------------- 레이어 설정
}

void STAGE1::CreateInterBush(int x, int y)
{
	/// 숨기 오브젝트 (수동)
	auto m_hideInteractionObject = std::make_shared<Entity>("InterBush");
	m_hideInteractionObject->SetName("HideInteractionObject");
	Scene::AddEntity(m_hideInteractionObject);
	m_hideInteractionObject->GetComponent<Transform>()->SetLocalPosition(Vector3(x, 0.0f, y));
	m_hideInteractionObject->AddComponent<SphereCollider>();
	m_hideInteractionObject->GetComponent<SphereCollider>()->SetRadius(1.5f);
	m_hideInteractionObject->GetComponent<SphereCollider>()->SetDisableGravity(true);
	m_hideInteractionObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	m_hideInteractionObject->AddComponent<BoxCollider>()->SetOffset({ 0,1,0 });				/// 박스 콜라이더
	m_hideInteractionObject->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);	/// 박스 콜라이더 
	m_hideInteractionObject->AddComponent<HideInteractionScript>();
	m_hideInteractionObject->AddComponent<MeshRenderer>();
	m_hideInteractionObject->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Barrel);
	m_hideInteractionObject->SetLayerCollision(LayerCollision::Interactable);  ///----------------------------------------------------------- 레이어 설정

	/// 숨기 가능 표시용 UI - 기술 오브젝트쪽 보고 다시 만듦
	auto m_hideInterKey = std::make_shared<Entity>("CanHideInterKey");
	m_hideInterKey->AddComponent<UIRenderer>("CanHideInterKeyRenderer");
	m_hideInterKey->AddComponent<MeshFilter>("CanHideInterKeyMesh");
	m_hideInterKey->AddComponent<HideInterKeyScript>();
	m_hideInterKey->GetComponent<HideInterKeyScript>()->m_hideInteractionScript = m_hideInteractionObject->GetComponent<HideInteractionScript>();
	Scene::AddEntity(m_hideInterKey);
	m_hideInterKey->SetRender(false);
	m_hideInterKey->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_IngameInterKey);
	m_hideInteractionObject->GetComponent<Transform>()->SetChild(m_hideInterKey->GetComponent<Transform>());
	m_hideInterKey->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 2.2f, 0.f));
}

void STAGE1::CreatePotion(int x, int y)
{
	///SkillObject
	auto techniquesObject = std::make_shared<Entity>("Potion");
	techniquesObject->SetName("TechniquesObject");
	Scene::AddEntity(techniquesObject);
	techniquesObject->GetComponent<Transform>()->SetLocalPosition(Vector3(x, 0.50f, y));
	techniquesObject->AddComponent<SphereCollider>();
	techniquesObject->GetComponent<SphereCollider>()->SetRadius(1.5f);
	techniquesObject->GetComponent<SphereCollider>()->SetDisableGravity(true);
	techniquesObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	techniquesObject->AddComponent<BoxCollider>()->SetOffset({ 0,0.5,0 });				/// 박스 콜라이더
	techniquesObject->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);		/// 박스 콜라이더
	techniquesObject->AddComponent<TechniquesScript>();
	techniquesObject->AddComponent<MeshRenderer>();
	techniquesObject->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Box1);
	techniquesObject->SetLayerCollision(LayerCollision::Interactable);  ///----------------------------------------------------------- 레이어 설정

	/// 인터랙티브 키 (기술 오브젝트)
	auto techniquesInterKey = std::make_shared<Entity>("InteractiveKey (Techniques Object)");
	auto techniquesIterKeyRenderer = techniquesInterKey->AddComponent<UIRenderer>("InteractiveKeyRenderer");
	auto techniquesInterKeyMesh = techniquesInterKey->AddComponent<MeshFilter>("InteractiveKeyMesh");
	auto techniquesInterKeyScript = techniquesInterKey->AddComponent<InterKeyScript>();
	techniquesInterKeyScript->m_techniquesScript = techniquesObject->GetComponent<TechniquesScript>();
	Scene::AddEntity(techniquesInterKey);
	techniquesInterKeyMesh->SetModelKey(GraphicsTransfer::ModelKey::QTE_InteractiveKey);
	techniquesObject->GetComponent<Transform>()->SetChild(techniquesInterKey->GetComponent<Transform>()); // 기술 오브젝트의 자식으로 설정
	auto techniquesInterKeyTransform = techniquesInterKey->GetComponent<Transform>();
	techniquesInterKeyTransform->SetLocalPosition(Vector3(0.f, 2.2f, 0.f));
	techniquesInterKeyTransform->SetLocalScale(Vector3(0.01f, 0.01f, 0.01f));
}

void STAGE1::CreateGuard(int x, int y)
{
	///Guard
	//for (auto i : m_guardData)

	//경비병
	auto GuardEntity = std::make_shared<GuardPrefab>();
	Scene::AddEntity(GuardEntity);	

	// UI
	auto ui = std::make_shared<Entity>("GuardUI");
	Scene::AddEntity(ui);
	ui->AddComponent<UIRenderer>();
	ui->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_GuardFSM);
	ui->AddComponent<GuardUI>();
	GuardEntity->GetComponent<Transform>()->SetChild(ui->GetComponent<Transform>());
	//ui->GetComponent<Transform>()->SetParent(GuardEntity->GetComponent<Transform>());

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

	GuardEntity->SetPos(Vector3(x, 0, y));
	GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(0, 0, 0));
	GuardEntity->SetLayerCollision(LayerCollision::Guard);  ///----------------------------------------------------------- 레이어 설정
	//float position = 0.f;	// <- 개별 설정
	//if (i.m_direction == EntityDirection::DOWN)
	//{
	//	position = 0.f;
	//}
	//else if (i.m_direction == EntityDirection::RIGHT)
	//{
	//	position = -90.f;
	//}
	//else if (i.m_direction == EntityDirection::LEFT)
	//{
	//	position = 90.f;
	//}
	//else if (i.m_direction == EntityDirection::UP)
	//{
	//	position = 180.f;
	//}

	//GuardEntity->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, position, 0.f));

}

void STAGE1::CreatePlayer(int x, int y)
{

}

void STAGE1::CreateStreetLight(int x, int y)
{
	auto store = std::make_shared<Entity>("Lamp");
	auto storeRenderer = store->AddComponent<MeshRenderer>();
	auto storeMesh = store->AddComponent<MeshFilter>();
	auto storeTransform = store->GetComponent<Transform>();
	Scene::AddEntity(store);
	storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Lamp);
	store->AddComponent<PointLight>();
	store->AddComponent<TorchLight>();
	store->AddComponent<BoxCollider>();

	storeTransform->SetLocalPosition(Vector3(x, 0.f, y));
}

void STAGE1::CreateTree(int x, int y)
{
	auto store = std::make_shared<Entity>("Tree");
	auto storeRenderer = store->AddComponent<MeshRenderer>();
	auto storeMesh = store->AddComponent<MeshFilter>();
	auto storeTransform = store->GetComponent<Transform>();
	Scene::AddEntity(store);
	storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Tree1);

	store->AddComponent<BoxCollider>();
	storeTransform->SetWorldPosition(Vector3(x, 0.f, y));
}

void STAGE1::CreateStore(int x, int y, int dir, int type)
{
	///Shop
	/// 음식 가게
	auto store = std::make_shared<Entity>("SampleStore");
	auto storeRenderer = store->AddComponent<MeshRenderer>("StoreRenderer");
	auto storeMesh = store->AddComponent<MeshFilter>("SampleStore");
	auto storeTransform = store->GetComponent<Transform>();
	//storeTransform->AddPosition(Vector3(x, 0.f, y));
	Scene::AddEntity(store);
	//storeMesh->SetModelKey(GraphicsTransfer::ModelKey::BagleStore);
	store->AddComponent<BoxCollider>()->SetSize({ 7.f,7.5f,5.8f });
	store->GetComponent<BoxCollider>()->SetOffset({ -0.7,3.8,0.2 });
	store->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);
	store->SetLayerCollision(LayerCollision::Store);  ///----------------------------------------------------------- 레이어 설정
	//store->AddComponent<BoxCollider>()->SetBodyType(BodyType::Static);



	/// 상인 
	auto merchant = std::make_shared<Entity>("SampleMerchant");
	Scene::AddEntity(merchant);
	merchant->AddComponent<ShopKeeperFSM>();
	merchant->AddComponent<ShopKeeperDoubtScript>();
	auto merchantRenderer = merchant->AddComponent<MeshRenderer>("MerchantRenderer");
	auto merchantMesh = merchant->AddComponent<MeshFilter>("PlayerMesh");
	merchantMesh->SetModelKey(GraphicsTransfer::ModelKey::Merchant);
	store->GetComponent<Transform>()->SetChild(merchant->GetComponent<Transform>()); // 음식 가게의 자식으로 설정
	merchant->GetComponent<Transform>()->AddPosition(Vector3(-0.1f, 0.f, -3.4f));
	merchant->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
	/*merchant->AddComponent<CapsuleCollider>()->FreezeRotation(true, true, true);  /// 캡슐 콜라이더
	merchant->GetComponent<CapsuleCollider>()->SetOffset({ 0.f, 1.f, 0.f });	  /// 캡슐 콜라이더
	merchant->GetComponent<CapsuleCollider>()->SetSize({ 0.5,0.5,0.5 });			  /// 캡슐 콜라이더
	merchant->GetComponent<CapsuleCollider>()->SetBodyType(BodyType::Static);	  /// 캡슐 콜라이더*/
	merchant->GetComponent<ShopKeeperFSM>()->Start();
	merchant->SetLayerCollision(LayerCollision::Merchant); ///----------------------------------------------------------- 레이어 설정
	/*merchant->GetComponent<CapsuleCollider>()->SetOffset();*/

	/// 상인의 UI
	auto merchantUI = std::make_shared<Entity>("MerchantUI");
	Scene::AddEntity(merchantUI);
	merchant->GetComponent<Transform>()->SetChild(merchantUI->GetComponent<Transform>());
	merchantUI->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Merchant);
	merchantUI->AddComponent<UIRenderer>();
	merchantUI->AddComponent<MerchantUI>();

	///상인의 호출범위
	auto callingRange = std::make_shared<Entity>("callingRange");
	Scene::AddEntity(callingRange);
	callingRange->AddComponent<Transform>();
	callingRange->AddComponent<MeshRenderer>();
	callingRange->AddComponent<SphereCollider>();
	callingRange->GetComponent<SphereCollider>()->SetRadius(20.f);
	callingRange->GetComponent<SphereCollider>()->SetDisableGravity(true);
	callingRange->GetComponent<SphereCollider>()->SetIsTrigger(true);
	callingRange->AddComponent<ShopKeeperCallingScript>();
	callingRange->SetLayerCollision(LayerCollision::CallingRange); ///----------------------------------------------------------- 레이어 설정

	///호출범위 상인 엔티티에 붙이기
	merchant->GetComponent<Transform>()->SetChild(callingRange->GetComponent<Transform>());
	callingRange->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 0));

	int m_range = 10;
	///상인의 눈치 범위
//정면 ray
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(0)->SetUnitDirectionOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(0)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(0)->SetDistance(m_range);

	//왼쪽 15도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(1)->SetUnitDirectionOffset(Vector3(-0.26, 0, 1));
	merchant->GetComponent<RayCaster>(1)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(1)->SetDistance(m_range);

	//오른쪽 15도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(2)->SetUnitDirectionOffset(Vector3(0.26, 0, 1));
	merchant->GetComponent<RayCaster>(2)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(2)->SetDistance(m_range);

	//왼쪽 7.5도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(3)->SetUnitDirectionOffset(Vector3(-0.13, 0, 1));
	merchant->GetComponent<RayCaster>(3)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(3)->SetDistance(m_range);

	//오른쪽 7.5도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(4)->SetUnitDirectionOffset(Vector3(0.13, 0, 1));
	merchant->GetComponent<RayCaster>(4)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(4)->SetDistance(m_range);


	/// 푸드 테이블
	auto table = std::make_shared<Entity>("SampleTable");
	auto tableRenderer = table->AddComponent<MeshRenderer>("TableRenderer");
	auto tableMesh = table->AddComponent<MeshFilter>("SampleTable");
	table->AddComponent<SphereCollider>();
	table->GetComponent<SphereCollider>()->SetRadius(3.f);
	table->GetComponent<SphereCollider>()->SetDisableGravity(true);
	table->GetComponent<SphereCollider>()->SetIsTrigger(true);
	auto tableScript = table->AddComponent<FoodTableScript>(FoodType::Bagle);
	Scene::AddEntity(table);
	tableMesh->SetModelKey(GraphicsTransfer::ModelKey::FoodTable);
	store->GetComponent<Transform>()->SetChild(table->GetComponent<Transform>()); // 음식 가게의 자식으로 설정
	table->AddComponent<BoxCollider>()->SetSize({ 2.9f,1.f,1.3f });				/// 박스 콜라이더
	table->AddComponent<BoxCollider>()->SetOffset({ 0.f, 1.f, 0.f });			/// 박스 콜라이더
	table->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);			/// 박스 콜라이더
	table->GetComponent<Transform>()->AddPosition(Vector3(-0.1f, 0.f, -4.9f));
	table->SetLayerCollision(LayerCollision::Interactable); ///----------------------------------------------------------- 레이어 설정

	/// 푸드
	auto food = std::make_shared<Entity>("SampleFood");
	auto foodRenderer = food->AddComponent<MeshRenderer>("FoodRenderer");
	auto foodMesh = food->AddComponent<MeshFilter>("SampleFood");
	auto foodTransform = food->GetComponent<Transform>();
	Scene::AddEntity(food);
	foodMesh->SetModelKey(GraphicsTransfer::ModelKey::SampleFood);
	table->GetComponent<Transform>()->SetChild(food->GetComponent<Transform>()); // 음식 테이블의 자식으로 설정
	foodTransform->SetLocalPosition(Vector3(0.f, 0.9f, 0.f));
	foodTransform->SetLocalRotation(Vector3(0.f, -5.f, 0.f));

	/// 인터랙티브 키 (음식 테이블)
	auto foodTableInterKey = std::make_shared<Entity>("InteractiveKey (Food Table)");
	auto foodTableIterKeyRenderer = foodTableInterKey->AddComponent<UIRenderer>("InteractiveKeyRenderer");
	auto foodTableInterKeyMesh = foodTableInterKey->AddComponent<MeshFilter>("InteractiveKeyMesh");
	auto foodTableInterKeyScript = foodTableInterKey->AddComponent<InterKeyScript>();
	foodTableInterKeyScript->m_foodTableScript = table->GetComponent<FoodTableScript>();
	Scene::AddEntity(foodTableInterKey);
	foodTableInterKeyMesh->SetModelKey(GraphicsTransfer::ModelKey::QTE_InteractiveKey);
	table->GetComponent<Transform>()->SetChild(foodTableInterKey->GetComponent<Transform>()); // 음식 테이블의 자식으로 설정
	auto foodTableInterKeyTransform = foodTableInterKey->GetComponent<Transform>();
	foodTableInterKeyTransform->SetLocalPosition(Vector3(0.f, 2.2f, 0.f));
	foodTableInterKeyTransform->SetLocalScale(Vector3(0.01f, 0.01f, 0.01f));

	store->GetComponent<Transform>()->SetWorldPosition(Vector3(x, 0, y));
	tableScript->SetPointerSense(merchant->GetComponent<ShopKeeperFSM>()->GetPointerSense());


	int a = ranInt(gen);
	food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::SampleFood);

	switch (type)
	{
	case 1: // 빵
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::BagleStore);

		if (a == 0) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Bagle);
		if (a == 1) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::BaguettePoint);
		break;

	case 2: // 디저트
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::CupCakeStore);

		if (a == 0) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Cake);
		if (a == 1) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Cake);
		break;

	case 3:  // 과일
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::AppleStore);

		if (a == 1) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::WaterMelon);
		if (a == 0) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Banana);
		break;

	case 4:  // 해산물
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::FishStore);

		if (a == 1) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Fish);
		if (a == 0) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Squid);
		break;

	case 5:  // 고기
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::SausageStore);

		if (a == 0) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::CartoonMeat);
		if (a == 1) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Ham);
		if (a == 2) table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Sausage);
		break;
	}

	//store->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, position, 0.f));


	switch (dir)
	{
	case 0:  //정면
		break;

	case 1: // 오른쪽
		storeTransform->SetLocalRotation(Vector3(0, 90, 0));
		break;

	case 2: //왼쪽
		storeTransform->SetLocalRotation(Vector3(0, -90, 0));
		break;
	}

	/// 스타트 함수가 불리고 이 둘을 연결하기 위함
	m_tableScripts.push_back(tableScript);
	m_shopkeeperFSMs.push_back(merchant->GetComponent<ShopKeeperFSM>());
}

void STAGE1::CreateRestaurant(int x, int y, int dir, int type)
{
	///Shop
	/// 음식 가게
	auto store = std::make_shared<Entity>("SampleStore");
	auto storeRenderer = store->AddComponent<MeshRenderer>("StoreRenderer");
	auto storeMesh = store->AddComponent<MeshFilter>("SampleStore");
	auto storeTransform = store->GetComponent<Transform>();
	//storeTransform->AddPosition(Vector3(x, 0.f, y));
	Scene::AddEntity(store);
	//storeMesh->SetModelKey(GraphicsTransfer::ModelKey::BagleStore);
	store->AddComponent<BoxCollider>()->SetSize({ 7.f,7.5f,5.8f });
	store->GetComponent<BoxCollider>()->SetOffset({ -0.7,3.8,0.2 });
	store->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);
	store->SetLayerCollision(LayerCollision::Store); ///----------------------------------------------------------- 레이어 설정
	//store->AddComponent<BoxCollider>()->SetBodyType(BodyType::Static);

	/// 상인 
	auto merchant = std::make_shared<Entity>("SampleMerchant");
	Scene::AddEntity(merchant);
	merchant->AddComponent<ShopKeeperFSM>();
	merchant->AddComponent<ShopKeeperDoubtScript>();
	auto merchantRenderer = merchant->AddComponent<MeshRenderer>("MerchantRenderer");
	auto merchantMesh = merchant->AddComponent<MeshFilter>("PlayerMesh");
	merchantMesh->SetModelKey(GraphicsTransfer::ModelKey::Merchant);
	store->GetComponent<Transform>()->SetChild(merchant->GetComponent<Transform>()); // 음식 가게의 자식으로 설정
	merchant->GetComponent<Transform>()->AddPosition(Vector3(-0.1f, 0.f, -3.4f));
	merchant->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
	/*merchant->AddComponent<CapsuleCollider>()->FreezeRotation(true, true, true);		 /// 캡슐 콜라이더
	merchant->GetComponent<CapsuleCollider>()->SetOffset({ 0.f, 1.f, 0.f });			 /// 캡슐 콜라이더
	merchant->GetComponent<CapsuleCollider>()->SetBodyType(BodyType::Static);		 /// 캡슐 콜라이더*/
	merchant->GetComponent<ShopKeeperFSM>()->Start();
	merchant->SetLayerCollision(LayerCollision::Merchant); ///----------------------------------------------------------- 레이어 설정
	/*merchant->GetComponent<CapsuleCollider>()->SetOffset();*/

	/// 상인의 UI
	auto merchantUI = std::make_shared<Entity>("MerchantUI");
	Scene::AddEntity(merchantUI);
	merchant->GetComponent<Transform>()->SetChild(merchantUI->GetComponent<Transform>());
	merchantUI->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Merchant);
	merchantUI->AddComponent<UIRenderer>();
	merchantUI->AddComponent<MerchantUI>();

	///상인의 호출범위
	auto callingRange = std::make_shared<Entity>("callingRange");
	Scene::AddEntity(callingRange);
	callingRange->AddComponent<Transform>();
	callingRange->AddComponent<MeshRenderer>();
	callingRange->AddComponent<SphereCollider>();
	callingRange->GetComponent<SphereCollider>()->SetRadius(20.f);
	callingRange->GetComponent<SphereCollider>()->SetDisableGravity(true);
	callingRange->GetComponent<SphereCollider>()->SetIsTrigger(true);
	callingRange->AddComponent<ShopKeeperCallingScript>();
	callingRange->SetLayerCollision(LayerCollision::CallingRange); ///----------------------------------------------------------- 레이어 설정

	///호출범위 상인 엔티티에 붙이기
	merchant->GetComponent<Transform>()->SetChild(callingRange->GetComponent<Transform>());
	callingRange->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 0));

	int m_range = 10;
	///상인의 눈치 범위
//정면 ray
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(0)->SetUnitDirectionOffset(Vector3(0, 0, 1));
	merchant->GetComponent<RayCaster>(0)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(0)->SetDistance(m_range);

	//왼쪽 15도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(1)->SetUnitDirectionOffset(Vector3(-0.26, 0, 1));
	merchant->GetComponent<RayCaster>(1)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(1)->SetDistance(m_range);

	//오른쪽 15도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(2)->SetUnitDirectionOffset(Vector3(0.26, 0, 1));
	merchant->GetComponent<RayCaster>(2)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(2)->SetDistance(m_range);

	//왼쪽 7.5도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(3)->SetUnitDirectionOffset(Vector3(-0.13, 0, 1));
	merchant->GetComponent<RayCaster>(3)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(3)->SetDistance(m_range);

	//오른쪽 7.5도
	merchant->AddComponent<RayCaster>();
	merchant->GetComponent<RayCaster>(4)->SetUnitDirectionOffset(Vector3(0.13, 0, 1));
	merchant->GetComponent<RayCaster>(4)->SetStartPosOffset(Vector3(0, 1, 1));
	merchant->GetComponent<RayCaster>(4)->SetDistance(m_range);


	/// 푸드 테이블
	auto table = std::make_shared<Entity>("SampleTable");
	auto tableRenderer = table->AddComponent<MeshRenderer>("TableRenderer");
	auto tableMesh = table->AddComponent<MeshFilter>("SampleTable");
	table->AddComponent<SphereCollider>();
	table->GetComponent<SphereCollider>()->SetRadius(3.f);
	table->GetComponent<SphereCollider>()->SetDisableGravity(true);
	table->GetComponent<SphereCollider>()->SetIsTrigger(true);
	auto tableScript = table->AddComponent<FoodTableScript>(FoodType::Bagle);
	Scene::AddEntity(table);
	tableMesh->SetModelKey(GraphicsTransfer::ModelKey::FoodTable);
	store->GetComponent<Transform>()->SetChild(table->GetComponent<Transform>()); // 음식 가게의 자식으로 설정
	table->GetComponent<Transform>()->SetLocalPosition({ -0.1, 0.0f, -4.9f });
	table->AddComponent<BoxCollider>()->SetSize({ 2.9f,1.f,1.3f });				/// 박스 콜라이더
	table->AddComponent<BoxCollider>()->SetOffset({ 0.f, 1.f, 0.f });			/// 박스 콜라이더
	table->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);			/// 박스 콜라이더
	table->SetLayerCollision(LayerCollision::Interactable); ///----------------------------------------------------------- 레이어 설정

	/// 푸드
	auto food = std::make_shared<Entity>("SampleFood");
	auto foodRenderer = food->AddComponent<MeshRenderer>("FoodRenderer");
	auto foodMesh = food->AddComponent<MeshFilter>("SampleFood");
	auto foodTransform = food->GetComponent<Transform>();
	foodTransform->SetLocalPosition(Vector3(0.f, 0.9f, 0.f));
	foodTransform->SetLocalRotation(Vector3(0.f, -5.f, 0.f));

	Scene::AddEntity(food);
	foodMesh->SetModelKey(GraphicsTransfer::ModelKey::SampleFood);
	table->GetComponent<Transform>()->SetChild(food->GetComponent<Transform>()); // 음식 테이블의 자식으로 설정

	/// 인터랙티브 키 (음식 테이블)
	auto foodTableInterKey = std::make_shared<Entity>("InteractiveKey (Food Table)");
	auto foodTableIterKeyRenderer = foodTableInterKey->AddComponent<UIRenderer>("InteractiveKeyRenderer");
	auto foodTableInterKeyMesh = foodTableInterKey->AddComponent<MeshFilter>("InteractiveKeyMesh");
	auto foodTableInterKeyScript = foodTableInterKey->AddComponent<InterKeyScript>();
	foodTableInterKeyScript->m_foodTableScript = table->GetComponent<FoodTableScript>();
	Scene::AddEntity(foodTableInterKey);
	foodTableInterKeyMesh->SetModelKey(GraphicsTransfer::ModelKey::QTE_InteractiveKey);
	table->GetComponent<Transform>()->SetChild(foodTableInterKey->GetComponent<Transform>()); // 음식 테이블의 자식으로 설정
	auto foodTableInterKeyTransform = foodTableInterKey->GetComponent<Transform>();
	foodTableInterKeyTransform->SetLocalPosition(Vector3(0.f, 2.2f, 0.f));
	foodTableInterKeyTransform->SetLocalScale(Vector3(0.01f, 0.01f, 0.01f));

	store->GetComponent<Transform>()->SetWorldPosition(Vector3(x, 0, y));
	tableScript->SetPointerSense(merchant->GetComponent<ShopKeeperFSM>()->GetPointerSense());
	tableScript->SetDelicious(true);

	food->GetComponent<MeshFilter>()->SetModelKey(ModelKey::SampleFood);
	switch (type)
	{
	case 2: // 빵
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::BagleStore);
		table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::BaguetteWheel);
		break;

	case 1: // 디저트
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::CupCakeStore);
		table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::CupCake);
		break;

	case 3:  // 과일
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::AppleStore);
		table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Apple);
		break;

	case 4:  // 해산물
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::FishStore);
		table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Crab);
		break;

	case 5:  // 고기
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::SausageStore);
		table->GetComponent<FoodTableScript>()->SetFoodType(FoodType::Ham);
		break;
	}

	switch (dir)
	{
	case 0:  //정면
		break;

	case 1: // 오른쪽
		storeTransform->SetLocalRotation(Vector3(0, 90, 0));
		break;

	case 2: //왼쪽
		storeTransform->SetLocalRotation(Vector3(0, -90, 0));
		break;
	}

	/// 스타트 함수가 불리고 이 둘을 연결하기 위함
	m_tableScripts.push_back(tableScript);
	m_shopkeeperFSMs.push_back(merchant->GetComponent<ShopKeeperFSM>());
}

void STAGE1::CreateFountain(int x, int y)
{
	auto fountain = std::make_shared<Entity>("Fountain");
	Scene::AddEntity(fountain);
	fountain->AddComponent<MeshRenderer>();
	auto mesh = fountain->AddComponent<MeshFilter>();
	mesh->SetModelKey(GraphicsTransfer::ModelKey::Fountain);
	fountain->AddComponent<BoxCollider>()->SetSize({ 4,3,4 });
	fountain->GetComponent<BoxCollider>()->SetOffset({ 0.f,1.f,0.f });
	fountain->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);
	fountain->GetComponent<Transform>()->SetWorldPosition(Vector3(x, 0.f, y));
}

void STAGE1::CreateMap(int info, int x, int y, int direction, int minigame)
{
	Info id = static_cast<Info>(info);


	switch (id)
	{
			break;

	case STAGE1::Info::Wall:
		//CreateWall(x, y);
		break;
	case STAGE1::Info::AutoBush: // 자동부쉬 (잔디)
		CreateAutoBush(x, y);
		break;
	case STAGE1::Info::InterBush: // 수동부쉬 (통)
		CreateInterBush(x, y);
		break;
	case STAGE1::Info::Potion: // 아이템 (박스)
		CreatePotion(x, y);
		break;
	case STAGE1::Info::store: // 가게
		CreateStore(x, y, direction, minigame);
		break;
	case STAGE1::Info::Restaurant: // 맛집
		CreateRestaurant(x, y, direction, minigame);
		break;
	case STAGE1::Info::Plaza: // 분수대
		CreateFountain(x, y);
		break;
	case STAGE1::Info::Guard: // 경비병
		CreateGuard(x, y);
		break;
	case STAGE1::Info::Player: // 플레이어
		break;
	case STAGE1::Info::Streetlight: // 가로등
		//CreateStreetLight(x, y);
		break;
	case STAGE1::Info::Tree: // 나무
		//CreateTree(x, y);
		break;
	case STAGE1::Info::End:
		break;
	default:
		break;
	}
}

void STAGE1::CreateDeco(DecoType type, float x, float y, float yRotation /*= 0.f*/, float scale)
{
	auto deco = std::make_shared<Entity>("DecoObject");
	Scene::AddEntity(deco);
	deco->AddComponent<MeshRenderer>();
	auto mesh = deco->AddComponent<MeshFilter>();
	deco->GetComponent<Transform>()->SetWorldPosition(Vector3(x, 0.f, y));
	deco->GetComponent<Transform>()->SetWorldRotation(Vector3(0, yRotation, 0));
	deco->GetComponent<Transform>()->SetWorldScale(Vector3(scale, scale, scale));

	switch (type)
	{
	case STAGE1::DecoType::Grass:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Grass);
		break;
	case STAGE1::DecoType::Tree1:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Tree1);
		break;
	case STAGE1::DecoType::Tree2:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Tree2);
		break;
	case STAGE1::DecoType::Table:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Table);
		break;
	case STAGE1::DecoType::Flag:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Flag);
		break;
	case STAGE1::DecoType::Candle1:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Candle1);
		deco->AddComponent<PointLight>()->SetPosOffset(Vector3(0,2.9 * scale,-0.2 * scale));
		deco->AddComponent<TorchLight>();
		break;
	case STAGE1::DecoType::Candle2:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Candle2);
		deco->AddComponent<PointLight>()->SetPosOffset(Vector3(0, 2.9 * scale, -0.2 * scale));
		deco->AddComponent<TorchLight>();
		break;
	case STAGE1::DecoType::Candy:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Candy);
		deco->AddComponent<PointLight>();
		deco->AddComponent<TorchLight>();
		break;
	case STAGE1::DecoType::Lamp:
		mesh->SetModelKey(GraphicsTransfer::ModelKey::Lamp);
		deco->AddComponent<PointLight>()->SetPosOffset(Vector3(0, 4.0 * scale, -0.2 * scale));
		deco->AddComponent<TorchLight>();
		break;
	}
}
