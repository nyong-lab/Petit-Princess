#include "GameDemo_pch.h"
#include "UIBlack.h"
#include "UIRenderer.h"

UIBlack::UIBlack()
	: Entity("UIEntity")
{

}

UIBlack::UIBlack(const std::shared_ptr<UIBlack>& other)
	: Entity(other)
{

}

UIBlack::~UIBlack()
{

}

void UIBlack::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_Black);

	// ��ƼƼ Start ȣ��
	Entity::Start();
}

