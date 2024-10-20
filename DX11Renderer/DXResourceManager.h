#pragma once

#include <map>
#include <string>
#include <memory>

#include "IResource.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Model.h"

class Device;

/// <summary>
/// �׷��� ���ҽ����� ������ �ְ� �����ϴ� �Ŵ���
/// 
/// 24.01.21
/// �ڿ���
/// </summary>
class DXResourceManager : public std::enable_shared_from_this<DXResourceManager>
{
public:
	DXResourceManager() = default;
	DXResourceManager(std::wstring basePath, std::shared_ptr<Device> device);
	~DXResourceManager() = default;

public:
	void Initialize();
	void Finalize();

	/// <summary>
	/// ���ҽ��� �̹� �����ϸ� ����,
	/// ������ ���� �ҷ��ͼ� ���ҽ� �����̳ʿ� �ִ´�
	/// �ҷ��� �� filePath �׳� L"" �ϸ� �˴ϴ�
	/// </summary>
	template<typename T, typename... Args>
	std::shared_ptr<T> LoadResource(std::wstring key, Args&&... args);

	void UnLoadResource(std::wstring key);

public:
	ComPtr<ID3D11SamplerState> GetLinearSampleState() { return m_linearSamplerState; }
	ComPtr<ID3D11DepthStencilState> GetBasicDepthStencilState() { return m_basicDepthState; }
	ComPtr< ID3D11RasterizerState> GetBackCullRasterizerState() { return m_cullBackRasterState; }
	ComPtr<ID3D11RasterizerState> GetNoneCullRasterizerState() { return m_cullBackRasterState; }
	ComPtr<ID3D11BlendState> GetBlendState() { return m_alphaDisabledBlendState; }
	ComPtr<ID3D11DepthStencilState> GetDepthDisabledState() { return m_depthDisabledDepthState; }

private:
	// �̸� �ε��� ���ҽ���
	void CreateSamplerState();
	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateBlendState();
	void CreateBindState();

	void CreateGeoMetry();
	void CreateVertexShaderAndInputLayout();
	void CreatePixelShader();
	void CreateMaterial();

	// Geometry Buffer
	void CreateGrid();
	void CreateTextureBox();
	void CreateWireFrameBox();
	void CreateWireFrameSphere();
	void CreateWireFrameCapsule_Top();
	void CreateWireFrameCapsule_Middle();
	void CreateWireFrameCapsule_Bottom();
	void CreateWireFrameRay();
	void CreateSkyBox();
	void CreateFullScreenRect();
	void CreateCanvas();
	void CreateUIRect(float width, float height, std::wstring key);
	void CreateModel();

private:
	std::wstring m_basePath;
	std::shared_ptr<Device> m_device;
	std::map<std::wstring, std::shared_ptr<IResource>> m_resources;

private:
	// ���������� ���� �ڿ��̴ϱ� �ϴ� ���⿡ ����
	ComPtr<ID3D11SamplerState> m_linearSamplerState;

	ComPtr<ID3D11DepthStencilState> m_basicDepthState;
	ComPtr<ID3D11DepthStencilState> m_depthDisabledDepthState;
	ComPtr<ID3D11DepthStencilState> m_cubeMapDepthState;

	ComPtr<ID3D11RasterizerState>	m_cullBackRasterState;
	ComPtr<ID3D11RasterizerState>	m_cullFrontRasterState;
	ComPtr<ID3D11RasterizerState>	m_cullNoneRasterState;

	ComPtr<ID3D11BlendState>		m_alphaDisabledBlendState;
	ComPtr<ID3D11BlendState>		m_alphaEnabledBlendState;
};

template<typename T, typename... Args>
std::shared_ptr<T>
DXResourceManager::LoadResource(std::wstring key, Args&&... args)
{
	auto iter = m_resources.find(key);
	if (iter == m_resources.end())
	{
		std::shared_ptr<T> resource = std::make_shared<T>();
		if (resource->Load(std::forward<Args>(args)...))
		{
			// �ε� �����ϸ� ���ҽ� �ֱ�
			m_resources.insert(make_pair(key, resource));
			return resource;
		}
		else
		{
			assert(nullptr && "���ҽ� Ÿ���� �ȸ���!");
			return nullptr;
		}
	}
	else
	{
		std::shared_ptr<T> resource = dynamic_pointer_cast<T>(iter->second);
		if (resource)
		{
			return resource;
		}
	}

	return nullptr;
}
