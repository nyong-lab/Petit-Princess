#include "renderpch.h"
#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

bool Mesh::Load()
{
	return true;
}

void Mesh::UnLoad()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
}
