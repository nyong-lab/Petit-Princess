#include "GameDemo_pch.h"
#include "UISense.h"

#include "UIRenderer.h"

UISense::UISense()
	: Entity("UIEntity")
{

}

UISense::UISense(const std::shared_ptr<UISense>& other)
	: Entity(other)
{

}

UISense::~UISense()
{

}

void UISense::Start()
{
	auto pRenderer = AddComponent<UIRenderer>("UIRenderer");
	auto pMesh = AddComponent<MeshFilter>("UIMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::UI_Sense);


	// ��ƼƼ Start ȣ��
	Entity::Start();
}
