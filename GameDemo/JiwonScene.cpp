#include "GameDemo_pch.h"
#include "JiwonScene.h"

#include "SoundManager.h"

#include "CameraModule.h"

#include "IGraphics.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"

#include "Grid.h"
#include "CubeMap.h"
#include "TextureBox.h"
#include "UI.h"
#include "UIGauge.h"
#include "ModelTest.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"
#include "Transform.h"
#include "RayCaster.h"
#include "MeshRenderer.h"
#include "UIRenderer.h"
#include "SunLight.h"
#include "TextRender.h"
#include "TimerUI.h"

#include "PlayerFSM.h"
#include "ShopKeeperFSM.h"

#include "PlayerMoveScript.h"
#include "UITestScript.h"
#include "HideAutomaticScript.h"
#include "HideInteractionScript.h"
#include "InterKeyScript.h"
#include "ShopKeeperDoubtScript.h"
#include "FoodTableScript.h"
#include "ShopKeeperCallingScript.h"
#include "UIMinimap.h"
#include "UIMinimapCurPos.h"

#include "PlayerPrefab.h"
#include "GuardPrefab.h"

#include "DirectionalLight.h"

#include "RedBox.h"


#include "ResourceManager.h"
#include "GameManager.h"
#include "ScoreUI.h"
#include "UIDataSyncScript.h"


JiwonScene::JiwonScene(std::wstring sceneName) : Scene(sceneName)
{
	// ������ ���� ������ ��ġ�ϴ� ĭ�� ������ ����
	// ���� ������ ���� �������, ���� �ƴ϶� ĭ ���� ��ġ�ϰ� �Ϸ��� ��� X�� Z���� 0.5�� ����� ��
	// ������ ��񺴵��� ���� �������� �����̴� ������ �Ǿ������� �ϴ��� �׳� �д�

	m_shopData = /*���߿� ������ �ٸ� ���ͷ� ����*/
	{
		{Vector3(8.f, 0.f, 6.f), EntityDirection::RIGHT},			//1
		{Vector3(35.f, 0.f, 10.f), EntityDirection::RIGHT},			//2
		{Vector3(18.f, 0.f, 21.f), EntityDirection::RIGHT},			//3		// ���: ����
		{Vector3(40.5f, 0.f, 31.f), EntityDirection::DOWN},			//4		// ���: X����ġ�� �ָ��ؼ� .5�� �س���
		{Vector3(20.f, 0.f, 42.f), EntityDirection::RIGHT},			//5
		{Vector3(60.5f, 0.f, 28.f), EntityDirection::DOWN},			//6		// ���: X����ġ�� �ָ��ؼ� .5�� �س���
		{Vector3(7.f, 0.f, 57.f), EntityDirection::DOWN},			//7
		{Vector3(76.f, 0.f, 15.f), EntityDirection::DOWN},			//8		// ���: �� ó���� ���� ����
		{Vector3(27.f, 0.f, 72.f), EntityDirection::DOWN},			//9
		{Vector3(43.f, 0.f, 59.f), EntityDirection::RIGHT},			//10

		{Vector3(61.f, 0.f, 48.f), EntityDirection::RIGHT},			//11
		{Vector3(80.f, 0.f, 36.f), EntityDirection::DOWN},			//12
		{Vector3(98.f, 0.f, 29.f), EntityDirection::LEFT},			//13
		{Vector3(113.f, 0.f, 20.f), EntityDirection::LEFT},			//14
		{Vector3(122.f, 0.f, 5.f), EntityDirection::RIGHT},			//15
		{Vector3(134.f, 0.f, 19.f), EntityDirection::LEFT},			//16	// ���: ����
		{Vector3(145.f, 0.f, 6.f), EntityDirection::LEFT},			//17
		{Vector3(126.f, 0.f, 33.f), EntityDirection::LEFT},			//18
		{Vector3(137.f, 0.f, 45.f), EntityDirection::LEFT},			//19
		{Vector3(99.f, 0.f, 49.f), EntityDirection::LEFT},			//20

		{Vector3(115.f, 0.f, 56.f), EntityDirection::LEFT},			//21
		{Vector3(129.f, 0.f, 66.f), EntityDirection::RIGHT},		//22
		{Vector3(144.f, 0.f, 76.f), EntityDirection::LEFT},			//23
		{Vector3(131.f, 0.f, 85.f), EntityDirection::RIGHT},		//24	// ���: ����
		{Vector3(111.f, 0.f, 95.f), EntityDirection::LEFT},			//25
		{Vector3(129.f, 0.f, 103.f), EntityDirection::DOWN},		//26
		{Vector3(144.f, 0.f, 103.f), EntityDirection::DOWN},		//27
		{Vector3(124.f, 0.f, 119.f), EntityDirection::LEFT},		//28
		{Vector3(140.f, 0.f, 131.f), EntityDirection::LEFT},		//29
		{Vector3(143.f, 0.f, 144.f), EntityDirection::LEFT},		//30

		{Vector3(122.f, 0.f, 142.f), EntityDirection::DOWN},		//31
		{Vector3(105.f, 0.f, 123.f), EntityDirection::RIGHT},		//32
		{Vector3(100.f, 0.f, 139.f), EntityDirection::LEFT},		//33
		{Vector3(76.f, 0.f, 140.f), EntityDirection::DOWN},			//34	// ���: ����
		{Vector3(83.f, 0.f, 124.f), EntityDirection::DOWN},			//35
		{Vector3(50.f, 0.f, 139.f), EntityDirection::RIGHT},		//36
		{Vector3(56.f, 0.f, 122.f), EntityDirection::LEFT},			//37
		{Vector3(30.f, 0.f, 132.f), EntityDirection::LEFT},			//38
		{Vector3(9.f, 0.f, 141.f), EntityDirection::RIGHT},			//39
		{Vector3(13.f, 0.f, 120.f), EntityDirection::DOWN},			//40

		{Vector3(21.f, 0.f, 101.f), EntityDirection::DOWN},			//41
		{Vector3(34.f, 0.f, 114.f), EntityDirection::LEFT},			//42
		{Vector3(39.f, 0.f, 89.f), EntityDirection::RIGHT},			//43
		{Vector3(53.f, 0.f, 102.f), EntityDirection::DOWN},			//44
		{Vector3(57.f, 0.f, 79.f), EntityDirection::RIGHT},			//45
		{Vector3(69.f, 0.f, 62.f), EntityDirection::DOWN},			//46
		{Vector3(91.f, 0.f, 64.f), EntityDirection::LEFT},			//47
		{Vector3(113.f, 0.f, 75.f), EntityDirection::DOWN},			//48
		{Vector3(97.f, 0.f, 80.f), EntityDirection::LEFT},			//49
		{Vector3(97.f, 0.f, 109.f), EntityDirection::LEFT},			//50

		{Vector3(72.f, 0.f, 111.f), EntityDirection::RIGHT},		//51
		{Vector3(76.f, 0.f, 94.f), EntityDirection::DOWN},			//52
		{Vector3(16.f, 0.f, 85.f), EntityDirection::LEFT},			//53	// ���: ����
	};

	m_guardData =
	{
		{Vector3(25.f, 0.f, 30.f), EntityDirection::DOWN},			//1
		{Vector3(43.f, 0.f, 18.f), EntityDirection::LEFT},			//2		// start is wall �ɸ��¾�
		{Vector3(16.f, 0.f, 52.f), EntityDirection::UP},			//3
		{Vector3(10.f, 0.f, 67.f), EntityDirection::UP},			//4
		{Vector3(47.f, 0.f, 81.f), EntityDirection::LEFT},			//5

		{Vector3(5.f, 0.f, 93.f), EntityDirection::RIGHT},			//6
		{Vector3(41.f, 0.f, 97.f), EntityDirection::LEFT},			//7
		{Vector3(24.f, 0.f, 111.f), EntityDirection::LEFT},			//8
		{Vector3(8.f, 0.f, 131.f), EntityDirection::RIGHT},			//9
		{Vector3(34.f, 0.f, 145.f), EntityDirection::RIGHT},		//10

		{Vector3(46.f, 0.f, 124.f), EntityDirection::LEFT},			//11
		{Vector3(48.f, 0.f, 38.f), EntityDirection::RIGHT},			//12
		{Vector3(91.f, 0.f, 29.f), EntityDirection::LEFT},			//13
		{Vector3(113.f, 0.f, 10.f), EntityDirection::LEFT},			//14
		{Vector3(114.f, 0.f, 35.f), EntityDirection::UP},			//15	// start is wall �ɸ��¾�

		{Vector3(122.f, 0.f, 25.f), EntityDirection::DOWN},			//16
		{Vector3(61.f, 0.f, 54.f), EntityDirection::LEFT},			//17
		{Vector3(53.f, 0.f, 131.f), EntityDirection::RIGHT},		//18
		{Vector3(64.f, 0.f, 111.f), EntityDirection::LEFT},			//19
		{Vector3(69.f, 0.f, 147.f), EntityDirection::LEFT},			//20

		{Vector3(82.f, 0.f, 115.f), EntityDirection::RIGHT},		//21
		{Vector3(78.f, 0.f, 85.f), EntityDirection::LEFT},			//22
		{Vector3(70.f, 0.f, 68.f), EntityDirection::RIGHT},			//23
		{Vector3(88.f, 0.f, 137.f), EntityDirection::UP},			//24
		{Vector3(99.f, 0.f, 101.f), EntityDirection::LEFT},			//25

		{Vector3(100.f, 0.f, 64.f), EntityDirection::RIGHT},		//26
		{Vector3(93.f, 0.f, 53.f), EntityDirection::LEFT},			//27
		{Vector3(103.f, 0.f, 132.f), EntityDirection::LEFT},		//28
		{Vector3(114.f, 0.f, 148.f), EntityDirection::RIGHT},		//29
		{Vector3(132.f, 0.f, 131.f), EntityDirection::LEFT},		//30

		{Vector3(122.f, 0.f, 77.f), EntityDirection::RIGHT},		//21
		{Vector3(124.f, 0.f, 53.f), EntityDirection::UP},			//22	// start is wall �ɸ��¾�
		{Vector3(139.f, 0.f, 112.f), EntityDirection::LEFT},		//23
		{Vector3(137.f, 0.f, 94.f), EntityDirection::LEFT},			//24
	};

	m_bushData =
	{
		Vector3(21.f, 0.f, 6.f),		//1
		//Vector3(52.f, 0.f, 6.f),		//2
		//Vector3(47.f, 0.f, 14.f),		//3
		//Vector3(36.f, 0.f, 22.f),		//4
		//Vector3(33.f, 0.f, 43.f),		//5
		//Vector3(69.f, 0.f, 34.f),		//6
		//Vector3(77.f, 0.f, 46.f),		//7
		//Vector3(90.5f, 0.f, 40.f),		//8
		//Vector3(107.f, 0.f, 26.5f),		//9
		//Vector3(103.f, 0.f, 15.f),		//10

		//Vector3(105.f, 0.f, 5.f),		//11
		//Vector3(119.f, 0.f, 12.f),		//12
		//Vector3(27.f, 0.f, 57.f),		//13
		//Vector3(17.f, 0.f, 77.f),		//14
		//Vector3(40.f, 0.f, 74.f),		//15
		//Vector3(67.f, 0.f, 76.f),		//16
		//Vector3(56.f, 0.f, 91.f),		//17
		//Vector3(11.f, 0.f, 109.f),		//18
		//Vector3(23.f, 0.f, 124.f),		//19
		//Vector3(47.5f, 0.f, 113.f),		//20

		//Vector3(75.f, 0.f, 127.f),		//21
		//Vector3(65.f, 0.f, 140.f),		//22
		//Vector3(118.f, 0.f, 129.f),		//23
		//Vector3(92.f, 0.f, 91.f),		//23
		//Vector3(125.f, 0.f, 86.f),		//25
	};

	m_potionData = /*���������� �߽��� ĭ�� �ƴ϶� ������ �����־ �ٸ��ֵ��̶� ����� ��ġ�� �����ַ��� .5�� �ۼ�*/
	{
		Vector3(8.5f, 0.f, 13.5f),		//1
		Vector3(8.5f, 0.f, 34.5f),		//2
		Vector3(57.5f, 0.f, 14.5f),		//3
		Vector3(50.5f, 0.f, 27.5f),		//4
		Vector3(76.5f, 0.f, 23.5f),		//5

		Vector3(40.5f, 0.f, 41.5f),		//6
		Vector3(96.5f, 0.f, 15.5f),		//7
		Vector3(136.5f, 0.f, 10.5f),	//8
		Vector3(140.5f, 0.f, 30.5f),	//9
		Vector3(143.5f, 0.f, 57.5f),	//10

		Vector3(146.5f, 0.f, 85.5f),	//11
		Vector3(110.5f, 0.f, 85.5f),	//12
		Vector3(111.5f, 0.f, 110.5f),	//13
		Vector3(107.5f, 0.f, 145.5f),	//14
		Vector3(96.5f, 0.f, 120.5f),	//15

		Vector3(86.5f, 0.f, 76.5f),		//16
		Vector3(54.5f, 0.f, 60.5f),		//17
		Vector3(5.5f, 0.f, 73.5f),		//18
		Vector3(26.5f, 0.f, 87.5f),		//19
		Vector3(38.5f, 0.f, 106.5f),	//20

		Vector3(70.5f, 0.f, 99.5f),		//21
		Vector3(68.5f, 0.f, 120.5f),	//22
	};

	// 1�� ���� �׸���� ���� �ڽ��ݶ��̴��� �¾Ƽ� �����
	for (auto& i : m_shopData)
	{
		i.m_position.x -= 1.f;
		i.m_position.z -= 1.f;
	}
}

void JiwonScene::Start()
{
	// �ùķ��̼� �⺻ on
	Scene::SetSimulating(true);

	// ī�޶� ��ġ �� - pitchRadian�� -1.22173 ������ �θ� ī�޶� ����� 70��
	m_cameraManager.lock()->GetMainCamera()->SetLookRadian(Vector2(0.037f, -1.22173f));
	m_cameraManager.lock()->GetMainCamera()->SetPosition(Vector3(-0.789f, 14.024f, -4.929f));

#pragma region Base
	auto gridEntity1 = std::make_shared<Grid>();
	Scene::AddEntity(gridEntity1);

	// ����Ʈ
	auto light = std::make_shared<Entity>("DLight");
	Scene::AddEntity(light);
	light->AddComponent<DirectionalLight>();

	//Plane(�ٴ�)
	auto RealPlane = std::make_shared<TextureBox>();
	RealPlane->SetName("RealPlane");
	Scene::AddEntity(RealPlane);
	RealPlane->AddComponent<PlaneCollider>();
#pragma endregion

	/// ���� �޴���
	auto mgr = std::make_shared<Entity>("GameManager");
	Scene::AddEntity(mgr);
	auto manager = mgr->AddComponent<GameManager>();

	// ���� �޴����� �ڽ����� �߰� -> �ð�
	auto timemgr = std::make_shared<Entity>("TimeUI");
	Scene::AddEntity(timemgr);
	timemgr->SetParent(mgr.get());
	timemgr->AddComponent<DirectionalLight>();
	timemgr->AddComponent<SunLight>();
	timemgr->AddComponent<TimerUI>();
	timemgr->AddComponent<TextRender>();
	timemgr->AddComponent<MeshFilter>();
	timemgr->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Timer);
	timemgr->AddComponent<UIRenderer>();

	/// �÷��̾� 
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

	//int(*grid)[150] = m_resourceManager.lock()->GetMapInfo();

	//// astar ���� �� ǥ��
	//for (int i = 0; i < 150; i++)
	//{
	//	for (int j = 0; j < 150; j++)
	//	{
	//		if (grid[i][j] == 1)
	//		{
	//			auto wall = std::make_shared<PinkBox>();
	//			wall->SetName("wall");
	//			wall->GetComponent<Transform>()->SetLocalPosition(Vector3(j, 0, i));
	//			AddEntity(wall);
	//		}
	//	}
	//}

	// ���Կ� ����
	for (auto i : m_shopData)
	{
		/// ���� ����
		auto store = std::make_shared<Entity>("SampleStore");
		auto storeRenderer = store->AddComponent<MeshRenderer>("StoreRenderer");
		auto storeMesh = store->AddComponent<MeshFilter>("SampleStore");
		auto storeTransform = store->GetComponent<Transform>();
		store->AddComponent<BoxCollider>();
		store->GetComponent<BoxCollider>()->SetSize(Vector3(7.0f, 7.0f, 7.0f));
		store->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);
		Scene::AddEntity(store);
		storeMesh->SetModelKey(GraphicsTransfer::ModelKey::SampleStore);

		///// ���� 
		//auto merchant = std::make_shared<Entity>("SampleMerchant");
		//merchant->AddComponent<ShopKeeperFSM>();
		//merchant->AddComponent<ShopKeeperDoubtScript>();
		//auto merchantRenderer = merchant->AddComponent<MeshRenderer>("MerchantRenderer");
		//auto merchantMesh = merchant->AddComponent<MeshFilter>("PlayerMesh");
		//Scene::AddEntity(merchant);
		//merchantMesh->SetModelKey(GraphicsTransfer::ModelKey::Player);
		//store->GetComponent<Transform>()->SetChild(merchant->GetComponent<Transform>()); // ���� ������ �ڽ����� ����

		/////������ ��ġ ����
		//int range = 10;
		////���� ray
		//merchant->AddComponent<RayCaster>();
		//merchant->GetComponent<RayCaster>(0)->SetUnitDirectionOffset(Vector3(0, 0, -1));
		//merchant->GetComponent<RayCaster>(0)->SetStartPosOffset(Vector3(0, 0, -1));
		//merchant->GetComponent<RayCaster>(0)->SetDistance(range);

		////magic number == tan 15
		////���� 15��
		//merchant->AddComponent<RayCaster>();
		//merchant->GetComponent<RayCaster>(1)->SetUnitDirectionOffset(Vector3(-0.26, 0, -1));
		//merchant->GetComponent<RayCaster>(1)->SetStartPosOffset(Vector3(0, 0, -1));
		//merchant->GetComponent<RayCaster>(1)->SetDistance(range);

		////������ 15��
		//merchant->AddComponent<RayCaster>();
		//merchant->GetComponent<RayCaster>(2)->SetUnitDirectionOffset(Vector3(0.26, 0, -1));
		//merchant->GetComponent<RayCaster>(2)->SetStartPosOffset(Vector3(0, 0, -1));
		//merchant->GetComponent<RayCaster>(2)->SetDistance(range);

		/////������ ȣ�����
		//auto callingRange = std::make_shared<Entity>("callingRange");
		//Scene::AddEntity(callingRange);
		//callingRange->AddComponent<Transform>();
		//callingRange->AddComponent<MeshRenderer>();
		//callingRange->AddComponent<SphereCollider>();
		//callingRange->GetComponent<SphereCollider>()->SetRadius(1.f);
		//callingRange->GetComponent<SphereCollider>()->SetDisableGravity(true);
		//callingRange->GetComponent<SphereCollider>()->SetIsTrigger(true);
		//callingRange->AddComponent<ShopKeeperCallingScript>();

		/////ȣ����� ���� ��ƼƼ�� ���̱�
		//merchant->GetComponent<Transform>()->SetChild(callingRange->GetComponent<Transform>());
		//callingRange->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 0));

		///// Ǫ�� ���̺�
		//auto table = std::make_shared<Entity>("SampleTable");
		//auto tableRenderer = table->AddComponent<MeshRenderer>("TableRenderer");
		//auto tableMesh = table->AddComponent<MeshFilter>("SampleTable");
		//table->AddComponent<SphereCollider>();
		//table->GetComponent<SphereCollider>()->SetRadius(3.f);
		//table->GetComponent<SphereCollider>()->SetDisableGravity(true);
		//table->GetComponent<SphereCollider>()->SetIsTrigger(true);
		//auto tableScript = table->AddComponent<FoodTableScript>(FoodType::Banana);
		//Scene::AddEntity(table);
		//tableMesh->SetModelKey(GraphicsTransfer::ModelKey::SampleTable);
		//store->GetComponent<Transform>()->SetChild(table->GetComponent<Transform>()); // ���� ������ �ڽ����� ����

		///// Ǫ��
		//auto food = std::make_shared<Entity>("SampleFood");
		//auto foodRenderer = food->AddComponent<MeshRenderer>("FoodRenderer");
		//auto foodMesh = food->AddComponent<MeshFilter>("SampleFood");
		//auto foodTransform = food->GetComponent<Transform>();

		//Scene::AddEntity(food);
		//foodMesh->SetModelKey(GraphicsTransfer::ModelKey::SampleFood);
		//table->GetComponent<Transform>()->SetChild(food->GetComponent<Transform>()); // ���� ���̺��� �ڽ����� ����

		///// interative key
		//auto interKey = std::make_shared<Entity>("InterKey");
		//auto interKeyRenderer = interKey->AddComponent<UIRenderer>("interKeyRenderer");
		//auto interKeyMesh = interKey->AddComponent<MeshFilter>("interKeyMesh");
		//auto interKeyScript = interKey->AddComponent<InterKeyScript>();
		////interKeyScript->m_script = table->GetComponent<FoodTableScript>();		// ������Ʈ �޾Ҵ��� m_script���ٰ���
		//Scene::AddEntity(interKey);
		//interKeyMesh->SetModelKey(GraphicsTransfer::ModelKey::QTE_InteractiveKey);
		//table->GetComponent<Transform>()->SetChild(interKey->GetComponent<Transform>()); // ���� ���̺��� �ڽ����� ����
		//auto interKeyTransform = interKey->GetComponent<Transform>();
		//interKeyTransform->SetLocalPosition(Vector3(0.f, 2.2f, 0.f));
		//interKeyTransform->SetLocalScale(Vector3(0.01f, 0.01f, 0.01f));

		// ��ġ ����
		storeTransform->AddPosition(i.m_position); // <- ���� ����

		float position;	// <- ���� ����
		if (i.m_direction == EntityDirection::DOWN) position = 0.f;
		else if (i.m_direction == EntityDirection::RIGHT) position = -90.f;
		else if (i.m_direction == EntityDirection::LEFT) position = 90.f;
		storeTransform->SetLocalRotation(Vector3(0.f, position, 0.f));

		//merchant->GetComponent<Transform>()->AddPosition(Vector3(-0.6f, 0.f, -2.1f)); // ���⼭���ʹ� ������� ��ġ
		//table->GetComponent<Transform>()->AddPosition(Vector3(-0.6f, 0.f, -3.f));
		//foodTransform->AddPosition(Vector3(0.f, 1.f, 0.f));
	}

	for (auto i : m_guardData)
	{
		//auto GuardRangeEntity = std::make_shared<Entity>("GuardRange");
		//Scene::AddEntity(GuardRangeEntity);

		//���
		auto GuardEntity = std::make_shared<GuardPrefab>();
		GuardEntity->GetComponent<Transform>()->SetWorldPosition(i.m_position);
		Scene::AddEntity(GuardEntity);

		/*GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(5, 0, 0));
		GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(5, 0, -22));
		GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(0, 0, -22));*/

		////��� ��������
		//auto returnGuard1 = std::make_shared<PinkBox>();
		//Scene::AddEntity(returnGuard1);

		//��������
		/*auto returnGuard2 = std::make_shared<YellowBox>();
		Scene::AddEntity(returnGuard2);

		auto returnGuard3 = std::make_shared<YellowBox>();
		Scene::AddEntity(returnGuard3);

		auto returnGuard4 = std::make_shared<YellowBox>();
		Scene::AddEntity(returnGuard4);*/

		//GuardRangeEntity->GetComponent<Transform>()->SetChild(returnGuard2->GetComponent<Transform>());
		//GuardRangeEntity->GetComponent<Transform>()->SetChild(returnGuard3->GetComponent<Transform>());
		//GuardRangeEntity->GetComponent<Transform>()->SetChild(returnGuard4->GetComponent<Transform>());

		GuardEntity->SetPatrolPath(GuardEntity->GetComponent<Transform>()->GetWorldPosition() + Vector3(0, 0, 0));

		float headDir;	// <- ���� ����
		if (i.m_direction == EntityDirection::DOWN) headDir = 0.f;
		else if (i.m_direction == EntityDirection::RIGHT) headDir = -90.f;
		else if (i.m_direction == EntityDirection::LEFT) headDir = 90.f;
		else if (i.m_direction == EntityDirection::UP) headDir = 180.f;
		//GuardRangeEntity->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, headDir, 0.f));

		//GuardEntity->SetPos(Vector3(0, 0, 0));
		//returnGuard1->GetComponent<Transform>()->SetLocalPosition(Vector3(0, -5.f, 0));
		//returnGuard2->GetComponent<Transform>()->SetLocalPosition(Vector3(0, -5.f, 0) + Vector3(5, 0, 0));
		//returnGuard3->GetComponent<Transform>()->SetLocalPosition(Vector3(0, -5.f, 0) + Vector3(5, 0, -22));
		//returnGuard4->GetComponent<Transform>()->SetLocalPosition(Vector3(0, -5.f, 0) + Vector3(0, 0, -22));
	}

	//auto returnGuard5 = std::make_shared<TextureBox>();
	//returnGuard5->SetName("asd");
	//Scene::AddEntity(returnGuard5);

	std::shared_ptr<Entity> minimap = std::make_shared<UIMinimap>();
	minimap->SetName("Minimap");
	minimap->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
	minimap->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	minimap->AddComponent<UITestScript>(UIType::MINIMAP);
	Scene::AddEntity(minimap);
	minimap->SetRender(false);

	auto minimapCurPos = std::make_shared<UIMinimapCurPos>();
	minimapCurPos->SetName("MinimapCurpos");
	//minimapCurPos->GetComponent<Transform>()->SetParent(player->GetComponent<Transform>());
	minimapCurPos->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
	minimapCurPos->GetComponent<Transform>()->SetWorldScale({ 0.2f, 0.2f ,1.f });
	minimapCurPos->AddComponent<UITestScript>(UIType::MINIMAPCURPOS);
	Scene::AddEntity(minimapCurPos);
	minimapCurPos->SetRender(false);

	// �÷��̾��� ��ġ�� �̴ϸ� ȭ��ǥ�� �������ֱ� ����.. ���� ��� �ڽ����� ���� ��ƼƼ ����
	auto dataSyncEntity = std::make_shared<Entity>("DataSyncEntity");
	dataSyncEntity->GetComponent<Transform>()->SetChild(player->GetComponent<Transform>());			//0�� �ڽ�
	dataSyncEntity->GetComponent<Transform>()->SetChild(minimapCurPos->GetComponent<Transform>());	//1�� �ڽ�
	dataSyncEntity->AddComponent<UIDataSyncScript>();
	dataSyncEntity->GetComponent<UIDataSyncScript>()->Start();
	Scene::AddEntity(dataSyncEntity);


	Scene::Start();
}

void JiwonScene::Update(double dt)
{
	Scene::Update(dt);

#pragma region 
	////���� �׽�Ʈ
	//if (InputManager::GetInstance()->IsKeyDown(VK_NUMPAD1))
	//{
	//	SoundManager::GetInstance()->PlayBGM("picnic.mp3"); // ������� �ٲٱ�
	//}

	//if (InputManager::GetInstance()->IsKeyDown(VK_NUMPAD2))
	//{
	//	SoundManager::GetInstance()->PlayBGM("seaway.mp3"); // ������� �ٲٱ�
	//}

	//if (InputManager::GetInstance()->IsKeyDown(VK_SPACE))
	//{
	//	SoundManager::GetInstance()->PlaySFX("swish.wav"); // ȿ���� ����
	//}

	//if (InputManager::GetInstance()->IsKeyDown('S'))
	//{
	//	SoundManager::GetInstance()->StopAllSounds(); // ���Ҹ� ����
	//}

	//if (InputManager::GetInstance()->IsKeyDown('P'))
	//{
	//	SoundManager::GetInstance()->PauseBGM(); // ������� �Ͻ�����
	//}

	//if (InputManager::GetInstance()->IsKeyDown('R'))
	//{
	//	SoundManager::GetInstance()->ResumeBGM(); // ������� �簳(resume)
	//}
#pragma endregion �����׽�Ʈ
}