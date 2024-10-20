#include "renderpch.h"
#include "Buffer.h"

Buffer::Buffer()
{

}

Buffer::~Buffer()
{

}

bool Buffer::Load()
{
	return true;
}

void Buffer::UnLoad()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
}
