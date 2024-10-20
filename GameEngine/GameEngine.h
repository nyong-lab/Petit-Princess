#pragma once
#include "GameEngine.h"

class SceneManager;
class ResourceManager;
class IGraphics;
class CameraManager;

class SoundManager;
class InputManager;

//�ӽ�!!!------------------------
//assimp test
class MyModel;
class ShaderClass;
class SkyBox;

// ���� �׽�Ʈ
class Grid;

class Scene;
class EventManager;


//�ӽ�!!!------------------------

/// <summary>
/// 2024.01.03
/// ���� ���丮�� �Ŵ����� Ŭ������ ��� �ִ� ���ӿ��� Ŭ����
/// �ۼ� : ���¿�
/// ���� : �ռ��� -> FixedUpdate �߰�
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
	
	//Editor ���� interface
	std::weak_ptr<SceneManager> GetSceneManager();
	std::weak_ptr<CameraManager> GetCameraManager();
	std::weak_ptr<ResourceManager> GetResourceManager();
	InputManager* GetInputManager();

	//��������
	void Resize(int width, int height);

private:
	HWND m_hWnd;

	//�Ŵ��� Ŭ����
	std::shared_ptr<SceneManager> m_sceneManager;
	std::shared_ptr<ResourceManager> m_resourceManager;
	std::shared_ptr<CameraManager> m_cameraManager;

	SoundManager* m_soundManager;
	InputManager* m_inputManager;
	EventManager* m_eventManager;
	CollisionManager* m_collisionManager;

	//�׷��Ƚ� ����
	std::shared_ptr<IGraphics> m_graphicsEngine;

	//���� �׽�Ʈ
	std::shared_ptr<Grid> m_grid;

	//��ǲ�Ŵ����� ������Ʈ�� ��ü�� �������� ������>????

	// �������� ������Ʈ�� ����� ��.
	float m_fixedUpdateTimer;
	float m_simulationPeriod = 1.f / 144.f;  
	/// TODO::�Ϲ������� 50~60������ ���� ����ǳ�, 
	///������ �ʼ��� �����Ǿ� �־�� ������ ���Ŀ� ������ ����. ����� hz�� �����.
};