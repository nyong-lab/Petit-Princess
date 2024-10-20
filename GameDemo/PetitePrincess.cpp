#include "GameDemo_pch.h"
#include "PetitePrincess.h"

// Engine
#include "GameEngine.h"
#include "IGraphics.h"

// Scene
#include "DongilScene.h"
#include "SunWoongScene.h"
#include "YeonhaScene.h"
#include "AstarScene.h"
#include "SeoheeScene.h"
#include "JiwonScene.h"
#include "TitleScene.h"
#include "STAGE1.h"
#include "HungryEnding.h"
#include "HappyEnding.h"

// Entity
#include "Entity.h"

// component
#include "BoxCollider.h"
#include "MeshRenderer.h"

// Manager
#include "SceneManager.h"
#include "EventManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "PathManager.h"


void PetitePrincess::Initialize(std::unique_ptr<GameEngine>& gameEngine, std::shared_ptr<IGraphics> graphics)
{
	///--------------------Main---------------------
	/// 씬 폴더에 들어있는 모든 데이터들을 파싱해서 씬매니저에 등록한다.
	/// 씬 폴더에 존재하는 파일 이름과 동일한 이름으로 하드코딩해서 AddScene 하면
	/// 기존 씬 위에 데이터 덮어 씌워지니 참고
	/*for (const auto& entry : std::filesystem::directory_iterator(PathManager::GetSceneDataPath()))
	{
		const auto& path = entry.path();
		std::wstring sceneName = path.filename().wstring();
		gameEngine->GetSceneManager().lock()->LoadScene(sceneName, true);
	}
	std::shared_ptr<Scene> scene = gameEngine->GetSceneManager().lock()->GetScene("RealTitle");
	gameEngine->GetSceneManager().lock()->SetCurScene(scene);
	EventManager::GetInstance()->SetCurScene(scene);*/

	 //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	///-------------------SunWoong Scene--------------------
	std::shared_ptr<SunWoongScene> sunWoongScene = std::make_shared<SunWoongScene>(L"SunWoongScene");
	sunWoongScene->Initialize(graphics, 3,
		gameEngine->GetCameraManager().lock(),
		gameEngine->GetResourceManager().lock(),
		gameEngine->GetSceneManager().lock());
 	gameEngine->AddScene("SunWoongScene", sunWoongScene);

	///----------------Jiwon Scene--------------------
	std::shared_ptr<JiwonScene> jiwonScene = std::make_shared<JiwonScene>(std::wstring(L"jiwonScene"));
	jiwonScene->Initialize(graphics, 3,
		gameEngine->GetCameraManager().lock(),
		gameEngine->GetResourceManager().lock(),
		gameEngine->GetSceneManager().lock());
	gameEngine->AddScene("jiwonScene", jiwonScene);

	///----------------Title Scene--------------------
	std::shared_ptr<TitleScene> title = std::make_shared<TitleScene>(std::wstring(L"Title"));
	title->Initialize(graphics, 3,
		gameEngine->GetCameraManager().lock(),
		gameEngine->GetResourceManager().lock(),
		gameEngine->GetSceneManager().lock());
	gameEngine->AddScene("Title", title);

	///----------------Stage1 Scene--------------------
	std::shared_ptr<STAGE1> stage1 = std::make_shared<STAGE1>(std::wstring(L"STAGE1"));
	stage1->Initialize(graphics, 3,
		gameEngine->GetCameraManager().lock(),
		gameEngine->GetResourceManager().lock(),
		gameEngine->GetSceneManager().lock());
	gameEngine->AddScene("STAGE1", stage1);

	///----------------Ending Scene--------------------
	std::shared_ptr<HungryEnding> hungry = std::make_shared<HungryEnding>(std::wstring(L"Hungry"));
	hungry->Initialize(graphics, 3,
		gameEngine->GetCameraManager().lock(),
		gameEngine->GetResourceManager().lock(),
		gameEngine->GetSceneManager().lock());
	gameEngine->AddScene("Hungry", hungry);

	///----------------Ending Scene(Happy)--------------------
	std::shared_ptr<HappyEnding> happy = std::make_shared<HappyEnding>(std::wstring(L"Happy"));
	happy->Initialize(graphics, 3,
		gameEngine->GetCameraManager().lock(),
		gameEngine->GetResourceManager().lock(),
		gameEngine->GetSceneManager().lock());
	gameEngine->AddScene("Happy", happy);

	/// 씬 지정
	//gameEngine->SetCurScene(sunWoongScene);
	//gameEngine->SetCurScene(jiwonScene);
	//gameEngine->SetCurScene(stage1);
	gameEngine->SetCurScene(title);
	//gameEngine->SetCurScene(hungry);
	//gameEngine->SetCurScene(happy);

	//EventManager::GetInstance()->SetCurScene(sunWoongScene);
	//EventManager::GetInstance()->SetCurScene(jiwonScene);
	//EventManager::GetInstance()->SetCurScene(stage1);
	EventManager::GetInstance()->SetCurScene(title);
	//EventManager::GetInstance()->SetCurScene(hungry);
	//EventManager::GetInstance()->SetCurScene(happy);
}

