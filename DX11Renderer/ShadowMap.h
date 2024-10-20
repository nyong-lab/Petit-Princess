#pragma once

#include "MathHeaders.h"

/// 그림자 만들기(광원 기준 뎁스 버퍼)
/// 2024.02.14 김선웅
class GraphicsEngine;
class Mesh;
class ResourceManager;
class DXResourceManager;

class DXManager;
class VertexShader;
class PixelShader;

class ShadowMap 
{
public:
	ShadowMap(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> pDeviceContext, UINT width, UINT height);
	~ShadowMap();

	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDsvAndSetNullRenderTarget();
	void OnUpdate(Matrix viewTM, Matrix projectionTM, Vector3 camPos);
	void OnRender();
	void SetUp(std::shared_ptr<Mesh> geo,  std::shared_ptr<VertexShader> VS, std::shared_ptr<PixelShader> PS );
	void BuildScreenQuadGeometryBuffers();
private:
	ShadowMap(const ShadowMap& rhs);
	ShadowMap& operator=(const ShadowMap& rhs);

private:
	//ID3D11Device* m_device;
	//ID3D11DeviceContext* m_context;		// 디바이스 컨텍스트
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<ID3D11Device> m_device;

	UINT mWidth;
	UINT mHeight;

	ID3D11ShaderResourceView* m_depthMapSRV;
	ID3D11DepthStencilView* m_depthMapDSV;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11Texture2D* m_renderTargetTexture = nullptr;;

	// 그림자만 그릴 것이라면 필요없다.
	ID3D11RenderTargetView* m_renderTargetView;		// 랜더 타겟 뷰

	D3D11_VIEWPORT m_viewport;

	ID3D11Buffer* m_screenQuadVB;
	ID3D11Buffer* m_screenQuadIB;

	Matrix mLightView;
	Matrix mLightProj;
	Vector3 mCamPos;


	//Copy ----------------------------------------------------------------------------------
	std::shared_ptr<Mesh> m_geometryBuffer;
 	std::shared_ptr<PixelShader> m_lightPixelShader;
 	std::shared_ptr<VertexShader> m_vertexShader;
};
