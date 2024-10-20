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
	// ������ �߰�
	auto pRenderer = AddComponent<MeshRenderer>("RedBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("RedBoxMeshFilter");

	// L"YellowBox", L"PinkBox" �� Ű�� �ٲ㼭 ������ �����ؼ� ����˴ϴ�
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::RedBox);

	// ��ƼƼ Start ȣ��
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
	// ������ �߰�
	auto pRenderer = AddComponent<MeshRenderer>("PinkBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("PinkBoxMeshFilter");

	// L"YellowBox", L"PinkBox" �� Ű�� �ٲ㼭 ������ �����ؼ� ����˴ϴ�
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::PinkBox);

	// ��ƼƼ Start ȣ��
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
	// ������ �߰�
	auto pRenderer = AddComponent<MeshRenderer>("YellowBoxRenderer");
	auto pMesh = AddComponent<MeshFilter>("YellowBoxMeshFilter");

	// L"YellowBox", L"PinkBox" �� Ű�� �ٲ㼭 ������ �����ؼ� ����˴ϴ�
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::YellowBox);

	// ��ƼƼ Start ȣ��
	Entity::Start();
}