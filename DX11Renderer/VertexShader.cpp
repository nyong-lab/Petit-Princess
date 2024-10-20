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
	// ���ؽ� ���̴� �����
	HRESULT hr;
	ComPtr<ID3DBlob> blop;

	// ���ؽ� ���̴� �о�ͼ� �����ϱ�
	HR(D3DReadFileToBlob(filePath.c_str(), &blop));

	ComPtr<ID3D11VertexShader> vertexShader;
	HR(device->GetDevice()->CreateVertexShader(
		blop->GetBufferPointer(), blop->GetBufferSize(), nullptr, &vertexShader));

	ComPtr<ID3D11InputLayout> inputLayout;
	//��ǲ ���̾ƿ� �����
	HR(device->GetDevice()->CreateInputLayout(
		inputLayoutDesc.data(), UINT(inputLayoutDesc.size()),	// ����� ����
		blop->GetBufferPointer(),								// �����ϵ� ���̴��� ���� ������
		blop->GetBufferSize(),									// �����ϵ� ���̴��� ũ��
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
