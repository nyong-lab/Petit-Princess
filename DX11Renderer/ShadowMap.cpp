#include "renderpch.h"
#include "ShadowMap.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "DXResourceManager.h"

#include <DirectXColors.h>



ShadowMap::ShadowMap(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,  UINT width, UINT height)
	: m_device(device), m_context(deviceContext), mWidth(width), mHeight(height), m_depthMapSRV(0), m_depthMapDSV(0),
	m_screenQuadVB(nullptr), m_screenQuadIB(nullptr)
{
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	// Use typeless format because the DSV is going to interpret
	// the bits as DXGI_FORMAT_D24_UNORM_S8_UINT, whereas the SRV is going to interpret
	// the bits as DXGI_FORMAT_R24_UNORM_X8_TYPELESS.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap = 0;
	m_device->CreateTexture2D(&texDesc, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(depthMap, &dsvDesc, &m_depthMapDSV);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	m_device->CreateShaderResourceView(depthMap, &srvDesc, &m_depthMapSRV);

	// View saves a reference to the texture so we can release our reference.
	depthMap->Release();


	/// 렌더 타겟뷰도 하나 만들어 준다.
	// We call this function everytime the window is resized so that the render target is a quarter
	// the client area dimensions.  So Release the previous views before we create new ones.
	//m_renderTargetView->Release();

	D3D11_TEXTURE2D_DESC texDesc2;

	texDesc2.Width = mWidth;
	texDesc2.Height = mHeight;
	texDesc2.MipLevels = 1;
	texDesc2.ArraySize = 1;
	texDesc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc2.SampleDesc.Count = 1;
	texDesc2.SampleDesc.Quality = 0;
	texDesc2.Usage = D3D11_USAGE_DEFAULT;
	texDesc2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc2.CPUAccessFlags = 0;
	texDesc2.MiscFlags = 0;

	ID3D11Texture2D* offscreenTex = 0;
	m_device->CreateTexture2D(&texDesc2, 0, &offscreenTex);

	// Null description means to create a view to all mipmap levels using 
	// the format the texture was created with.
	//HR(md3dDevice->CreateRenderTargetView(offscreenTex, 0, &mOffscreenRTV));
	m_device->CreateRenderTargetView(offscreenTex, 0, &m_renderTargetView);

	// View saves a reference to the texture so we can release our reference.
	offscreenTex->Release();



}

ShadowMap::~ShadowMap()
{
	m_renderTargetView->Release();

	m_depthMapSRV->Release();
	m_depthMapDSV->Release();

	//m_screenQuadVB->Release();
	//m_screenQuadIB->Release();
}

ID3D11ShaderResourceView* ShadowMap::DepthMapSRV() 
{
	return m_depthMapSRV;
}


void ShadowMap::BindDsvAndSetNullRenderTarget()
{

	// Set null render target because we are only going to draw to depth buffer.
	// Setting a null render target will disable color writes.
	//ID3D11RenderTargetView* renderTargets[1] = { 0 };

	/// 렌더 타겟은 필요없는데, 기존의 쉐이더 루틴을 수정해야 한다.
	ID3D11RenderTargetView* renderTargets[1] = { m_renderTargetView };

	m_context->OMSetRenderTargets(1, renderTargets, m_depthMapDSV);

	m_context->RSSetViewports(1, &m_viewport);
	const float color[] = { 0.f, 0, 0, 0.f };
	m_context->ClearRenderTargetView(m_renderTargetView, color);
	m_context->ClearDepthStencilView(m_depthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// void ShadowMap::OnUpdate(XMMATRIX viewTM, XMMATRIX projectionTM, Vector3 camPos)
// {
// 	mLightView = viewTM;
// 	mLightProj = projectionTM;
// 	mCamPos = camPos;
// }

void ShadowMap::OnUpdate(Matrix viewTM, Matrix projectionTM, Vector3 camPos)
{
	mLightView = viewTM;
	mLightProj = projectionTM;
	mCamPos = camPos;
}

void ShadowMap::OnRender()
{
	/// 광원 입장에서의 Transform을 설정 해 준다.


}

void ShadowMap::BuildScreenQuadGeometryBuffers()
{

}




