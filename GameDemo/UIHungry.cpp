#include "GameDemo_pch.h"
#include "UIHungry.h"
#include "UIRenderer.h"

UIHungry::UIHungry()
	: Entity("UIEntity")
{

}

UIHungry::UIHungry(const std::shared_ptr<UIHungry>& other)
	: Entity(other)
{

}

UIHungry::~UIHungry()
{

}

void UIHungry::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UIHungry);

	// 엔티티 Start 호출
	Entity::Start();
}
