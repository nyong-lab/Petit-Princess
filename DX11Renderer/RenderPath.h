#pragma once

class GraphicsEngine;
class Mesh;
class ResourceManager;
class DXResourceManager;
class Device;
class DXManager;
class VertexShader;
class PixelShader;
class ShadowMap;
class Model;

#include <array>
#include "GraphicsTransfer.h"

using namespace GraphicsTransfer;

/// <summary>
/// 디퍼드 랜더의 패스
/// 박연하 손서희
/// </summary>
class RenderPath
{
	enum RenderState
	{
		ShadowCasting,
		RenderGBuffer,

		RenderLight,

		DownSampling,
		BlurX,
		BlurY,
		Bloom,

		FXAA,
		OutputToBackBuffer,

		/// After Deffered ----------

		Forward,
		UI,
		Text,
	};

	enum GBuffer
	{
		BaseColor,
		Normal,
		PositionDepth,
		RoughnessMetallicAO,

		GBufferCount,
	};

/// 디퍼드 전용 상수버퍼 ----------------------------------------------------------------------
	struct PointCb
	{
		PointLightData points[64];
		unsigned int count;
		Vector3 pad;
	};

	struct FrameCb
	{
		Vector4 Frame;
	};

	struct DownSampleCb
	{
		int Scale;
		Vector3 pad;
	};

/// 생성자와 소멸자 ----------------------------------------------------------------------
public:
	RenderPath();
	~RenderPath();


/// 멤버 함수 ---------------------------------------------------------------------------
public:
	void Initialize(std::shared_ptr<Device> deviceClass, int textureWidth, int textureHeight, 
		std::shared_ptr<DXResourceManager> resourceManager, std::shared_ptr<DXManager> dxManager);
	void ProcessDeffered();

	void ClearRenderTarget();

	void ResizeStart();
	void ResizeEnd(int width, int height);	

private:
	void InitializeGBuffer(ComPtr<ID3D11Device> device);
	void InitializeLightBloom();

	void InitializeCBuffer();

	void ObjectRender(ModelData modelData);
	void TextDraw(GraphicsTransfer::Text textInfo);

	void UnBind(UINT textureCount);
	void UpdateVertexConstantBuffer(std::shared_ptr<Mesh> mesh, ModelData data, ComPtr<ID3D11Buffer> buffer, VertexCBufferType type);
	void UpdatePixelConstantBuffer(ModelData data, ComPtr<ID3D11Buffer> buffer, PixelCBufferType type);

	void BindLightCBuffer();
	void BindFrameCBuffer(UINT slot, UINT invSize);
	void BindDownSampleCBuffer(UINT invSize);
	void BindWorldProperty(UINT slot);

	void CalcLightTM(Vector3 centerPosition); // 그림자를 위한 행렬

/// 렌더 대기열
public:
	std::vector<ModelData> m_models;
	std::vector<ModelData> m_UIs;
	DirectionalLightData m_directionalLight;
	std::vector<PointLightData> m_pointLights;
	std::vector<GraphicsTransfer::Text> m_texts;
	LightWorldProperty m_worldPropertyCb;

/// 멤버 변수 ---------------------------------------------------------------------------
private:
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<ID3D11Device> m_device;

	std::shared_ptr<DXResourceManager> m_resourceManager;
	std::shared_ptr<DXManager> m_dxManager;

	int m_textureWidth = 0;
	int m_textureHeight = 0;
	
	//World ----------------------------------------------------------------------------------
	ComPtr<ID3D11Buffer> m_worldPropertyBuffer;

	//Copy ----------------------------------------------------------------------------------
	std::shared_ptr<Mesh> m_geometryBuffer;
	std::shared_ptr<VertexShader> m_defferedVS;
	ComPtr<ID3D11RenderTargetView> m_backBuffer;
	ComPtr<ID3D11DepthStencilView> m_backBufferDepthStencil;

	//Gbuffer -------------------------------------------------------------------------------
	std::array<ComPtr<ID3D11Texture2D>, GBuffer::GBufferCount> m_renderTargetTextureArray;
	std::array<ComPtr<ID3D11RenderTargetView>, GBuffer::GBufferCount> m_renderTargetViewArray;
	std::array<ComPtr<ID3D11ShaderResourceView>, GBuffer::GBufferCount> m_shaderResourceViewArray;

	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	//Light ----------------------------------------------------------------------------------
	std::array<ComPtr<ID3D11Texture2D>,2> m_lightRenderTargetTexture; //0 : 원본, 1:블룸추출
	std::array<ComPtr<ID3D11RenderTargetView>,2> m_lightRenderTargetView;
	std::array<ComPtr<ID3D11ShaderResourceView>,2> m_lightShaderResourceView;
	
	ComPtr<ID3D11Texture2D> m_lightDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_lightDepthStencilView;

	std::shared_ptr<PixelShader> m_lightPS;
	ComPtr<ID3D11Buffer> m_lightCBuffer_Directional;
	ComPtr<ID3D11Buffer>m_lightCBuffer_Points;
	PointCb m_pointCb;

	//AO  ------------------------------------------------------------------------------------

	//Shadow  --------------------------------------------------------------------------------
	std::shared_ptr<ShadowMap> m_shadowMap;
	std::shared_ptr<PixelShader> m_shadowPixelShader;
	std::shared_ptr<VertexShader> m_shadowVertexShader;

	//std::shared_ptr<PixelShader> m_shadowPixelShader;
	std::shared_ptr<VertexShader> m_shadowStaticVertexShader;

	DirectX::BoundingSphere m_sceneBounds;

	Matrix m_lightViewTM;
	Matrix m_lightProjTM;
	Matrix m_shadowTransform;
	RenderState m_renderState;

	//FXAA  ------------------------------------------------------------------------------------
	std::shared_ptr<PixelShader> m_fxaaPS;
	ComPtr<ID3D11Buffer> m_frameBuffer;
	FrameCb m_frameCb;

	// Blur ----------------------------------------------------------------------------
	std::array<ComPtr<ID3D11Texture2D>, 4> m_blurXRenderTargetTexture;
	std::array<ComPtr<ID3D11RenderTargetView>, 4> m_blurXRenderTargetView;
	std::array<ComPtr<ID3D11ShaderResourceView>, 4> m_blurXShaderResourceView;
	std::shared_ptr<PixelShader> m_blurXPS;

	std::array<ComPtr<ID3D11Texture2D>, 4> m_blurYRenderTargetTexture;
	std::array<ComPtr<ID3D11RenderTargetView>, 4> m_blurYRenderTargetView;
	std::array<ComPtr<ID3D11ShaderResourceView>, 4> m_blurYShaderResourceView;
	std::shared_ptr<PixelShader> m_blurYPS;

	// Bloom ------------------------------------------------------------------------------------
 	std::shared_ptr<PixelShader> m_bloomPS;

	// DownSample ------------------------------------------------------------------------------------
	std::shared_ptr<PixelShader> m_downSamplePS;
	std::shared_ptr<VertexShader> m_downSampleVS;
	ComPtr<ID3D11Buffer> m_downSampleBuffer;
	DownSampleCb m_downSampleCb;
};





/*
constexpr uint32_t kNumGBufferTextures = 2;
constexpr uint32_t kNumFramesInFlight = 2;
// RTV layout (one frame):
// [backbuffer] [GBuffer] * kNumGBufferTextures
namespace RTVLayout
{
	enum
	{
		Backbuffer = 0,
		GBuffer,
		ProxyLight,
		DownSampledDepth,
		RTVcubemap,
		Count = DownSampledDepth + kNumGBufferTextures
	};
}

// SRV layout (one frame):
// [GBuffer] * kNumGBufferTextures [lighting output UAV] [lighting output SRV]
namespace SRVLayout
{
	enum
	{
		GBuffer,
		LightingUAV = GBuffer + kNumGBufferTextures,
		LightingSRV,
		VerticalBlurUAV,
		VerticalBlurSRV,
		DepthBufferSRV,
		ShadowMapSRV,
		DownSampledDepthUAV,
		DownSampledDepthSRV,
		TiledLightingPixelShaderCBV,
		TransMatrixCBV,
		PerLightPixelCBV,
		Count
	};
}

namespace DSVLayout
{
	enum
	{
		SceneDepth = 0,
		ShadowMap,
		LightCullingDepth,
		Count
	};
}

constexpr uint32_t kNumTextures = 30;
constexpr uint32_t kTextureSRVsOffset = SRVLayout::Count * kNumFramesInFlight;
constexpr uint32_t kNumSRVDescriptors = kTextureSRVsOffset + kNumTextures;


// root signatures slots helpers
namespace RootSignature
{
	namespace BackBuffer
	{
		enum
		{
			CountedLighting = 0,
			Count
		};
	}


	namespace Blur
	{
		enum
		{
			TextureToBlur = 0,
			BlurredTexture,
			Count
		};
	}

	namespace ProxyLight
	{
		enum
		{
			VertexConstantBuffer = 0,
			PixelConstantBuffer,
			TransMatrixConstantBuffer,
			LightingTilesBuffer,
			MinMaxDepths,
			PerLightConstantBuffer,
			Count
		};
	}

	namespace UAVClear
	{
		enum
		{
			ConstantBuffer,
			BufferToClear,
			Count
		};
	}

	namespace DepthDownsampling
	{
		enum
		{
			DownsampledTexture = 0,
			TextureToDownsample,
			Count

		};
	}

	namespace CopyDepth
	{
		enum
		{
			HiZ = 0,
			Count
		};
	}

	ComPtr<ID3D12Resource> mBackBufferTextures[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mDepthTextures[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mDownsampledDepthComputeOutput[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mLightCullingDepthMax[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mShadowMapTextures[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mBaseColorAndRoughness[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mNormalsAndMetalness[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mComputeOutput[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mVerticalBlurTexture[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mProxySphereOutput[kNumFramesInFlight];

	// buffers
	ComPtr<ID3D12Resource> mLightsBuffer;
	ComPtr<ID3D12Resource> mLightingTilesBuffer;
	ComPtr<ID3D12Resource> mUploadConstantBuffer[kNumFramesInFlight];
	ComPtr<ID3D12Resource> mMatTransConstantBuffer;
	ComPtr<ID3D12Resource> mPerLightPixelConstantBuffer;

	UINT8* mLightPixelConstantBufferGPUAddress[kNumFramesInFlight];


*/