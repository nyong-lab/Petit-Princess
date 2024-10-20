#include "GameDemo_pch.h"
#include "UIDeliciousFood.h"

#include "UIRenderer.h"

UIDeliciousFood::UIDeliciousFood()
	: Entity("UIEntity")
{

}

UIDeliciousFood::UIDeliciousFood(const std::shared_ptr<UIDeliciousFood>& other)
	: Entity(other)
{

}

UIDeliciousFood::~UIDeliciousFood()
{

}

void UIDeliciousFood::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_DeliciousFood);


	// 엔티티 Start 호출
	Entity::Start();
}
