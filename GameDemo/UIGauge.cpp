#include "GameDemo_pch.h"
#include "UIGauge.h"
#include "UIRenderer.h"
#include "TextRender.h"

UIGauge::UIGauge()
	: Entity("UIEntity")
{

}

UIGauge::UIGauge(const std::shared_ptr<UIGauge>& other)
	: Entity(other)
{

}

UIGauge::~UIGauge()
{

}

void UIGauge::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UIGauge);

	// 엔티티 Start 호출
	Entity::Start();
}

