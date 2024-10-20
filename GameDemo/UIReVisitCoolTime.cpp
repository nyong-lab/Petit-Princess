#include "GameDemo_pch.h"
#include "UIReVisitCoolTime.h"

#include "UIRenderer.h"

UIReVisitCoolTime::UIReVisitCoolTime()
	: Entity("UIEntity")
{

}

UIReVisitCoolTime::UIReVisitCoolTime(const std::shared_ptr<UIReVisitCoolTime>& other)
	: Entity(other)
{

}

UIReVisitCoolTime::~UIReVisitCoolTime()
{

}

void UIReVisitCoolTime::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_ReVisitCoolTime);


	// 엔티티 Start 호출
	Entity::Start();
}
