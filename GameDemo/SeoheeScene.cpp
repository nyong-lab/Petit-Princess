#include "GameDemo_pch.h"
#include "SeoheeScene.h"

#include "IGraphics.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"

#include "Grid.h"
#include "CubeMap.h"
#include "TextureBox.h"
#include "UI.h"
#include "ModelTest.h"

#include "BoxCollider.h"
#include "PlaneCollider.h"
#include "Transform.h"
#include "RayCaster.h"
#include "DirectionalLight.h"
#include "PetitCamera.h"
#include "ShTest.h"
#include "CapsuleCollider.h"

#include "DongilScene.h"

SeoheeScene::SeoheeScene(std::wstring scenename) : Scene(scenename)
{

}

void SeoheeScene::Start()
{
	// �ٴ�
	auto gridEntity = std::make_shared<Grid>();
	gridEntity->SetName("RealPlane");
	Scene::AddEntity(gridEntity);
	gridEntity->AddComponent<PlaneCollider>();

	//�ٴ���ô�ϴ� ����
// 	auto Plane = std::make_shared<TextureBox>(m_graphics, m_cameraManager.lock()->GetMainCamera());
// 	Plane->SetName("Plane");
// 	Scene::AddEntity(Plane);
// 	Plane->AddComponent<BoxCollider>();
// 	Plane->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.25f, 0.f));
// 	Plane->GetComponent<Transform>()->SetLocalScale(Vector3(10.f, 0.5f, 10.f));
// 	Plane->GetComponent<BoxCollider>()->SetSize(Vector3(10.f, 0.5f, 10.f));
// 	Plane->GetComponent<BoxCollider>()->SetBodyType(BodyType::Static);

	//ť��
	auto cube = std::make_shared<TextureBox>();
	Scene::AddEntity(cube);
	cube->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 5, 0));
	cube->AddComponent<BoxCollider>();

	//�÷��̾�
	auto cube2 = std::make_shared<TextureBox>();
	Scene::AddEntity(cube2);
	cube2->SetName("player");
	cube2->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 3, 0));
	cube2->AddComponent<CapsuleCollider>();
	cube2->GetComponent<CapsuleCollider>()->FreezeRotation(true, true, true);
	cube2->AddComponent<PetitCamera>();
	cube2->AddComponent<ShTest>();
	cube2->GetComponent<PetitCamera>()->SetTarget(cube2);


	// ����Ʈ
	auto light = std::make_shared<Entity>("DLight");
	Scene::AddEntity(light);
	light->AddComponent<DirectionalLight>();

	Scene::Start();
}

void SeoheeScene::Update(double dt)
{
	Scene::Update(dt);

	///----------�̺�Ʈ �Ŵ��� Ȯ�ο�------------
	if (InputManager::GetInstance()->IsKeyDown(VK_BACK))
	{
		EventManager::GetInstance()->SceneChange(m_sceneManager.lock()->GetScene("TestScene"));
	}

	if (InputManager::GetInstance()->IsKeyDown('M'))
	{
		CollisionManager::GetInstance()->RaycastMultiple(Vector3(0, -1, 0), Vector3(0, 1, 0), 10);
	}

	if (InputManager::GetInstance()->IsKeyDown('C'))
	{
		CollisionManager::GetInstance()->RaycastClosest(Vector3(0, -1, 0), Vector3(0, 1, 0), 10);
	}
}
