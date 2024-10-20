#pragma once
#include "GameEngine_pch.h"
#include "GameEngine.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "IGraphics.h"
#include "CameraManager.h"
#include "CollisionManager.h"

//싱글톤
#include "SoundManager.h"
#include "InputManager.h"
#include "EventManager.h"

//임시!!!-----------------
//entity factory 테스트용도
#include "EntityFactory.h"
#include "TransformBulider.h"
#include "BoxColiderBulider.h"

// 리플렉션 테스트 용도
#include "MeshRenderer.h"
#include "BoxCollider.h"
//------------------------

GameEngine::GameEngine() 
	: m_sceneManager(nullptr), m_resourceManager(nullptr), m_soundManager(nullptr),m_inputManager(nullptr), m_eventManager(nullptr)
	, m_hWnd(nullptr)
{

}

GameEngine::~GameEngine()
{

}

void GameEngine::Initialize(HWND hWnd, std::shared_ptr<IGraphics> graphics)
{
	m_hWnd = hWnd;
	m_graphicsEngine = graphics;

	m_sceneManager = std::make_shared<SceneManager>();
	m_resourceManager = std::make_shared<ResourceManager>();
	m_soundManager = SoundManager::GetInstance();
	m_inputManager = InputManager::GetInstance();
	m_cameraManager = std::make_shared<CameraManager>();
	m_collisionManager = CollisionManager::GetInstance();

	m_resourceManager->Initialize();
	m_collisionManager->Initialize();

	m_sceneManager->Initialize();
	m_sceneManager->InitializeManager(m_graphicsEngine, 2, m_cameraManager, m_resourceManager);

	m_soundManager->Initialize(m_resourceManager);
	m_inputManager->Initialize(hWnd);
	m_cameraManager->Initialize(hWnd);

	m_eventManager = EventManager::GetInstance();
}

void GameEngine::Start()
{
	m_sceneManager->Start();
}

void GameEngine::FixedUpdate(double dt)
{
	
}

void GameEngine::Render(double dt)
{
	m_sceneManager->Render(dt);
	m_sceneManager->DebugRender();
}

void GameEngine::Update(double dt)
{
	//IManger상속 -> 나중에 묶어서 Update
	m_resourceManager->Update(dt);
	m_cameraManager->Update(dt);
	m_sceneManager->Update(dt);

	//싱글톤
	m_soundManager->Update(dt);
	m_inputManager->Update();

	// fixedUpdate ------------------------------------------
	m_fixedUpdateTimer += dt;
	while (m_fixedUpdateTimer >= m_simulationPeriod)
	{
		m_collisionManager->FixedUpdate(m_simulationPeriod);
		m_sceneManager->FixedUpdate(m_simulationPeriod);

		/* Collider 관련 업데이트 루프---- 업데이트 순서가 보장되어야 함!!
		
		1. 유저스크립트를 포함한 각종 컴포넌트들이 Transform 변경 ...sceneManager Update
		2. 최종 Transform을 physx 월드로 전달
		3. 물리 시뮬레이션 ... collisionManager fixedUpdate
		4. Enter, Exit 콜백 전달
		5. 시뮬레이션 결과를 트렌스폼으로 전달
		6. Stay 콜백 전달 ...sceneManager fixedUpdate
		
		*/
		m_fixedUpdateTimer -= m_simulationPeriod;
	}

	// -----------------------------------------------------

	m_eventManager->Update( );
}

void GameEngine::LateUpdate(double dt)
{

}

void GameEngine::Finalize()
{
	//IManger상속 -> 나중에 묶어서 
	m_sceneManager->Finalize();
	m_resourceManager->Finalize();
	m_cameraManager->Finalize();

	//싱글톤
	SoundManager::ReleaseInstance();
	InputManager::ReleaseInstance();
	EventManager::ReleaseInstance();
	CollisionManager::GetInstance()->Finalize();
	CollisionManager::ReleaseInstance();
}

//void GameEngine::OnMouseDown(int x, int y)
//{
//	m_cameraManager->SetPreMousePos(x, y);
//	m_cameraManager->SetCurMousePos(x, y);
//	SetCapture(m_hWnd);
//}
//
//void GameEngine::OnMouseUp(int x, int y)
//{
//	ReleaseCapture();
//}
//
//void GameEngine::OnMouseMove(int button,int x, int y)
//{
//	if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEYSTATE::Press 
//		|| InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEYSTATE::Down)
//	{
//		m_cameraManager->SetCurMousePos(x, y);
//	}
//}
//
//void GameEngine::MouseWheels(int x)
//{
//
//}

void GameEngine::AddScene(std::string name, std::shared_ptr<Scene> scene)
{
	m_sceneManager->AddScene(name, scene);
}

void GameEngine::SetCurScene(std::shared_ptr<Scene> scene)
{
	m_sceneManager->SetCurScene(scene);
}

std::weak_ptr<SceneManager> GameEngine::GetSceneManager()
{
	return m_sceneManager;
}

std::weak_ptr<CameraManager> GameEngine::GetCameraManager()
{
	return m_cameraManager;
}

std::weak_ptr<ResourceManager> GameEngine::GetResourceManager()
{
	return m_resourceManager;
}

InputManager* GameEngine::GetInputManager()
{
	return m_inputManager;
}

void GameEngine::Resize(int width, int height)
{
	m_cameraManager->Resize(width, height);
}
