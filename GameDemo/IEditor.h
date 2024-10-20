#pragma once

#include <memory>
#include <any>
#include <windows.h>

class IGraphics;
class IDevice;
class SceneManager;
class CameraManager;
class InputManager;
class ResourceManager;
class Timer;

/// <summary>
/// Editor의 인터페이스 클래스 
/// 작성자 : 김동일
/// </summary>
class IEditor
{
public:
	virtual ~IEditor() {}

	virtual void Initialize(HWND hWnd, std::weak_ptr<IGraphics> graphics, std::weak_ptr<SceneManager> sceneManager, std::weak_ptr<CameraManager> cameraManager, std::weak_ptr<ResourceManager> gameResourceManager, std::weak_ptr<Timer> timer) abstract;
	virtual void NewFrame() abstract;
	virtual void Update(double dt) abstract;
	virtual void Render() abstract;
	virtual void Finalize() abstract;

};