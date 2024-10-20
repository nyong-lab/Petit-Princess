#pragma once
#include "IResource.h"

#include "Device.h"
#include "BufferData.h"
#include "DebugMacro.h"

#include "Bone.h"

class Device;

/// <summary>
/// 버텍스 버퍼, 인덱스 버퍼를 가지고 있는 리소스
/// 
/// 24.01.19
/// 박연하
/// </summary>
class Mesh : public IResource
{
public:
	Mesh();
	~Mesh();

public:
	bool Load();

	template <typename VERTEX_T>
	bool Load(
		std::shared_ptr<Device> device,
		std::vector<VERTEX_T> vertexList,
		std::vector<UINT> indexList,
		D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		bool cpuWrite = false, bool gpuWrite = false);
	void UnLoad() override;

public:
	ComPtr<ID3D11Buffer> GetVertexBuffer() { return m_vertexBuffer; }
	UINT GetVertexStride() { return m_vertexStride; }
	UINT GetVertexOffset() { return m_vertexOffset; }

	ComPtr<ID3D11Buffer> GetIndexBuffer() { return m_indexBuffer; }
	UINT GetIndexCount() { return m_indexCount; }

	D3D_PRIMITIVE_TOPOLOGY GetTopology() { return m_primitiveTopology; }

public:
	std::string m_meshName;
	std::vector<Bone> m_bones;
	Matrix m_boneTransforms[512];

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	UINT m_vertexCount;
	UINT m_vertexStride;
	UINT m_vertexOffset;

	ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount;
	UINT m_indexStride;
	UINT m_indexOffset;

	D3D_PRIMITIVE_TOPOLOGY m_primitiveTopology;
};

template <typename VERTEX_T>
bool Mesh::Load(
	std::shared_ptr<Device> device,
	std::vector<VERTEX_T> vertexList,
	std::vector<UINT> indexList,
	D3D_PRIMITIVE_TOPOLOGY topology,
	bool cpuWrite, bool gpuWrite)
{
	m_primitiveTopology = topology;

	/// 버텍스 버퍼 초기화 ------------------------------------------------

	m_vertexCount = (unsigned int)vertexList.size();
	m_vertexOffset = 0u;
	m_vertexStride = sizeof(VERTEX_T);

	// 버텍스 버퍼 desc 설정
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = m_vertexStride * m_vertexCount;

	if (cpuWrite == false && gpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE; // CPU Read, GPU Read
	}
	else if (cpuWrite == true && gpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;  // CPU Write, GPU Read
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (cpuWrite == false && gpuWrite == true)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;  // CPU Read, GPU Write
	}
	else
	{
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertexList.data();

	HRESULT hr;
	HR(device->GetDevice()->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf()));

	/// -------------------------------------------------------------------

	/// 인덱스 버퍼 초기화 ---------------------------------------------------

	m_indexStride = sizeof(UINT);
	m_indexCount = static_cast<UINT>(indexList.size());

	// 인덱스 버퍼 desc 설정
	//D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0u;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = m_indexStride * m_indexCount;
	desc.StructureByteStride = m_indexStride;
	desc.MiscFlags = 0u;

	// 인덱스 버퍼 만들기
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indexList.data();  //indexData.pSysMem = indices;

	HR(device->GetDevice()->CreateBuffer(&desc, &data, m_indexBuffer.GetAddressOf()));

	/// -------------------------------------------------------------------

	return true;
}
