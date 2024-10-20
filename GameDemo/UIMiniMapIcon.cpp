#include "GameDemo_pch.h"
#include "UIMiniMapIcon.h"
#include "UIRenderer.h"
#include "MeshFilter.h"

UIMiniMapIcon::UIMiniMapIcon()
	: Entity("UIEntity")
{

}

UIMiniMapIcon::UIMiniMapIcon(const std::shared_ptr<UIMiniMapIcon>& other)
	: Entity(other)
{

}

UIMiniMapIcon::~UIMiniMapIcon()
{

}

void UIMiniMapIcon::Start()
{
	/*m_camera.lock()->SetProjectionMode(ProjectionMode::Orthographic);*/
	//m_camera.lock()->SetPosition(Vector3(0,0,0));

	// 렌더러 추가
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_MiniMapIcon);

	// 엔티티 Start 호출
	Entity::Start();

}
