#include "GameDemo_pch.h"
#include "UISkill.h"
#include "UIRenderer.h"

UISkill::UISkill()
	: Entity("UIEntity")
{

}

UISkill::UISkill(const std::shared_ptr<UISkill>& other)
	: Entity(other)
{

}

UISkill::~UISkill()
{

}

void UISkill::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_Skill);

	// 엔티티 Start 호출
	Entity::Start();
}