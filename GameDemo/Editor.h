#pragma once

#include "IEditor.h"

#include <vector>
#include <memory>
#include <string>

class Entity;
class DockingGui;
class HierarchyGui;
class InspectorGui;
class ProjectGui;
class WorldGui;
class SeveralGui;

class IGraphics;
class Timer;
class CameraManager;
class ResourceManager;

class Timer;

/// <summary>
/// ImGui의 기능들을 담당하고, Gui를 통하여 맵을 조작할 수 있는 클래스
/// 작성자 : 김동일
/// </summary>
class Editor : public IEditor
{
public:
	Editor();
	virtual ~Editor() final;

	virtual void Initialize(HWND hWnd, std::weak_ptr<IGraphics> graphics, std::weak_ptr<SceneManager> sceneManager, std::weak_ptr<CameraManager> cameraManager, std::weak_ptr<ResourceManager> gameResourceManager, std::weak_ptr<Timer> timer) final;
	virtual void NewFrame() final;
	virtual void Update(double dt) final;
	virtual void Render() final;
	virtual void Finalize() final;

	void Theme_Blender(ImGuiStyle* dst = NULL);
	void Theme_Cyberpunk(ImGuiStyle* dst = NULL);
	void Theme_Nord(ImGuiStyle* dst = NULL);
	void Theme_MoonLight(ImGuiStyle* dst = NULL);

private:
	std::shared_ptr<WorldGui> m_worldGui;
	std::unique_ptr<DockingGui> m_dockingGui;
	std::unique_ptr<HierarchyGui> m_hierarchyGui;
	std::shared_ptr<InspectorGui> m_inspectorGui;
	std::shared_ptr<ProjectGui> m_projectGui;
	std::unique_ptr<SeveralGui> m_severalGui;

	std::weak_ptr<IGraphics> m_graphics;
	std::weak_ptr<Timer> m_timer;
	std::weak_ptr<SceneManager> m_sceneManager;
	std::weak_ptr<CameraManager> m_cameraManager;
	std::weak_ptr<ResourceManager> m_gameResourceManager;

	std::weak_ptr<Entity> m_selectEntity;
	std::weak_ptr<Entity> m_prevEntity;
};