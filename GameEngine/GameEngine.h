#pragma once
#include "GameEngine.h"

class SceneManager;
class ResourceManager;
class IGraphics;
class CameraManager;

class SoundManager;
class InputManager;

//임시!!!------------------------
//assimp test
class MyModel;
class ShaderClass;
class SkyBox;

// 연하 테스트
class Grid;

class Scene;
class EventManager;


//임시!!!------------------------

/// <summary>
/// 2024.01.03
/// 각종 팩토리와 매니저급 클래스를 담고 있는 게임엔진 클래스
/// 작성 : 유승운
/// 수정 : 손서희 -> FixedUpdate 추가
/// </summary>
class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	GameEngine(const GameEngine& engine) = delete;
	GameEngine& operator=(const GameEngine& engine) = delete;

	GameEngine(const GameEngine&& engine) = delete;
	GameEngine&& operator=(const GameEngine&& engine) = delete;

	void Initialize(HWND hWnd, std::shared_ptr<IGraphics> graphics);
	void Start();
	void FixedUpdate(double dt);
	void Update(double dt);
	void LateUpdate(double dt);
	void Render(double dt);
	void Finalize();

	//void OnMouseDown(int x, int y);
	//void OnMouseUp(int x, int y);
	//void OnMouseMove(int button, int x, int y);
	//void MouseWheels(int x);

	//test
	void AddScene(std::string name,std::shared_ptr<Scene> scene);
	void SetCurScene(std::shared_ptr<Scene> scene);
	
	//Editor 전용 interface
	std::weak_ptr<SceneManager> GetSceneManager();
	std::weak_ptr<CameraManager> GetCameraManager();
	std::weak_ptr<ResourceManager> GetResourceManager();
	InputManager* GetInputManager();

	//리사이즈
	void Resize(int width, int height);

private:
	HWND m_hWnd;

	//매니저 클래스
	std::shared_ptr<SceneManager> m_sceneManager;
	std::shared_ptr<ResourceManager> m_resourceManager;
	std::shared_ptr<CameraManager> m_cameraManager;

	SoundManager* m_soundManager;
	InputManager* m_inputManager;
	EventManager* m_eventManager;
	CollisionManager* m_collisionManager;

	//그래픽스 엔진
	std::shared_ptr<IGraphics> m_graphicsEngine;

	//렌더 테스트
	std::shared_ptr<Grid> m_grid;

	//인풋매니저는 컴포넌트로 대체가 가능하지 않을까>????

	// 물리앤진 업데이트에 사용할 값.
	float m_fixedUpdateTimer;
	float m_simulationPeriod = 1.f / 144.f;  
	/// TODO::일반적으로 50~60사이의 값이 권장되나, 
	///보간이 필수로 구현되어 있어야 함으로 추후에 구현시 변경. 모니터 hz와 맞췄다.
};