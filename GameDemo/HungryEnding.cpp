#include "GameDemo_pch.h"
#include "HungryEnding.h"

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

#include "UIBlack.h"
#include "UITestScript.h"

//component
#include "MainUI.h"
#include "UIRenderer.h"
#include "MeshFilter.h"
#include "GraphicsTransfer.h"
HungryEnding::HungryEnding(std::wstring sceneName)
{

}

void HungryEnding::Start()
{
	auto background = std::make_shared<Entity>("Background");
	Scene::AddEntity(background);
	background->AddComponent<MainUI>();
	background->AddComponent<MeshFilter>();
	background->GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::UI_Main);
	background->AddComponent<UIRenderer>();

	SoundManager::GetInstance()->StopAllSounds();
	SoundManager::GetInstance()->PlayBGM("GameOver.mp3");

	/// fadeIN/OUT   ------------ 力啊 力老 第俊
	auto BlackUI = std::make_shared<UIBlack>();
	Scene::AddEntity(BlackUI);
	BlackUI->GetComponent<Transform>()->SetWorldPosition({ 0,0,0 });
	BlackUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	BlackUI->AddComponent<UITestScript>(UIType::FADE);

	Scene::Start();
}

void HungryEnding::Update(double dt)
{
	Scene::Update(dt);
}
