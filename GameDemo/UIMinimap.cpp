#include "GameDemo_pch.h"
#include "UIMinimap.h"

#include "UIRenderer.h"

UIMinimap::UIMinimap()
	: Entity("UIMinimap")
{

}

UIMinimap::UIMinimap(const std::shared_ptr<UIMinimap>& other)
	: Entity(other)
{

}

UIMinimap::~UIMinimap()
{

}

void UIMinimap::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_Minimap);


	// 엔티티 Start 호출
	Entity::Start();
}
