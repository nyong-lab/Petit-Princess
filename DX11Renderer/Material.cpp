#include "renderpch.h"
#include "Material.h"
#include "DebugMacro.h"

#include "StringHelper.h"
#include "Device.h"

#include <locale>
#include <codecvt>

Material::Material()
{

}

Material::~Material()
{

}

bool Material::Load()
{
	return true;
}

bool Material::Load(std::shared_ptr<Device> device, const MaterialColor& color, aiTextureType type)
{
	Initialize1x1ColorTexture(device, color, type);
	return true;
}

bool Material::Load(std::shared_ptr<Device> device, const MaterialColor* colorData, UINT width, UINT height, aiTextureType type)
{
	InitializeColorTexture(device, colorData, width, height, type);
	return true;
}

bool Material::Load(std::wstring filePath, std::shared_ptr<Device> device, aiTextureType type)
{
	HRESULT hr;

	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);

	bool isSame = str.find("dds") != std::string::npos;
	if (isSame)
	{
		HR(DirectX::CreateDDSTextureFromFile(
			device->GetDevice().Get(), filePath.c_str(),
			(ID3D11Resource**)m_texture.GetAddressOf(),
			m_textureResourceView.GetAddressOf()));

		if (FAILED(hr))
		{
			Initialize1x1ColorTexture(device, Colors::UnhandledTextureColor, type);
		}

		return true;
	}
	else
	{
		/// CreateWICTextureFromFile 쓰기 전에 초기화 해야 함
		if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		{
			MessageBox(0, L"WICTexture Initialize Failed", 0, 0);
		}

		HR(DirectX::CreateWICTextureFromFile(
			device->GetDevice().Get(), filePath.c_str(),
			(ID3D11Resource**)m_texture.GetAddressOf(),
			m_textureResourceView.GetAddressOf()));

		if (FAILED(hr))
		{
			Initialize1x1ColorTexture(device, Colors::UnhandledTextureColor, type);
		}

		return true;
	}
}

bool Material::Load(std::wstring filePath, std::shared_ptr<Device> device)
{
	HRESULT hr = S_OK;
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);

	bool isSame = str.find("dds") != std::string::npos;
	if (isSame)
	{
		HR(DirectX::CreateDDSTextureFromFile(
			device->GetDevice().Get(), filePath.c_str(),
			(ID3D11Resource**)m_texture.GetAddressOf(),
			m_textureResourceView.GetAddressOf()));

		if (FAILED(hr))
		{
			MessageBox(0, L"DDSTexture Failed", 0, 0);
		}

		return true;
	}
	else
	{
		/// CreateWICTextureFromFile 쓰기 전에 초기화 해야 함
		if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		{
			MessageBox(0, L"WICTexture Initialize Failed", 0, 0);
		}

		HR(DirectX::CreateWICTextureFromFile(
			device->GetDevice().Get(), filePath.c_str(),
			(ID3D11Resource**)m_texture.GetAddressOf(),
			m_textureResourceView.GetAddressOf()));

		if (FAILED(hr))
		{
			MessageBox(0, L"WICTexture Failed", 0, 0);
		}

		return true;
	}
	
	return false;
}

bool Material::Load(std::wstring filePath, std::shared_ptr<Device> device, const uint8_t* pData, size_t size, aiTextureType type)
{
	HRESULT hr;
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);

	bool isSame = str.find("dds") != std::string::npos;
	if (isSame)
	{
		HR(DirectX::CreateDDSTextureFromMemory(
			device->GetDevice().Get(), pData, size,
			(ID3D11Resource**)m_texture.GetAddressOf(),
			m_textureResourceView.GetAddressOf()));

		return true;
	}
	else
	{
		/// CreateWICTextureFromFile 쓰기 전에 초기화 해야 함
		if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		{
			MessageBox(0, L"WICTexture Initialize Failed", 0, 0);
		}

		HR(DirectX::CreateWICTextureFromMemory(
			device->GetDevice().Get(), pData, size,
			(ID3D11Resource**)m_texture.GetAddressOf(),
			m_textureResourceView.GetAddressOf()));

		return true;
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Texture from memory.", 0, 0);
	}

	return false;
}

bool Material::Load(std::shared_ptr<Device> device, ID3D11ShaderResourceView* srv, aiTextureType textureType)
{
	if (srv)
	{
		m_textureResourceView = srv;
	}

	return true;
}

void Material::UnLoad()
{
	m_texture->Release();
	m_textureResourceView->Release();
}

void Material::Initialize1x1ColorTexture(std::shared_ptr<Device> device, const MaterialColor& colorData, aiTextureType type)
{
	InitializeColorTexture(device, &colorData, 1, 1, type);
}

void Material::InitializeColorTexture(std::shared_ptr<Device> device, const MaterialColor* colorData, UINT width, UINT height, aiTextureType type)
{
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(MaterialColor);
	HRESULT hr = device->GetDevice()->CreateTexture2D(&textureDesc, &initialData, &m_texture);

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->GetDevice()->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_textureResourceView.GetAddressOf());
}
