#pragma once

#include "BaseGui.h"


class CameraManager;
class CameraModule;
class SceneManager;

class TestGui :  public BaseGui
{
public:
	TestGui();
	~TestGui() {}

	void Initialize(std::weak_ptr<CameraManager> cameraManager, std::weak_ptr<SceneManager> sceneManager);
	void Update();

private:
	void ShowTest();
	void ShowDebug();
	void ShowCamera();
	void ReadPropertyInformation(const Property* property);
	void ReadPrimitiveType(const Property* property);
	void ReadEnumClassType(const Property* property);


private:
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<CameraModule> m_mainCamera;
	std::unordered_map<std::string, std::vector<std::string>> m_enumClassStringList;
};

