#include "GameDemo_pch.h"
#include "Grid.h"

#include "MeshRenderer.h"

Grid::Grid()
	: Entity("GridEntity")
{

}

Grid::~Grid()
{

}

void Grid::Start()
{
	// 렌더러 추가
	auto pRenderer = AddComponent<MeshRenderer>("GridRenderer");
	auto pMesh = AddComponent<MeshFilter>("GridMeshFilter");
	pMesh->SetModelKey(GraphicsTransfer::ModelKey::Grid);
	pMesh->m_pbrSet.useBaseColorMap = false;
	pMesh->m_cullingOff = true;

	// 엔티티 Start 호출
	Entity::Start();
}
