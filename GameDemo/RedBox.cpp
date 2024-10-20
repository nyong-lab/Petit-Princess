#include "GameDemo_pch.h"
#include "RedBox.h"
#include "MeshRenderer.h"

RedBox::RedBox()
	: Entity("RedBoxEntity")
{
}

RedBox::RedBox(const std::shared_ptr<RedBox>& other)
	: Entity(other)
{

}

RedBox::~RedBox()
{

}

void RedBox::Start()
{
	// 렌더러 추가
	auto pRenderer = AddComponent<MeshRenderer>("RedBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("RedBoxMeshFilter");

	// L"YellowBox", L"PinkBox" 모델 키만 바꿔서 프리팹 생성해서 쓰면됩니당
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::RedBox);

	// 엔티티 Start 호출
	Entity::Start();
}



PinkBox::PinkBox()
	: Entity("PinkBoxEntity")
{
}

PinkBox::PinkBox(const std::shared_ptr<PinkBox>& other)
	: Entity(other)
{
	
}

PinkBox::~PinkBox()
{

}

void PinkBox::Start()
{
	// 렌더러 추가
	auto pRenderer = AddComponent<MeshRenderer>("PinkBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("PinkBoxMeshFilter");

	// L"YellowBox", L"PinkBox" 모델 키만 바꿔서 프리팹 생성해서 쓰면됩니당
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::PinkBox);

	// 엔티티 Start 호출
	Entity::Start();
}


YellowBox::YellowBox()
	: Entity("YellowBoxEntity")
{
}

YellowBox::YellowBox(const std::shared_ptr<YellowBox>& other)
	: Entity(other)
{
}

YellowBox::~YellowBox()
{

}

void YellowBox::Start()
{
	// 렌더러 추가
	auto pRenderer = AddComponent<MeshRenderer>("YellowBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("YellowBoxMeshFilter");

	// L"YellowBox", L"PinkBox" 모델 키만 바꿔서 프리팹 생성해서 쓰면됩니당
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::YellowBox);

	// 엔티티 Start 호출
	Entity::Start();
}