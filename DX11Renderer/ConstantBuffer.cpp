#include "renderpch.h"
#include "ConstantBuffer.h"
#include "DebugMacro.h"

ConstantBuffer::ConstantBuffer()
{

}

ConstantBuffer::~ConstantBuffer()
{

}

bool ConstantBuffer::Load(std::shared_ptr<Device> device, UINT bufferSize)
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = bufferSize;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr;
	HR(device->GetDevice()->CreateBuffer(&cbDesc, nullptr, m_constantBuffer.GetAddressOf()));

	return true;
}

bool ConstantBuffer::Load()
{
	return true;
}

void ConstantBuffer::UnLoad()
{
	m_constantBuffer->Release();
}
