#include "GameDemo_pch.h"
#include "CubeMap.h"

#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Transform.h"

CubeMap::CubeMap()
	: Entity("CubeMapEntity")
{

}

CubeMap::~CubeMap()
{

}

void CubeMap::Start()
{
	// 메시 렌더러, 메시 필터
	auto pRenderer = AddComponent<MeshRenderer>("CubeMapRenderer");
	auto pMesh = AddComponent<MeshFilter>("CubeMapMeshFilter");
	//pMesh->SetModelKey(GraphicsTransfer::ModelKey::GrassCubeMap);
	
	auto transform = GetComponent<Transform>();
	transform->SetLocalScale(Vector3(30.f, 30.f, 30.f));

	// 엔티티 Start 호출
	Entity::Start();
}
