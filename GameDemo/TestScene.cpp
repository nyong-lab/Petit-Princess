#include "GameDemo_pch.h"
#include "TestScene.h"

#include "Entity.h"
#include "Grid.h"
#include "TextureBox.h"
#include "CubeMap.h"
#include "ModelTest.h"
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

TestScene::TestScene(std::wstring sceneName)
	: Scene(sceneName)
{

}



void TestScene::Start()
{
	{
		/// 동일 테스트 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		auto reflectionEntity = std::make_shared<Entity>("ReflectionEntity");
		Scene::AddEntity(reflectionEntity);
		reflectionEntity->AddComponent<PlaneCollider>();
		reflectionEntity->AddComponent<MeshRenderer>();
		reflectionEntity->AddComponent<TestingScript>();

	}

	{
		/// 연하 테스트 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		auto gridEntity = std::make_shared<Grid>();
		Scene::AddEntity(gridEntity);

	}

	{
		/// 서희 테스트 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
		auto Plane = std::make_shared<TextureBox>();
		Plane->SetName("Plane");
		Scene::AddEntity(Plane);
		Plane->AddComponent<BoxCollider>();
		Plane->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.25f, 0.f));
		Plane->GetComponent<Transform>()->SetLocalScale(Vector3(10.f, 0.5f, 10.f));
		Plane->GetComponent<BoxCollider>()->SetMass(100);
		Plane->GetComponent<BoxCollider>()->SetDisableGravity(true);
		Plane->GetComponent<BoxCollider>()->FreezePosition(true, true, true);
		Plane->GetComponent<BoxCollider>()->FreezeRotation(true, true, true);
		Plane->GetComponent<BoxCollider>()->SetSize(Vector3(10.f, 0.5f, 10.f));

		// 	auto TextureBoxEntity2 = std::make_shared<TextureBox>(graphics, camera);
		// 	TextureBoxEntity2->AddComponent<BoxCollider>(collisionMgr);
		// 	TextureBoxEntity2->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 10.f, 0.f));
		// 	Scene::AddEntity(TextureBoxEntity2);
		for (int i = 0; i < 10; i++) {
			auto TextureBoxEntity3 = std::make_shared<TextureBox>();
			Scene::AddEntity(TextureBoxEntity3);
			TextureBoxEntity3->AddComponent<BoxCollider>();
			TextureBoxEntity3->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 17.5f + 1.5 * (float)i, 0.f));
			std::shared_ptr<Transform> tr = TextureBoxEntity3->GetComponent<Transform>();
			//TextureBoxEntity3->GetComponent<BoxCollider>()->FreezePosition(true, true, true);
			//TextureBoxEntity3->GetComponent<BoxCollider>()->FreezeRotation(true, true, true);
		}

		auto RealPlane = std::make_shared<TextureBox>();
		RealPlane->SetName("RealPlane");
		Scene::AddEntity(RealPlane);
		RealPlane->AddComponent<PlaneCollider>();
	}

	Scene::Start();
}


void TestScene::Update(double dt)
{
	Scene::Update(dt);

	///----------이벤트 매니저 확인용------------
	if (InputManager::GetInstance()->IsKeyDown(VK_BACK))
	{
		//if (m_sceneManager.lock()->GetCurScene() != shared_from_this())
		{
			EventManager::GetInstance()->SceneChange(m_sceneManager.lock()->GetScene("DongilScene"));
		}
	}
}