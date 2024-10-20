#include "GameDemo_pch.h"
#include "UI.h"
#include "UIRenderer.h"
#include "MeshFilter.h"

UI::UI()
	: Entity("UIEntity")
{

}

UI::UI(const std::shared_ptr<UI>& other)
	: Entity(other)
{

}

UI::~UI()
{

}

void UI::Start()
{
	/*m_camera.lock()->SetProjectionMode(ProjectionMode::Orthographic);*/
	//m_camera.lock()->SetPosition(Vector3(0,0,0));
	
	// 렌더러 추가
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI);

	// 엔티티 Start 호출
	Entity::Start();

}
