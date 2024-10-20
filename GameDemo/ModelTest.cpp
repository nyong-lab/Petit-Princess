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
	// 렌더러 추가
	auto pRenderer = AddComponent<MeshRenderer>("ModelRenderer");
	auto pMesh = AddComponent<MeshFilter>("SwabluMeshFilter");
	//pMesh->SetModelKey(L"Swablu");
	//pMesh->SetModelKey(L"SfBox");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::Erika);

	// 엔티티 Start 호출
	Entity::Start();
}
