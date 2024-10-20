#include "renderpch.h"
#include "PixelShader.h"
#include "DebugMacro.h"
#include "Device.h"

PixelShader::PixelShader()
{

}

PixelShader::~PixelShader()
{

}

bool PixelShader::Load(std::wstring filePath, std::shared_ptr<Device> device)
{
	HRESULT hr;

	ComPtr<ID3DBlob> blop;
	HR(D3DReadFileToBlob(filePath.c_str(), &blop));

	ComPtr<ID3D11PixelShader> pixelShader;
	HR(device->GetDevice()->CreatePixelShader(
		blop->GetBufferPointer(), blop->GetBufferSize(), nullptr, &pixelShader));

	m_pixelShader = pixelShader;

	return true;
}

bool PixelShader::Load()
{
	return true;
}

void PixelShader::UnLoad()
{
	m_pixelShader->Release();
}
