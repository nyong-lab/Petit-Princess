#include "GameDemo_pch.h"
#include "YeonhaScene.h"

#include "IGraphics.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"

#include "Grid.h"
#include "CubeMap.h"
#include "TextureBox.h"
#include "UI.h"
#include "ModelTest.h"
#include "RedBox.h"

#include "BoxCollider.h"
#include "PlaneCollider.h"
#include "MeshFilter.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include "CameraModule.h"

YeonhaScene::YeonhaScene(std::wstring scenename) : Scene(scenename)
{

}

void YeonhaScene::Start()
{
	// �׸���
	auto gridEntity = std::make_shared<Grid>();
	Scene::AddEntity(gridEntity);

	// ��
	auto EricaEntity = std::make_shared<ModelTest>();
	Scene::AddEntity(EricaEntity);
	EricaEntity->GetComponent<Transform>()->SetLocalScale(Vector3(0.03f, 0.03f, 0.03f));
	EricaEntity->GetComponent<Transform>()->SetLocalPosition(Vector3(3.f, 0.03f, 0.03f));
	
	/// -----------------------------------------------------------------------------------------------------------

	// ����Ʈ
	auto light = std::make_shared<Entity>("DLight");
	Scene::AddEntity(light);
	light->AddComponent<DirectionalLight>();

	// ����Ʈ
	auto plight = std::make_shared<Entity>("PLight");
	Scene::AddEntity(plight);
	plight->AddComponent<PointLight>();
	Scene::Start();
}

void YeonhaScene::Update(double dt)
{
	Scene::Update(dt);

	///----------�̺�Ʈ �Ŵ��� Ȯ�ο�------------
	if (InputManager::GetInstance()->IsKeyDown(VK_BACK))
	{
		//if (m_sceneManager.lock()->GetCurScene() != shared_from_this())
		{
			EventManager::GetInstance()->SceneChange(m_sceneManager.lock()->GetScene("ShScene"));
		}
	}

}
