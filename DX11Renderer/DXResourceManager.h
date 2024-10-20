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
/// 그래픽 리소스들을 가지고 있고 관리하는 매니저
/// 
/// 24.01.21
/// 박연하
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
	/// 리소스가 이미 존재하면 리턴,
	/// 없으면 새로 불러와서 리소스 컨테이너에 넣는다
	/// 불러올 땐 filePath 그냥 L"" 하면 됩니당
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
	// 미리 로드할 리소스들
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
	// 공통적으로 쓰는 자원이니까 일단 여기에 생성
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
			// 로드 성공하면 리소스 넣기
			m_resources.insert(make_pair(key, resource));
			return resource;
		}
		else
		{
			assert(nullptr && "리소스 타입이 안맞음!");
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
