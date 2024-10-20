#include "GameDemo_pch.h"
#include "ModelTest.h"

#include "MeshRenderer.h"
#include "Transform.h"

ModelTest::ModelTest()
	: Entity("ModelTestEntity")
{

}

ModelTest::~ModelTest()
{

}

void ModelTest::Start()
{
	// ������ �߰�
	auto pRenderer = AddComponent<MeshRenderer>("ModelRenderer");
	auto pMesh = AddComponent<MeshFilter>("SwabluMeshFilter");
	//pMesh->SetModelKey(L"Swablu");
	//pMesh->SetModelKey(L"SfBox");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::Erika);

	// ��ƼƼ Start ȣ��
	Entity::Start();
}
