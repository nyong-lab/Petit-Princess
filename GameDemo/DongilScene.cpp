#include "GameDemo_pch.h"
#include "DongilScene.h"

/// �Ŵ���
#include "IGraphics.h"
#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "CameraManager.h"

/// ��ƼƼ, ������
#include "Entity.h"
#include "Grid.h"
#include "TextureBox.h"
#include "CubeMap.h"
#include "UI.h"
#include "GuardPrefab.h"
#include "UIGauge.h"
#include "RedBox.h"
#include "UISense.h"
#include "UIDeliciousFood.h"
#include "UIReVisitCoolTime.h"
#include "UIHungry.h"


/// FSM
#include "PlayerFSM.h"

/// ������Ʈ
#include "MeshRenderer.h"
#include "UIRenderer.h"
#include "DirectionalLight.h"
#include "RayCaster.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"
#include "ShopKeeperFSM.h"
#include "TextRender.h"
#include "GameManager.h"
#include "PointLight.h"

/// ��ũ��Ʈ
#include "TestingScript.h"
#include "DongilScript.h"
#include "PetitCamera.h"
#include "FoodTableScript.h"
#include "InterKeyScript.h"
#include "ShopKeeperDoubtScript.h"
#include "PlayerMoveScript.h"
#include "UITestScript.h"
#include "GameManager.h"
#include "SunLight.h"
#include "TorchLight.h"
#include "HideInteractionScript.h"
#include "TimerUI.h"
#include "HideAutomaticScript.h"
#include "TechniquesScript.h"
#include "ScoreUI.h"
#include "PopUpUI.h"

DongilScene::DongilScene(std::wstring sceneName) : Scene(sceneName)
{

}

void DongilScene::Start()
{
	m_cameraManager.lock()->GetMainCamera()->SetPosition(Vector3(11, 20, -30));
	m_cameraManager.lock()->GetMainCamera()->AddLookRadian(Vector2(-0.5, -0.5f));

	/// ���� �޴���
	auto mgr = std::make_shared<Entity>("GameManager");
	Scene::AddEntity(mgr);
	auto manager = mgr->AddComponent<GameManager>();	

	// ���� �޴����� �ڽ����� �߰� -> �ð�
	auto timemgr = std::make_shared<Entity>("TimeUI");
	Scene::AddEntity(timemgr);
	timemgr->SetParent(mgr.get());
	auto light = timemgr->AddComponent<DirectionalLight>();
	light->SetIntensity(0.14);
	light->SetAmbient255(Vector4(255, 255, 255, 247));
	timemgr->AddComponent<SunLight>();
	timemgr->AddComponent<TimerUI>();
	timemgr->AddComponent<TextRender>();
	timemgr->AddComponent<MeshFilter>();
	timemgr->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Timer);
	timemgr->AddComponent<UIRenderer>();

	// ���� �޴����� �ڽ����� �߰� -> ������
// 	auto scoreMgr = std::make_shared<Entity>("ScoreUI");
// 	Scene::AddEntity(scoreMgr);

	/// ����Ʈ ����Ʈ
	auto torch1 = std::make_shared<Entity>("Torch");
	Scene::AddEntity(torch1);
	torch1->GetComponent<Transform>()->SetLocalPosition(Vector3(-4.7f, 4.1f, 5.5f));
	torch1->AddComponent<TorchLight>();
	torch1->AddComponent<PointLight>();

	/// ����Ʈ ����Ʈ
	auto torch2 = std::make_shared<Entity>("Torch");
	Scene::AddEntity(torch2);
	torch2->GetComponent<Transform>()->SetLocalPosition(Vector3(-15.f, 1.7f, 0.5f));
	torch2->AddComponent<TorchLight>();
	torch2->AddComponent<PointLight>();

	/// real floor
	auto gridEntity = std::make_shared<Grid>();
	gridEntity->SetName("RealPlane");
	Scene::AddEntity(gridEntity);
	gridEntity->AddComponent<PlaneCollider>();

	/// floor
	auto floor = std::make_shared<Entity>("SampleFloor");
	auto floorRenderer = floor->AddComponent<MeshRenderer>("FloorRenderer");
	auto floorMesh = floor->AddComponent<MeshFilter>("FloorMesh");
	floorMesh->m_pbrSet.useBaseColorMap = false;
	floorMesh->m_pbrSet.SetBaseColor({ 201, 148, 130, 255 });
	Scene::AddEntity(floor);
	floorMesh->SetModelKey(GraphicsTransfer::ModelKey::Floor);

	/// �÷��̾� 
	player = std::make_shared<Entity>("Player");
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
	transform->AddPosition(Vector3(5.f, 0.f, 0.f));
	collider->FreezeRotation(true, true, true);
	collider->SetSize(0.5f, 0.3f);
	collider->SetOffset(Vector3(0.f, 0.8f, 0.f));
	//player->AddComponent<PetitCamera>();
	//player->GetComponent<PetitCamera>()->SetTarget(player);
	Scene::AddEntity(player);
	playerMesh->SetModelKey(GraphicsTransfer::ModelKey::Player);
	manager->SetPlayer(player);

	/// �÷��̾��� �ڽ����� ScoreUI �߰�
	auto scoreUI = std::make_shared<Entity>("ScoreUI");
	Scene::AddEntity(scoreUI);
	scoreUI->GetComponent<Transform>()->SetParent(player->GetComponent<Transform>());
	scoreUI->AddComponent<ScoreUI>();
	scoreUI->AddComponent<UIRenderer>();
	scoreUI->AddComponent<MeshFilter>();
	scoreUI->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Score);

	/// �÷��̾��� �ڽ����� UI Hungry �߰�
	auto HungryUI = std::make_shared<UIHungry>();
	HungryUI->GetComponent<Transform>()->SetWorldPosition({ 800,-400,0 });
	HungryUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	HungryUI->AddComponent<UITestScript>(UIType::HUNGRY);
	auto hungryPointer = player->GetComponent<PlayerMoveScript>()->GetHungerPointer();
	HungryUI->GetComponent<UITestScript>()->SetHungry(hungryPointer);
	Scene::AddEntity(HungryUI);

	/// �÷��̾��� �ڽ����� ����Ʈ ����Ʈ �߰�
	auto torch3 = std::make_shared<Entity>("Torch");
	Scene::AddEntity(torch3);
	torch3->AddComponent<TorchLight>();
	torch3->AddComponent<PointLight>();
	player->GetComponent<Transform>()->SetChild(torch3->GetComponent<Transform>());
	torch3->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 2.6f, 0.f));

	/// ���� ����(���ɟ�)
	{
		auto store = std::make_shared<Entity>("CupcakeStore");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(-7.5f, 0.f, 9.4f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::CupCakeStore);
	}

	/// ���� ����(��)
	{
		auto store = std::make_shared<Entity>("BreadStore");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(0.4f, 0.f, 6.4f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::BagleStore);
	}

	/// �м���
	{
		auto store = std::make_shared<Entity>("Fountain");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(-7.f, 0.f, 1.1f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Fountain);
	}

	/// ����
	{
		auto store = std::make_shared<Entity>("Box1");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(-18.7f, 0.f, 1.f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Box1);
	}

	/// ����
	{
		auto store = std::make_shared<Entity>("Tree");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(3.9f, 0.f, 0.3f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Tree1);
	}

	/// �ܵ�
	{
		auto store = std::make_shared<Entity>("Grass");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(-2.6f, 0.f, 3.2f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Grass);
	}

	/// ���ε�
	{
		auto store = std::make_shared<Entity>("Lamp");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(-4.7f, 0.f, 6.5f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Lamp);
		store->AddComponent<PointLight>();
	}

	/// ����
	{
		auto store = std::make_shared<Entity>("Potion");
		auto storeRenderer = store->AddComponent<MeshRenderer>();
		auto storeMesh = store->AddComponent<MeshFilter>();
		auto storeTransform = store->GetComponent<Transform>();
		storeTransform->AddPosition(Vector3(-15.8f, 0.9f, -0.5f));
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::Potion);
	}

	/// ���� ����(����)
	auto store = std::make_shared<Entity>("SampleStore");
	auto storeRenderer = store->AddComponent<MeshRenderer>("StoreRenderer");
	auto storeMesh = store->AddComponent<MeshFilter>("SampleStore");
	auto storeTransform = store->GetComponent<Transform>();
	storeTransform->AddPosition(Vector3(0.6f, 0.f, 4.4f));
	Scene::AddEntity(store);
	storeMesh->SetModelKey(GraphicsTransfer::ModelKey::AppleStore);

	/// ���� 
	auto merchant = std::make_shared<Entity>("SampleMerchant");
	merchant->AddComponent<ShopKeeperFSM>();
	merchant->AddComponent<ShopKeeperDoubtScript>();
	auto merchantRenderer = merchant->AddComponent<MeshRenderer>("MerchantRenderer");
	auto merchantMesh = merchant->AddComponent<MeshFilter>("PlayerMesh");
	merchant->GetComponent<Transform>()->AddPosition(Vector3(0.f, 0.f, 1.f));
	Scene::AddEntity(merchant);
	merchantMesh->SetModelKey(GraphicsTransfer::ModelKey::Player);
	store->GetComponent<Transform>()->SetChild(merchant->GetComponent<Transform>()); // ���� ������ �ڽ����� ����

	/// ���� ��ġ ������ UI
	auto merchantSenseUI = std::make_shared<UISense>();
	merchantSenseUI->SetName("Sense");
	merchantSenseUI->GetComponent<Transform>()->SetWorldPosition({ 300,300,0 });
	merchantSenseUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	merchantSenseUI->AddComponent<UITestScript>(UIType::SENSE);
	Scene::AddEntity(merchantSenseUI);
	merchantSenseUI->SetRender(true);

	/// ���� ���� UI
	auto merchantDeliciousFoodUI = std::make_shared<UIDeliciousFood>();
	merchantDeliciousFoodUI->SetName("DeliciousFood");
	merchantDeliciousFoodUI->GetComponent<Transform>()->SetWorldPosition({ 600,600,0 });
	merchantDeliciousFoodUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	merchantDeliciousFoodUI->AddComponent<UITestScript>(UIType::DELICIOUS_FOOD);
	Scene::AddEntity(merchantDeliciousFoodUI);
	merchantDeliciousFoodUI->SetRender(true);

	/// ���� ��湮 UI
	auto merchantReVisitCoolTimeUI = std::make_shared<UIReVisitCoolTime>();
	merchantReVisitCoolTimeUI->SetName("ReVisitCoolTime");
	merchantReVisitCoolTimeUI->GetComponent<Transform>()->SetWorldPosition({ 900,900,0 });
	merchantReVisitCoolTimeUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	merchantReVisitCoolTimeUI->AddComponent<UITestScript>(UIType::REVISIT_COOLTIME);
	Scene::AddEntity(merchantReVisitCoolTimeUI);
	merchantReVisitCoolTimeUI->SetRender(true);

	// ���ο� ui�� ���̱�
	merchant->GetComponent<Transform>()->SetChild(merchantSenseUI->GetComponent<Transform>());
	merchant->GetComponent<Transform>()->SetChild(merchantDeliciousFoodUI->GetComponent<Transform>());
	merchant->GetComponent<Transform>()->SetChild(merchantReVisitCoolTimeUI->GetComponent<Transform>());

	/// Ǫ�� ���̺�
	auto table = std::make_shared<Entity>("SampleTable");
	auto tableRenderer = table->AddComponent<MeshRenderer>("TableRenderer");
	auto tableMesh = table->AddComponent<MeshFilter>("SampleTable");
	table->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	table->AddComponent<SphereCollider>();
	table->GetComponent<SphereCollider>()->SetRadius(3.f);
	table->GetComponent<SphereCollider>()->SetDisableGravity(true);
	table->GetComponent<SphereCollider>()->SetIsTrigger(true);
	auto tableScript = table->AddComponent<FoodTableScript>(FoodType::Bagle);
	Scene::AddEntity(table);
	tableMesh->SetModelKey(GraphicsTransfer::ModelKey::FoodTable);
	store->GetComponent<Transform>()->SetChild(table->GetComponent<Transform>()); // ���� ������ �ڽ����� ����

	/// Ǫ��
	auto food = std::make_shared<Entity>("SampleFood");
	auto foodRenderer = food->AddComponent<MeshRenderer>("FoodRenderer");
	auto foodMesh = food->AddComponent<MeshFilter>("SampleFood");
	auto foodTransform = food->GetComponent<Transform>();
	foodTransform->SetLocalPosition(Vector3(0.55f, 0.f, 0.f));
	foodTransform->AddPosition(Vector3(0.f, 1.f, 0.f));
	Scene::AddEntity(food);
	foodMesh->SetModelKey(GraphicsTransfer::ModelKey::SampleFood);
	table->GetComponent<Transform>()->SetChild(food->GetComponent<Transform>()); // ���� ���̺��� �ڽ����� ����
	table->GetComponent<Transform>()->AddPosition(Vector3(0.f, 0.f, -0.5f));

	storeTransform->AddPosition(Vector3(-15.2f, 0.f, 0.f));

	/// ���ͷ�Ƽ�� Ű (���� ���̺�)
	auto foodTableInterKey = std::make_shared<Entity>("InteractiveKey (Food Table)");
	auto foodTableIterKeyRenderer = foodTableInterKey->AddComponent<UIRenderer>("InteractiveKeyRenderer");
	auto foodTableInterKeyMesh = foodTableInterKey->AddComponent<MeshFilter>("InteractiveKeyMesh");
	auto foodTableInterKeyScript = foodTableInterKey->AddComponent<InterKeyScript>();
	foodTableInterKeyScript->m_foodTableScript = table->GetComponent<FoodTableScript>();
	Scene::AddEntity(foodTableInterKey);
	foodTableInterKeyMesh->SetModelKey(GraphicsTransfer::ModelKey::QTE_InteractiveKey);
	table->GetComponent<Transform>()->SetChild(foodTableInterKey->GetComponent<Transform>()); // ���� ���̺��� �ڽ����� ����
	auto foodTableInterKeyTransform = foodTableInterKey->GetComponent<Transform>();
	foodTableInterKeyTransform->SetLocalPosition(Vector3(0.f, 2.2f, 0.f));
	foodTableInterKeyTransform->SetLocalScale(Vector3(0.01f, 0.01f, 0.01f));

	/// ��� ������Ʈ
	auto techniquesObject = std::make_shared<TextureBox>();
	techniquesObject->SetName("TechniquesObject");
	Scene::AddEntity(techniquesObject);
	techniquesObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.0f, -5.f));
	techniquesObject->AddComponent<SphereCollider>();
	techniquesObject->GetComponent<SphereCollider>()->SetRadius(1.5f);
	techniquesObject->GetComponent<SphereCollider>()->SetDisableGravity(true);
	techniquesObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	techniquesObject->AddComponent<TechniquesScript>();

	/// ���ͷ�Ƽ�� Ű (��� ������Ʈ)
	auto techniquesInterKey = std::make_shared<Entity>("InteractiveKey (Techniques Object)");
	auto techniquesIterKeyRenderer = techniquesInterKey->AddComponent<UIRenderer>("InteractiveKeyRenderer");
	auto techniquesInterKeyMesh = techniquesInterKey->AddComponent<MeshFilter>("InteractiveKeyMesh");
	auto techniquesInterKeyScript = techniquesInterKey->AddComponent<InterKeyScript>();
	techniquesInterKeyScript->m_techniquesScript = techniquesObject->GetComponent<TechniquesScript>();
	Scene::AddEntity(techniquesInterKey);
	techniquesInterKeyMesh->SetModelKey(GraphicsTransfer::ModelKey::QTE_InteractiveKey);
	techniquesObject->GetComponent<Transform>()->SetChild(techniquesInterKey->GetComponent<Transform>()); // ��� ������Ʈ�� �ڽ����� ����
	auto techniquesInterKeyTransform = techniquesInterKey->GetComponent<Transform>();
	techniquesInterKeyTransform->SetLocalPosition(Vector3(0.f, 2.2f, 0.f));
	techniquesInterKeyTransform->SetLocalScale(Vector3(0.01f, 0.01f, 0.01f));

	/// ���� ������Ʈ (�ڵ�)
	auto m_hideAutomaticObject = std::make_shared<Entity>("HideAutomaticObject");
	Scene::AddEntity(m_hideAutomaticObject);
	m_hideAutomaticObject->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Bush);
	m_hideAutomaticObject->AddComponent<MeshRenderer>();
	m_hideAutomaticObject->GetComponent<Transform>()->SetLocalPosition(Vector3(-18.1f, 0.0f, -2.6f));
	m_hideAutomaticObject->AddComponent<SphereCollider>();
	m_hideAutomaticObject->GetComponent<SphereCollider>()->SetRadius(1.5f);
	m_hideAutomaticObject->GetComponent<SphereCollider>()->SetDisableGravity(true);
	m_hideAutomaticObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	m_hideAutomaticObject->AddComponent<HideAutomaticScript>();

	/// ���� ������Ʈ (����)
	auto m_hideInteractionObject = std::make_shared<Entity>("HideInteractionObject");
	Scene::AddEntity(m_hideInteractionObject);
	m_hideInteractionObject->AddComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::Barrel);;
	m_hideInteractionObject->AddComponent<MeshRenderer>();
	m_hideInteractionObject->GetComponent<Transform>()->SetLocalPosition(Vector3(-12.f, 0.f, -5.7f));
	m_hideInteractionObject->AddComponent<SphereCollider>();
	m_hideInteractionObject->GetComponent<SphereCollider>()->SetRadius(1.5f);
	m_hideInteractionObject->GetComponent<SphereCollider>()->SetDisableGravity(true);
	m_hideInteractionObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	m_hideInteractionObject->AddComponent<HideInteractionScript>();

	/// ���� ���� ǥ�ÿ� UI
	std::shared_ptr<UI> m_hideInteractionUI = std::make_shared<UI>();
	m_hideInteractionUI->SetName("PressToHide");
	m_hideInteractionUI->GetComponent<Transform>()->SetWorldPosition({ 500,000,0 });
	m_hideInteractionUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	m_hideInteractionUI->AddComponent<UITestScript>(UIType::STATIC);
	Scene::AddEntity(m_hideInteractionUI);
	m_hideInteractionUI->SetRender(false);
	// �ν��� ui ����
	m_hideInteractionObject->GetComponent<Transform>()->SetChild(m_hideInteractionUI->GetComponent<Transform>());

	///// ���  -�¿��� ��
	//auto GuardEntity = std::make_shared<GuardPrefab>();
	//Scene::AddEntity(GuardEntity);

	//GuardEntity->SetPos(Vector3(25, 0, 30));
	//GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(5, 0, 0));
	//GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(5, 0, -22));
	//GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(0, 0, -22));
	//GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(0, 0, 0));

	////��� ��������
	//auto returnGuard1 = std::make_shared<PinkBox>();
	//Scene::AddEntity(returnGuard1);
	//returnGuard1->GetComponent<Transform>()->SetLocalPosition(Vector3(25, -5.f, 30));

	////��������
	//auto returnGuard2 = std::make_shared<YellowBox>();
	//Scene::AddEntity(returnGuard2);
	//returnGuard2->GetComponent<Transform>()->SetLocalPosition(Vector3(25, -5.f, 30) + Vector3(5, 0, 0));

	//auto returnGuard3 = std::make_shared<YellowBox>();
	//Scene::AddEntity(returnGuard3);
	//returnGuard3->GetComponent<Transform>()->SetLocalPosition(Vector3(25, -5.f, 30) + Vector3(5, 0, -22));

	//auto returnGuard4 = std::make_shared<YellowBox>();
	//Scene::AddEntity(returnGuard4);
	//returnGuard4->GetComponent<Transform>()->SetLocalPosition(Vector3(25, -5.f, 30) + Vector3(0, 0, -22));


	// ESC ������ �� �� �˾� UI ---------------------- ���� �ڿ� �߰��ؾ� �մϴ�
	auto popUp = std::make_shared<Entity>("PopUpUI");
	Scene::AddEntity(popUp);
	popUp->AddComponent<PopUpUI>();
	popUp->AddComponent<UIRenderer>();
	popUp->AddComponent<MeshFilter>();
	popUp->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_IngamePopUp);


	Scene::Start();

	// FSM ���� ��ġ ������ - Ǫ�� ���̺� ��ũ��Ʈ ������ ����
	// FSM�� start �Լ��� ȣ�� �ǰ� �� ���ķ� ���� ���Ѿ��Ѵ�.
	tableScript->SetPointerSense(merchant->GetComponent<ShopKeeperFSM>()->GetPointerSense());
}

void DongilScene::Update(double dt)
{
	Scene::Update(dt);

	///----------�̺�Ʈ �Ŵ��� Ȯ�ο�------------
	if (InputManager::GetInstance()->IsKeyDown(VK_BACK))
	{
		EventManager::GetInstance()->SceneChange(m_sceneManager.lock()->GetScene("SunWoongScene"));
	}
}
