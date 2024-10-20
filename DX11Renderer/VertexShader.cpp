#include "renderpch.h"
#include "VertexShader.h"
#include "DebugMacro.h"
#include "ErrorLogger.h"
#include "Device.h"

VertexShader::VertexShader()
{

}

VertexShader::~VertexShader()
{

}


bool VertexShader::Load(std::wstring filePath, std::shared_ptr<Device> device, const vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDesc)
{
	// 버텍스 쉐이더 만들기
	HRESULT hr;
	ComPtr<ID3DBlob> blop;

	// 버텍스 쉐이더 읽어와서 저장하기
	HR(D3DReadFileToBlob(filePath.c_str(), &blop));

	ComPtr<ID3D11VertexShader> vertexShader;
	HR(device->GetDevice()->CreateVertexShader(
		blop->GetBufferPointer(), blop->GetBufferSize(), nullptr, &vertexShader));

	ComPtr<ID3D11InputLayout> inputLayout;
	//인풋 레이아웃 만들기
	HR(device->GetDevice()->CreateInputLayout(
		inputLayoutDesc.data(), UINT(inputLayoutDesc.size()),	// 요소의 갯수
		blop->GetBufferPointer(),								// 컴파일된 셰이더에 대한 포인터
		blop->GetBufferSize(),									// 컴파일된 셰이더의 크기
		&inputLayout));											// [out]

	m_inputLayout = inputLayout;
	m_vertexShader = vertexShader;

	return true;
}

bool VertexShader::Load()
{
	return true;
}

void VertexShader::UnLoad()
{
	m_inputLayout->Release();
	m_vertexShader->Release();
}
