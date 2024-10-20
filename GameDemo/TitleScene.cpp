#include "GameDemo_pch.h"
#include "TitleScene.h"

//engine
#include "IGraphics.h"

//manager
#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "SoundManager.h"

//entity
#include "Entity.h"

//component
#include "MainUI.h"
#include "UIRenderer.h"
#include "MeshFilter.h"
#include "GraphicsTransfer.h"

//script
#include "UITestScript.h"

//UI
#include "UIBlack.h"



TitleScene::TitleScene(std::wstring sceneName)
	: Scene(sceneName)
{

}

void TitleScene::Start()
{
	auto background = std::make_shared<Entity>("Background");
	Scene::AddEntity(background);
	background->AddComponent<MainUI>();
	background->AddComponent<MeshFilter>();
	background->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Main);
	background->AddComponent<UIRenderer>();

	SoundManager::GetInstance()->StopAllSounds();
	SoundManager::GetInstance()->PlayBGM("Title.mp3");



	Scene::Start();
}

void TitleScene::Update(double dt)
{
	Scene::Update(dt);

	// πË∞Ê¿Ω

}
