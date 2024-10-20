#include "renderpch.h"
#include "RenderPath.h"
#include "GraphicsEngine.h"
#include "DebugMacro.h"
#include "Device.h"
#include "DXResourceManager.h"
#include "DXManager.h"
#include "ConstantBuffer.h"
#include "BufferData.h"
#include "Material.h"
#include "BindState.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Buffer.h"
#include "ShadowMap.h"
#include "GraphicsTransfer.h"

#include "MathUtils.h"

RenderPath::RenderPath()
//:m_graphics(nullptr)
{

}

RenderPath::~RenderPath()
{

}

void RenderPath::Initialize(shared_ptr<Device> deviceClass, int textureWidth, int textureHeight,
	shared_ptr<DXResourceManager> resourceManager, std::shared_ptr<DXManager> dxManager)
{
	HRESULT hr = S_OK;
	m_resourceManager = resourceManager;
	m_dxManager = dxManager;
	m_context = deviceClass->GetDeviceContext();
	m_device = deviceClass->GetDevice();

	// 백버퍼 정보 받아오기
	m_backBuffer = m_dxManager->m_renderTargetView;
	m_backBufferDepthStencil = dxManager->m_depthStencilView;
	m_depthStencilState = m_resourceManager->GetBasicDepthStencilState();

	// 렌더 텍스처의 폭과 높이를 저장한다. 
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

	// 쉐이더 로드
	m_defferedVS = m_resourceManager->LoadResource<VertexShader>(L"DefferedProcessVS");
	m_downSampleVS = m_resourceManager->LoadResource<VertexShader>(L"DownSampleVS");

	m_lightPS = m_resourceManager->LoadResource<PixelShader>(L"DefferedDirLightPS");
	m_fxaaPS = m_resourceManager->LoadResource<PixelShader>(L"DefferedFXAAPS");
	m_blurXPS = m_resourceManager->LoadResource<PixelShader>(L"BlurXPS");
	m_blurYPS = m_resourceManager->LoadResource<PixelShader>(L"BlurYPS");
	m_bloomPS = m_resourceManager->LoadResource<PixelShader>(L"BloomPS");
	m_downSamplePS = m_resourceManager->LoadResource<PixelShader>(L"DownSamplePS");

	// 전체화면 버텍스, 인덱스
	m_geometryBuffer = m_resourceManager->LoadResource<Mesh>(L"FullScreenRectBuffer");

	// 랜더타겟, 상수버퍼 초기화
	InitializeCBuffer();
	InitializeGBuffer(m_device);
	InitializeLightBloom();

	// 그림자 클래스
	m_shadowVertexShader = m_resourceManager->LoadResource<VertexShader>(L"DeferredShadowDepthVS");
	m_shadowStaticVertexShader = m_resourceManager->LoadResource<VertexShader>(L"DeferredShadowStaticVS");
	m_shadowPixelShader = m_resourceManager->LoadResource<PixelShader>(L"DeferredShadowDepthPS");

	m_shadowMap = std::make_shared<ShadowMap>(m_device, m_context, 2048, 2048); // 그림자 해상도 일단 2048로	
}

void RenderPath::ProcessDeffered()
{
	//구분하기 편하라고 요렇게 해봤습니당!!
	RenderPath::ShadowCasting;
	{
		m_renderState = RenderPath::ShadowCasting;

		m_shadowMap->BindDsvAndSetNullRenderTarget();
		Matrix cameraTM = Matrix::Identity;

		for (auto& model : m_models)
		{
			ObjectRender(model);
		}

		// 카메라 위치 받기 
		if (m_models.size() != 0)
			cameraTM = m_models[0].transform.cameraWorld;
		CalcLightTM({ cameraTM._41, -1, cameraTM._43 + 20 });
		m_directionalLight.gShadowTransform = (m_lightViewTM * m_lightProjTM).Transpose();
	}

	RenderState::RenderGBuffer;
	{
		m_dxManager->ResetViewport();

		m_renderState = RenderPath::RenderGBuffer;
		// 렌더링 대상 뷰 배열, 깊이 스텐실 버퍼를 출력 렌더 파이프 라인에 바인딩한다.
		m_context->OMSetRenderTargets(GBuffer::GBufferCount, m_renderTargetViewArray[0].GetAddressOf(), m_depthStencilView.Get());
		//m_context->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthStencilView.Get());	// 렌더 타겟 뷰 초기화
		m_context->OMSetDepthStencilState(NULL, 1);						// 깊이 스텐실 상태 초기화
		m_context->IASetPrimitiveTopology(m_geometryBuffer->GetTopology());

		// 렌더 큐에서 오브젝트 가져와서 그리기
		for (auto& model : m_models)
		{
			ObjectRender(model);
		}

		m_models.clear();
	}

	RenderPath::RenderLight;
	{
		// 렌터 타겟을 라이트 전용으로 변경
		m_context->OMSetRenderTargets(2, m_lightRenderTargetView[0].GetAddressOf(), m_lightDepthStencilView.Get());	// 렌더 타겟 뷰 초기화
		//m_context->OMSetDepthStencilState(NULL, 1);						// 깊이 스텐실 상태 초기화
		m_context->OMSetDepthStencilState(m_resourceManager->GetDepthDisabledState().Get(), 1);	// 깊이 스텐실 상태 뎁스 비활성화로 설정

		// 버텍스 쉐이더와 픽셀 쉐이더 설정
		m_context->VSSetShader(m_defferedVS->GetVertexShader().Get(), 0, 0);
		m_context->PSSetShader(m_lightPS->GetPixelShader().Get(), 0, 0);

		BindLightCBuffer();
		BindWorldProperty(2);

		// gbuffer에 그렸던 내용들을 Texture로 연결 + shadowMap도 같이	
		for (int i = 0; i < GBuffer::GBufferCount; i++)
		{
			m_context->PSSetShaderResources(i, 1, m_shaderResourceViewArray[i].GetAddressOf());
		}

		ID3D11ShaderResourceView* srvArray[1] = { m_shadowMap->DepthMapSRV() };
		m_context->PSSetShaderResources(GBuffer::GBufferCount, 1, srvArray);
		m_context->PSSetSamplers(0, 1, m_resourceManager->GetLinearSampleState().GetAddressOf());

		// Set Topology
		ID3D11Buffer* vb = m_geometryBuffer->GetVertexBuffer().Get();
		UINT offset = m_geometryBuffer->GetVertexOffset();
		UINT stride = m_geometryBuffer->GetVertexStride();

		m_context->IASetPrimitiveTopology(m_geometryBuffer->GetTopology());
		m_context->IASetInputLayout(m_defferedVS->GetInputLayout().Get());
		m_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_context->IASetIndexBuffer(m_geometryBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// 렌더 큐에서 라이트 가져와서 그리기
		DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));

		// 렌더타겟을 텍스쳐로 사용 후 언바인딩 안해주면 경고를 뱉는다
		UnBind(GBuffer::GBufferCount + 1);
	}

	RenderPath::DownSampling;
	{
		// 버텍스 쉐이더와 픽셀 쉐이더 설정
		m_context->VSSetShader(m_downSampleVS->GetVertexShader().Get(), 0, 0);
		m_context->PSSetShader(m_downSamplePS->GetPixelShader().Get(), 0, 0);

		// 1/4배
		{
			m_context->OMSetRenderTargets(1, m_blurYRenderTargetView[1].GetAddressOf(), NULL);
			BindFrameCBuffer(0, 4);
			BindDownSampleCBuffer(4);
			m_context->PSSetShaderResources(0, 1, m_lightShaderResourceView[1].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}

		// 1/16배
		{
			m_context->OMSetRenderTargets(1, m_blurYRenderTargetView[2].GetAddressOf(), NULL);
			BindFrameCBuffer(0, 16);
			BindDownSampleCBuffer(16);
			m_context->PSSetShaderResources(0, 1, m_blurYShaderResourceView[1].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}

		// 1/64배
		{
			m_context->OMSetRenderTargets(1, m_blurYRenderTargetView[3].GetAddressOf(), NULL);
			BindFrameCBuffer(0, 64);
			BindDownSampleCBuffer(64);
			m_context->PSSetShaderResources(0, 1, m_blurYShaderResourceView[2].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}
	}

	/// 다운샘플링한 값 X축 블러
	RenderPath::BlurX;
	{
		// 버텍스 쉐이더와 픽셀 쉐이더 설정
		m_context->VSSetShader(m_downSampleVS->GetVertexShader().Get(), 0, 0);
		m_context->PSSetShader(m_blurXPS->GetPixelShader().Get(), 0, 0);

		// 기본
		{
			auto a = m_frameCb;
			m_context->OMSetRenderTargets(1, m_blurXRenderTargetView[0].GetAddressOf(), NULL);
			BindFrameCBuffer(0, 1);
			BindDownSampleCBuffer(1);
			m_context->PSSetShaderResources(0, 1, m_lightShaderResourceView[1].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}

		// 1/2
		{
			m_context->OMSetRenderTargets(1, m_blurXRenderTargetView[1].GetAddressOf(), NULL);
			BindFrameCBuffer(0, 4);
			BindDownSampleCBuffer(4);
			m_context->PSSetShaderResources(0, 1, m_blurYShaderResourceView[1].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}

		// 1/4
		{
			m_context->OMSetRenderTargets(1, m_blurXRenderTargetView[2].GetAddressOf(), NULL);
			BindFrameCBuffer(0, 16);
			BindDownSampleCBuffer(16);
			m_context->PSSetShaderResources(0, 1, m_blurYShaderResourceView[2].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}

		// 1/8
		{
			m_context->OMSetRenderTargets(1, m_blurXRenderTargetView[3].GetAddressOf(), NULL);
			BindFrameCBuffer(0, 64);
			BindDownSampleCBuffer(64);
			m_context->PSSetShaderResources(0, 1, m_blurYShaderResourceView[3].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}
	}

	/// 다운샘플링한 값 Y축 블러
	RenderPath::BlurY;
	{
		// 버텍스 쉐이더와 픽셀 쉐이더 설정
		m_context->VSSetShader(m_downSampleVS->GetVertexShader().Get(), 0, 0);
		m_context->PSSetShader(m_blurYPS->GetPixelShader().Get(), 0, 0);

		for (int i = 0; i < 4; i++)
		{
			int scale = pow(4, i);
			m_context->OMSetRenderTargets(1, m_blurYRenderTargetView[i].GetAddressOf(), NULL);
			BindFrameCBuffer(0, scale);
			BindDownSampleCBuffer(scale);
			m_context->PSSetShaderResources(0, 1, m_blurXShaderResourceView[i].GetAddressOf());
			DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));
		}
	}

	/// 원본 이미지와 혼합한다.
	RenderPath::Bloom;
	{
		// 렌더 타겟 설정
		m_context->OMSetRenderTargets(1, m_lightRenderTargetView[1].GetAddressOf(), m_lightDepthStencilView.Get());

		// 깊이 스텐실 상태 뎁스 비활성화로 설정
		m_context->OMSetDepthStencilState(m_resourceManager->GetDepthDisabledState().Get(), 1);

		// 버텍스 쉐이더와 픽셀 쉐이더 설정
		m_context->VSSetShader(m_defferedVS->GetVertexShader().Get(), 0, 0);
		m_context->PSSetShader(m_bloomPS->GetPixelShader().Get(), 0, 0);

		// 상수버퍼 바인드
		BindWorldProperty(0);

		// XY 블러 처리한 텍스쳐 바인드
		m_context->PSSetShaderResources(0, 1, m_lightShaderResourceView[0].GetAddressOf());  // 기본

		for (int i = 0; i < 4; i++)
			m_context->PSSetShaderResources(i + 1, 1, m_blurYShaderResourceView[i].GetAddressOf());

		// 드로우
		DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));

		// 사용한 텍스쳐 언바인드
		UnBind(5);
	}

	/// 안티엘리어싱
	RenderPath::FXAA;
	{
		// 렌터 타겟을 백버퍼로 변경
		m_context->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_lightDepthStencilView.Get());	// 렌더 타겟 뷰 초기화
		m_context->OMSetDepthStencilState(m_resourceManager->GetDepthDisabledState().Get(), 1);	// 깊이 스텐실 상태 뎁스 비활성화로 설정
		m_context->RSSetState(m_resourceManager->GetNoneCullRasterizerState().Get());

		// 버텍스 쉐이더와 픽셀 쉐이더 설정
		m_context->VSSetShader(m_defferedVS->GetVertexShader().Get(), 0, 0);
		m_context->PSSetShader(m_fxaaPS->GetPixelShader().Get(), 0, 0);

		// 상수버퍼 바인드
		BindFrameCBuffer(0, 1);

		// 텍스쳐 바인드
		m_context->PSSetShaderResources(0, 1, m_lightShaderResourceView[1].GetAddressOf());

		// Set Topology
		ID3D11Buffer* vb = m_geometryBuffer->GetVertexBuffer().Get();
		UINT offset = m_geometryBuffer->GetVertexOffset();
		UINT stride = m_geometryBuffer->GetVertexStride();
		m_context->IASetPrimitiveTopology(m_geometryBuffer->GetTopology());
		m_context->IASetInputLayout(m_defferedVS->GetInputLayout().Get());
		m_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_context->IASetIndexBuffer(m_geometryBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// back buffer에 그리기
		DRAW_CALL(m_context->DrawIndexed(m_geometryBuffer->GetIndexCount(), 0, 0));

		m_context->OMSetDepthStencilState(NULL, 1);	// 기본 스텐실로 설정
		m_context->RSSetState(m_resourceManager->GetBackCullRasterizerState().Get());

		// 사용한 텍스쳐 언바인드
		UnBind(1);
	}

	RenderPath::OutputToBackBuffer;

	RenderPath::Forward;

	RenderPath::UI;
	{
		// 스탠실도 백버퍼로 변경
		m_context->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_backBufferDepthStencil.Get());
		for (auto& ui : m_UIs)
		{
			ObjectRender(ui);
		}
		m_UIs.clear();
	}

	RenderPath::Text;
	{
		for (auto& text : m_texts)
			TextDraw(text);

		m_texts.clear();
	}

	// 렌더타겟을 텍스쳐로 사용 후 언바인딩 안해주면 경고를 뱉는다
	UnBind(GBuffer::GBufferCount + 1);
}

void RenderPath::ClearRenderTarget()
{
	//const float color[] = { 1.f, 0.8235f, 0.8627f, 1.f };
	const float color[] = { 0.f, 0, 0, 0.f };

	RenderState::RenderGBuffer;
	{
		// 렌더 타겟 버퍼를 지운다.
		for (int i = 0; i < GBuffer::GBufferCount; i++)
		{
			m_context->ClearRenderTargetView(m_renderTargetViewArray[i].Get(), color);
		}

		//깊이 버퍼를 지운다
		m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	RenderPath::RenderLight;
	{
		m_context->ClearRenderTargetView(m_lightRenderTargetView[0].Get(), color);
		m_context->ClearRenderTargetView(m_lightRenderTargetView[1].Get(), color);
		m_context->ClearDepthStencilView(m_lightDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	RenderPath::Bloom;
	{
		for (int i = 0; i < 4; i++)
		{
			m_context->ClearRenderTargetView(m_blurXRenderTargetView[i].Get(), color);
			m_context->ClearRenderTargetView(m_blurYRenderTargetView[i].Get(), color);
		}
	}
}

void RenderPath::ResizeStart()
{
	RenderState::RenderGBuffer;
	{
		for (int i = 0; i < GBuffer::GBufferCount; i++)
		{
			m_shaderResourceViewArray[i]->Release();
			m_renderTargetViewArray[i]->Release();
			m_renderTargetTextureArray[i]->Release();
		}

		m_depthStencilView->Release();
		m_depthStencilBuffer->Release();
	}

	RenderPath::RenderLight;
	{
		m_lightRenderTargetTexture[0]->Release();
		m_lightRenderTargetView[0]->Release();
		m_lightShaderResourceView[0]->Release();
		m_lightRenderTargetTexture[1]->Release();
		m_lightRenderTargetView[1]->Release();
		m_lightShaderResourceView[1]->Release();
		m_lightDepthStencilBuffer->Release();
		m_lightDepthStencilView->Release();
	}

	RenderPath::Bloom;
	{
		for (int i = 0; i < 4; i++)
		{
			m_blurXRenderTargetTexture[i]->Release();
			m_blurXRenderTargetView[i]->Release();
			m_blurXShaderResourceView[i]->Release();
			m_blurYRenderTargetTexture[i]->Release();
			m_blurYRenderTargetView[i]->Release();
			m_blurYShaderResourceView[i]->Release();
		}
	}
}

void RenderPath::ResizeEnd(int width, int height)
{
	m_textureWidth = width;
	m_textureHeight = height;

	m_backBuffer = m_dxManager->m_renderTargetView;
	InitializeGBuffer(m_device);
	InitializeLightBloom();
}

void RenderPath::InitializeGBuffer(ComPtr<ID3D11Device> device)
{
	HRESULT hr;

	// 렌더 타겟 텍스처 구조체를 초기화한다.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = m_textureWidth;
	textureDesc.Height = m_textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  ///~!!!@!@!#$!$!$!#!#!$!$!$
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// 렌더 타겟 텍스처를 생성한다.
	for (int i = 0; i < GBuffer::GBufferCount; i++)
	{
		HR(device->CreateTexture2D(&textureDesc, NULL, m_renderTargetTextureArray[i].GetAddressOf()));
	}

	// 렌더 타켓 뷰의 구조체를 설정한다.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// 렌더 타겟 뷰를 생성한다.
	for (int i = 0; i < GBuffer::GBufferCount; i++)
	{
		HR(device->CreateRenderTargetView(m_renderTargetTextureArray[i].Get(), &renderTargetViewDesc, m_renderTargetViewArray[i].GetAddressOf()));
	}

	// 셰이더 리소스 뷰의 구조체를 설정한다.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// 셰이더 리소스 뷰를 생성한다.
	for (int i = 0; i < GBuffer::GBufferCount; i++)
	{
		HR(device->CreateShaderResourceView(m_renderTargetTextureArray[i].Get(), &shaderResourceViewDesc, m_shaderResourceViewArray[i].GetAddressOf()));
	}

	// 깊이 버퍼의 구조체를 초기화한다.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = m_textureWidth;
	depthBufferDesc.Height = m_textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; ///$@$@$~$!%!%!%!%
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// 깊이 버퍼 텍스처를 생성
	HR(device->CreateTexture2D(&depthBufferDesc, NULL, m_depthStencilBuffer.GetAddressOf()));

	// 깊이 스텐실 뷰 구조체 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; ///5#%!%!%!%!%!%!$%%
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 스텐실 뷰 생성
	HR(device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf()));
}

void RenderPath::InitializeLightBloom()
{
	HRESULT hr;

	/// 렌더 타겟 텍스처 구조체를 초기화한다.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = m_textureWidth;
	textureDesc.Height = m_textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_R32G32B32A32_FLOAT;  ///~!!!@!@!#$!$!$!#!#!$!$!$
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	/// 렌더 타겟 텍스처를 생성한다.
	HR(m_device->CreateTexture2D(&textureDesc, NULL, m_lightRenderTargetTexture[0].GetAddressOf()));  // 원본 이미지
	HR(m_device->CreateTexture2D(&textureDesc, NULL, m_lightRenderTargetTexture[1].GetAddressOf()));  // 밝기 추출 , 블룸 합성

	for (int i = 1; i <= 4; i++)
	{
		textureDesc.Width = m_textureWidth / pow(4, i - 1);
		textureDesc.Height = m_textureHeight / pow(4, i - 1);

		HR(m_device->CreateTexture2D(&textureDesc, NULL, m_blurXRenderTargetTexture[i - 1].GetAddressOf()));  // 블러 X
		HR(m_device->CreateTexture2D(&textureDesc, NULL, m_blurYRenderTargetTexture[i - 1].GetAddressOf()));  // 블러 Y
	}


	/// 렌더 타켓 뷰의 구조체를 설정한다.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	/// 렌더 타겟 뷰를 생성한다.
	HR(m_device->CreateRenderTargetView(m_lightRenderTargetTexture[0].Get(), &renderTargetViewDesc, m_lightRenderTargetView[0].GetAddressOf()));
	HR(m_device->CreateRenderTargetView(m_lightRenderTargetTexture[1].Get(), &renderTargetViewDesc, m_lightRenderTargetView[1].GetAddressOf()));

	for (int i = 0; i < 4; i++)
	{
		HR(m_device->CreateRenderTargetView(m_blurXRenderTargetTexture[i].Get(), &renderTargetViewDesc, m_blurXRenderTargetView[i].GetAddressOf()));
		HR(m_device->CreateRenderTargetView(m_blurYRenderTargetTexture[i].Get(), &renderTargetViewDesc, m_blurYRenderTargetView[i].GetAddressOf()));
	}

	/// 셰이더 리소스 뷰의 구조체를 설정한다.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	/// 셰이더 리소스 뷰를 생성한다.
	HR(m_device->CreateShaderResourceView(m_lightRenderTargetTexture[0].Get(), &shaderResourceViewDesc, m_lightShaderResourceView[0].GetAddressOf()));
	HR(m_device->CreateShaderResourceView(m_lightRenderTargetTexture[1].Get(), &shaderResourceViewDesc, m_lightShaderResourceView[1].GetAddressOf()));

	for (int i = 0; i < 4; i++)
	{
		HR(m_device->CreateShaderResourceView(m_blurXRenderTargetTexture[i].Get(), &shaderResourceViewDesc, m_blurXShaderResourceView[i].GetAddressOf()));
		HR(m_device->CreateShaderResourceView(m_blurYRenderTargetTexture[i].Get(), &shaderResourceViewDesc, m_blurYShaderResourceView[i].GetAddressOf()));
	}
	/// 깊이 버퍼의 구조체를 초기화한다.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = m_textureWidth;
	depthBufferDesc.Height = m_textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	/// 깊이 버퍼 텍스처를 생성
	HR(m_device->CreateTexture2D(&depthBufferDesc, NULL, m_lightDepthStencilBuffer.GetAddressOf()));

	/// 깊이 스텐실 뷰 구조체 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	/// 깊이 스텐실 뷰 생성
	HR(m_device->CreateDepthStencilView(m_lightDepthStencilBuffer.Get(), &depthStencilViewDesc, m_lightDepthStencilView.GetAddressOf()));
}

void RenderPath::InitializeCBuffer()
{
	HRESULT hr;

	// 디렉셔널 라이트용
	{
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = sizeof(DirectionalLightData);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HR(m_device->CreateBuffer(&cbDesc, nullptr, m_lightCBuffer_Directional.GetAddressOf()));
	}

	// 포인트라이트용
	{
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = sizeof(PointCb);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HR(m_device->CreateBuffer(&cbDesc, nullptr, m_lightCBuffer_Points.GetAddressOf()));
	}

	//스크린 크기 버퍼
	{
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = sizeof(FrameCb);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HR(m_device->CreateBuffer(&cbDesc, nullptr, m_frameBuffer.GetAddressOf()));
	}

	// 월드
	{
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = sizeof(LightWorldProperty);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HR(m_device->CreateBuffer(&cbDesc, nullptr, m_worldPropertyBuffer.GetAddressOf()));
	}

	// 다운샘플링
	{
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = sizeof(DownSampleCb);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HR(m_device->CreateBuffer(&cbDesc, nullptr, m_downSampleBuffer.GetAddressOf()));
	}
}

void RenderPath::ObjectRender(ModelData modelData)
{
	auto model = m_resourceManager->LoadResource<Model>(modelData.modelKey);

	for (int index = 0; index < model->m_meshes.size(); index++)
	{
		// 몇번째 메쉬를 렌더하고 있는지 알기 위해 사용
		modelData.ui.meshCount++;

		// Set Shader, Constant Buffer
		std::shared_ptr<VertexShader> vertexShader;

		// 쉐도우 패스 중에 쉐이더 세팅
		if (m_renderState == RenderState::ShadowCasting)
		{
			if (model->m_hasShadow == false)
				return;
			if (model->m_hasAnimation)
			{
				vertexShader = m_shadowVertexShader;
				m_context->VSSetShader(vertexShader->GetVertexShader().Get(), 0, 0);
				auto pixelShader = m_shadowPixelShader;
				m_context->PSSetShader(pixelShader->GetPixelShader().Get(), 0, 0);
			}
			else
			{
				vertexShader = m_shadowStaticVertexShader;
				m_context->VSSetShader(vertexShader->GetVertexShader().Get(), 0, 0);
				auto pixelShader = m_shadowPixelShader;
				m_context->PSSetShader(pixelShader->GetPixelShader().Get(), 0, 0);
			}
		}
		else
		{
			vertexShader = model->m_vertexShader.lock();
			m_context->VSSetShader(vertexShader->GetVertexShader().Get(), 0, 0);

			auto pixelShader = model->m_pixelShader.lock();
			m_context->PSSetShader(pixelShader->GetPixelShader().Get(), 0, 0);
		}


		for (int k = 0; k < model->m_vertexCB.size(); k++)
		{
			auto vertexCB = model->m_vertexCB[k].lock()->GetConstantBuffer();
			UpdateVertexConstantBuffer(model->m_meshes[index], modelData, vertexCB, model->m_vertexCbufferTypes[k]);
			m_context->VSSetConstantBuffers(k, 1, vertexCB.GetAddressOf());
		}


		for (int j = 0; j < model->m_pixelCB.size(); j++)
		{
			// 텍스쳐가 없는데 텍스쳐를 사용하고 있다면 Magenta 처리
			if (model->m_materials.size() == 0 && modelData.meterial.useMap[0])
			{
				modelData.meterial.useMap[0] = false;
				modelData.meterial.baseColor = Vector4(1.f, 0.f, 1.f, 1.f);
			}

			auto pixelCB = model->m_pixelCB[j].lock()->GetConstantBuffer();
			UpdatePixelConstantBuffer(modelData, pixelCB, model->m_pixelCbufferTypes[j]);
			m_context->PSSetConstantBuffers(j, 1, pixelCB.GetAddressOf());
		}

		// Set Texture
		std::vector<ID3D11ShaderResourceView*> textureArray;
		if (model->m_materials.size() != 0)
		{
			auto textureList = model->m_materials[index];
			for (int l = 0; l < textureList.size(); l++)
			{
				textureArray.push_back(textureList[l].lock()->GetTextureResourceView().Get());
			}

			if (!textureArray.empty())
			{
				m_context->PSSetShaderResources(0, textureArray.size(), textureArray.data());
			}
		}

		// Set Bind State
		auto bindState = model->m_bindState.lock();

		/// Sampler State
		if (bindState && bindState->m_hasSampler)
		{
			m_context->PSSetSamplers(0, 1, bindState->m_sampler.GetAddressOf());
		}
		else
		{
			m_context->PSSetSamplers(0, 1, m_resourceManager->GetLinearSampleState().GetAddressOf());
		}

		/// DepthStencil State
		if (bindState && bindState->m_hasDepthStencil)
		{
			m_context->OMSetDepthStencilState(bindState->m_depthStencil.Get(), 1u);
		}
		else
		{
			m_context->OMSetDepthStencilState(m_resourceManager->GetBasicDepthStencilState().Get(), 1u);
		}

		/// Rasterizer State
		if (bindState && bindState->m_hasRasterizer)
		{
			m_context->RSSetState(bindState->m_rasterizer.Get());
		}
		else
		{
			m_context->RSSetState(m_resourceManager->GetBackCullRasterizerState().Get());
		}

		/// Blend State
		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		if (bindState && bindState->m_hasBlender)
		{
			m_context->OMSetBlendState(bindState->m_blender.Get(), blendFactor, 0xffffffff);
		}
		else
		{
			m_context->OMSetBlendState(m_resourceManager->GetBlendState().Get(), blendFactor, 0xffffffff);
		}


		// Set Vertex, Index Buffer
		auto buffer = model->m_meshes[index];
		if (!buffer) assert(false && "Buffer 없음");
		ID3D11Buffer* vb = buffer->GetVertexBuffer().Get();
		UINT offset = buffer->GetVertexOffset();
		UINT stride = buffer->GetVertexStride();

		// Set Topology
		m_context->IASetPrimitiveTopology(buffer->GetTopology());
		m_context->IASetInputLayout(vertexShader->GetInputLayout().Get());
		m_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_context->IASetIndexBuffer(buffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		DRAW_CALL(m_context->DrawIndexed(buffer->GetIndexCount(), 0, 0));

		UnBind(textureArray.size());
	}
}

void RenderPath::TextDraw(GraphicsTransfer::Text textInfo)
{
	ComPtr<ID2D1RenderTarget> dtdRT = m_dxManager->m_d2dRenderTarget;
	IDWriteTextLayout* layout = 0;

	HRESULT hr;

	// 브러쉬 만들기
	auto brush = m_dxManager->CreateSolidColorBrush(textInfo.color);

	HR(m_dxManager->m_d2dWriteFactory->CreateTextLayout(
		textInfo.text.c_str(),
		static_cast<UINT32>(wcslen(textInfo.text.c_str())),
		m_dxManager->m_textFormat[textInfo.fontSize].Get(),
		textInfo.boxSize.x, textInfo.boxSize.y,
		&layout));

	dtdRT->BeginDraw();

	dtdRT->DrawTextLayout(
		D2D1::Point2F(textInfo.pos.x, textInfo.pos.y),
		layout,
		brush.Get());

	HR(dtdRT->EndDraw());

	layout->Release();
}

void RenderPath::UnBind(UINT textureCount)
{
	std::vector<ID3D11ShaderResourceView*> nullSRVs(textureCount, nullptr);
	m_context->PSSetShaderResources(0, textureCount, nullSRVs.data());
}

void RenderPath::UpdateVertexConstantBuffer(std::shared_ptr<Mesh> mesh, ModelData data, ComPtr<ID3D11Buffer> buffer, VertexCBufferType type)
{
	if (!buffer) assert(false && "상수버퍼 값 존재하지 않음");

	void* pCbuffer = nullptr;
	size_t bufferSize = 0;

	switch (type)
	{
		case VertexCBufferType::Transform:
		{
			pCbuffer = new CBuffer::Transform;
			bufferSize = sizeof(CBuffer::Transform);

			if (m_renderState == RenderPath::ShadowCasting)
			{
				CBuffer::Transform* cb = reinterpret_cast<CBuffer::Transform*>(pCbuffer);
				cb->world = data.transform.world.Transpose();
				cb->view = m_lightViewTM.Transpose();
				cb->proj = m_lightProjTM.Transpose();
				cb->wvp = DirectX::XMMatrixIdentity();
				cb->worldInv = DirectX::XMMatrixIdentity();
			}
			else
			{
				CBuffer::Transform* cb = reinterpret_cast<CBuffer::Transform*>(pCbuffer);
				cb->world = data.transform.world.Transpose();
				cb->view = data.transform.view.Transpose();
				cb->proj = data.transform.proj.Transpose();
				cb->wvp = (data.transform.world * data.transform.viewProj).Transpose();
				cb->worldInv = data.transform.world.Invert();
			}
			//cb->worldInv = data.transform.world.Invert().Transpose();

			break;
		}
		case VertexCBufferType::UITransform:
		{
			pCbuffer = new CBuffer::UITransform;
			bufferSize = sizeof(CBuffer::UITransform);

			CBuffer::UITransform* cb = reinterpret_cast<CBuffer::UITransform*>(pCbuffer);
			cb->world = data.uiTransform.world.Transpose();
			cb->worldViewProj = data.uiTransform.worldViewProj.Transpose();
			cb->worldInv = cb->world.Invert();

			break;
		}
		case VertexCBufferType::MiniGameUIMove:
		{
			pCbuffer = new CBuffer::MiniGameUIMove;
			bufferSize = sizeof(CBuffer::MiniGameUIMove);
			CBuffer::MiniGameUIMove* cb = reinterpret_cast<CBuffer::MiniGameUIMove*>(pCbuffer);

			cb->UIcenter = data.ui.pos;
			cb->UIsize = data.ui.size;
			cb->meshCount = data.ui.meshCount;
			cb->pad = Vector3(0.f, 0.f, 0.f);
		}
		break;
		case VertexCBufferType::TimerUI:
		{
			pCbuffer = new CBuffer::TimerUI;
			bufferSize = sizeof(CBuffer::TimerUI);
			CBuffer::TimerUI* cb = reinterpret_cast<CBuffer::TimerUI*>(pCbuffer);

			cb->pos = data.ui.pos;
			cb->size = data.ui.size;
			cb->angle = data.ui.textureTranslation;
			cb->pad = Vector3(0.f, 0.f, 0.f);
		}
		break;
		case VertexCBufferType::BonePalette:
		{
			pCbuffer = new CBuffer::BonePalette;
			bufferSize = sizeof(CBuffer::BonePalette);
			CBuffer::BonePalette* cb = reinterpret_cast<CBuffer::BonePalette*>(pCbuffer);

			for (int i = 0; i < 512; i++)
			{
				cb->gBoneTransforms[i] = mesh->m_boneTransforms[i];
			}

		}
		break;
		default:
			break;
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, pCbuffer, bufferSize);
	m_context->Unmap(buffer.Get(), NULL);

	delete pCbuffer;
}

void RenderPath::UpdatePixelConstantBuffer(ModelData data, ComPtr<ID3D11Buffer> buffer, PixelCBufferType type)
{
	if (!buffer) assert(false && "상수버퍼 값 존재하지 않음");

	void* pCbuffer = nullptr;
	size_t bufferSize = 0;

	switch (type)
	{
		case PixelCBufferType::TextureTransform:
		{
			pCbuffer = new CBuffer::TextureTransform;
			bufferSize = sizeof(CBuffer::TextureTransform);

			CBuffer::TextureTransform* cb = reinterpret_cast<CBuffer::TextureTransform*>(pCbuffer);
			cb->textureTranslation = data.ui.textureTranslation;
			cb->typeNum = data.ui.typeNum;
			cb->isFirst = data.ui.isFirst;
			cb->pad = 0.f;
		}
		break;
		case PixelCBufferType::PBRMaterial:
		{
			pCbuffer = new CBuffer::PBRMaterial;
			bufferSize = sizeof(CBuffer::PBRMaterial);

			CBuffer::PBRMaterial* cb = reinterpret_cast<CBuffer::PBRMaterial*>(pCbuffer);
			auto& mat = data.meterial;
			cb->useBaseColorMap = mat.useMap[0];
			cb->useNormalMap = mat.useMap[1];
			cb->useMetallicMap = mat.useMap[2];
			cb->useRoughnessMap = mat.useMap[3];
			cb->useAOMap = mat.useMap[4];
			cb->pad3 = { 0.f, 0.f, 0.f };
			cb->BaseColor = mat.baseColor;
			cb->metallic = mat.metallic;
			cb->roughness = mat.roughness;
			cb->AO = mat.AO;
			cb->pad = 0.f;
		}
		break;
		case PixelCBufferType::CollisionCheck:
		{
			pCbuffer = new CBuffer::CollisionCheck;
			bufferSize = sizeof(CBuffer::CollisionCheck);

			CBuffer::CollisionCheck* cb = reinterpret_cast<CBuffer::CollisionCheck*>(pCbuffer);
			cb->isCollision = data.physics.isCollision;
			cb->pad = { 0.f, 0.f, 0.f };
		}
		break;
		case PixelCBufferType::MiniGameButton:
		{
			pCbuffer = new CBuffer::MiniGameButton;
			bufferSize = sizeof(CBuffer::MiniGameButton);
			CBuffer::MiniGameButton* cb = reinterpret_cast<CBuffer::MiniGameButton*>(pCbuffer);

			cb->isActive = data.ui.isActive;
			cb->pad = Vector3(0.f, 0.f, 0.f);
		}
		break;
		case PixelCBufferType::MiniGameProgress:
		{
			pCbuffer = new CBuffer::MiniGameProgress;
			bufferSize = sizeof(CBuffer::MiniGameProgress);
			CBuffer::MiniGameProgress* cb = reinterpret_cast<CBuffer::MiniGameProgress*>(pCbuffer);

			cb->foodtype = data.ui.typeNum;
			cb->gametype = data.ui.typeNum2;
			cb->stage = data.ui.stage;
			cb->progress = data.ui.progress;
			cb->meshCount = data.ui.meshCount;
			cb->direction = data.ui.direction;
		}
		break;
		case PixelCBufferType::MiniGameKeyChain:
		{
			pCbuffer = new CBuffer::MiniGameKeyChain;
			bufferSize = sizeof(CBuffer::MiniGameKeyChain);
			CBuffer::MiniGameKeyChain* cb = reinterpret_cast<CBuffer::MiniGameKeyChain*>(pCbuffer);

			auto keyInfo = data.ui.keyChains;
			int keymove[5];
			std::copy(keyInfo.begin(), keyInfo.end(), keymove);
			std::fill(keymove + keyInfo.size(), keymove + 5, 2);

			cb->key1 = keymove[0];
			cb->key2 = keymove[1];
			cb->key3 = keymove[2];
			cb->key4 = keymove[3];
			cb->key5 = keymove[4];
			cb->pad = Vector3(0.f, 0.f, 0.f);
		}
		break;
		case PixelCBufferType::MainUI:
		{
			pCbuffer = new CBuffer::MainUI;
			bufferSize = sizeof(CBuffer::MainUI);
			CBuffer::MainUI* cb = reinterpret_cast<CBuffer::MainUI*>(pCbuffer);

			cb->onExit = data.ui.isActive;
			cb->onCredit = data.ui.isActive2;
			cb->fade = data.ui.fade;
		}
		break;
		case PixelCBufferType::MerchantUI:
		{
			pCbuffer = new CBuffer::MerchantUI;
			bufferSize = sizeof(CBuffer::MerchantUI);
			CBuffer::MerchantUI* cb = reinterpret_cast<CBuffer::MerchantUI*>(pCbuffer);

			cb->state = data.ui.typeNum;
			cb->foodtype = data.ui.typeNum2;
			cb->ratio = data.ui.textureTranslation;
		}
		break;
		case PixelCBufferType::EatSpotUI:
		{
			pCbuffer = new CBuffer::EatSpotUI;
			bufferSize = sizeof(CBuffer::EatSpotUI);
			CBuffer::EatSpotUI* cb = reinterpret_cast<CBuffer::EatSpotUI*>(pCbuffer);

			auto foodInfo = data.ui.eatFoods;
			int eatFood[5];
			std::copy(foodInfo.begin(), foodInfo.end(), eatFood);
			std::fill(eatFood + foodInfo.size(), eatFood + 5, false);

			cb->meat = eatFood[0];
			cb->crab = eatFood[1];
			cb->apple = eatFood[2];
			cb->cupcake = eatFood[3];
			cb->baguette = eatFood[4];
			cb->count = data.ui.meshCount;
		}
		break;
		default:
			break;
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, pCbuffer, bufferSize);
	m_context->Unmap(buffer.Get(), NULL);

	delete pCbuffer;
}

void RenderPath::BindLightCBuffer()
{
	// 디렉셔널 라이트 ------------------------------------------------------------------
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		HRESULT hr;
		HR(m_context->Map(m_lightCBuffer_Directional.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource));
		DirectionalLightData* bufferData = static_cast<DirectionalLightData*>(subResource.pData);
		*bufferData = m_directionalLight;

		m_context->Unmap(m_lightCBuffer_Directional.Get(), 0);
		m_context->PSSetConstantBuffers(0 /*slot*/, 1, m_lightCBuffer_Directional.GetAddressOf());
	}

	// 포인트 라이트 ------------------------------------------------------------------
	{
		for (int i = 0; i < m_pointLights.size(); i++)
		{
			if (i == 64) break;
			m_pointCb.points[i] = m_pointLights[i];
		}

		m_pointCb.count = m_pointLights.size();
		if (m_pointLights.size() > 64) m_pointCb.count = 64;
		m_pointLights.clear();

		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		HRESULT hr;
		HR(m_context->Map(m_lightCBuffer_Points.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource));
		PointCb* bufferData = static_cast<PointCb*>(subResource.pData);
		std::memcpy(bufferData, &m_pointCb, sizeof(PointCb)); // 구조체 복사

		m_context->Unmap(m_lightCBuffer_Points.Get(), 0);
		m_context->PSSetConstantBuffers(1 /*slot*/, 1, m_lightCBuffer_Points.GetAddressOf());
	}
}

void RenderPath::BindFrameCBuffer(UINT slot, UINT invSize)
{
	m_frameCb.Frame.x = (float)m_textureWidth / invSize;
	m_frameCb.Frame.y = (float)m_textureHeight / invSize;
	m_frameCb.Frame.z = 1.f / m_frameCb.Frame.x;
	m_frameCb.Frame.w = 1.f / m_frameCb.Frame.y;

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	HRESULT hr;
	HR(m_context->Map(m_frameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource));
	FrameCb* bufferData = static_cast<FrameCb*>(subResource.pData);
	*bufferData = m_frameCb;

	m_context->Unmap(m_frameBuffer.Get(), 0);
	m_context->PSSetConstantBuffers(slot, 1, m_frameBuffer.GetAddressOf());
}

void RenderPath::BindDownSampleCBuffer(UINT invSize)
{
	m_downSampleCb.Scale = invSize;

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	HRESULT hr;
	HR(m_context->Map(m_downSampleBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource));
	DownSampleCb* bufferData = static_cast<DownSampleCb*>(subResource.pData);
	*bufferData = m_downSampleCb;

	m_context->Unmap(m_downSampleBuffer.Get(), 0);
	m_context->VSSetConstantBuffers(0, 1, m_downSampleBuffer.GetAddressOf());
}

void RenderPath::BindWorldProperty(UINT slot)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	HRESULT hr;
	HR(m_context->Map(m_worldPropertyBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource));
	LightWorldProperty* bufferData = static_cast<LightWorldProperty*>(subResource.pData);
	*bufferData = m_worldPropertyCb;

	m_context->Unmap(m_worldPropertyBuffer.Get(), 0);
	m_context->PSSetConstantBuffers(slot, 1, m_worldPropertyBuffer.GetAddressOf());
}

void RenderPath::CalcLightTM(Vector3 centerPosition)
{
	//mSceneBounds.Center = Vector3(0.0f, 0.0f, 0.0f);
	m_sceneBounds.Center = centerPosition;
	m_sceneBounds.Radius = sqrtf(10.0f * 10.0f + 15.0f * 15.0f);
	float shadowExpansionFactor = 1.0f; // 조절할 비율
	m_sceneBounds.Radius *= shadowExpansionFactor;
	// Only the first "main" light casts a shadow.

	Vector3 lightDir = Vector3(m_directionalLight.direction.x, m_directionalLight.direction.y, m_directionalLight.direction.z);
	//DirectX::XMVECTOR lightPos = -2.0f * m_sceneBounds.Radius * lightDir;
	DirectX::XMVECTOR lightPos = Vector3(m_sceneBounds.Center) - 2.0f * m_sceneBounds.Radius * lightDir;
	DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&m_sceneBounds.Center);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_lightViewTM = DirectX::XMMatrixLookAtLH(lightPos, targetPos, up);

	// Transform bounding sphere to light space.
	DirectX::XMFLOAT3 sphereCenterLS;
	DirectX::XMStoreFloat3(&sphereCenterLS, DirectX::XMVector3TransformCoord(targetPos, m_lightViewTM));

	// Ortho frustum in light space encloses scene.
// 	float l = sphereCenterLS.x - mSceneBounds.Radius;
// 	float b = sphereCenterLS.y - mSceneBounds.Radius;
// 	float n = sphereCenterLS.z - mSceneBounds.Radius;
// 	float r = sphereCenterLS.x + mSceneBounds.Radius;
// 	float t = sphereCenterLS.y + mSceneBounds.Radius;
// 	float f = sphereCenterLS.z + mSceneBounds.Radius;

	float horizontalScale = 2.0f;
	float verticalScale = 2.0f;

	float l = sphereCenterLS.x - horizontalScale * m_sceneBounds.Radius;
	float r = sphereCenterLS.x + horizontalScale * m_sceneBounds.Radius;
	float b = sphereCenterLS.y - verticalScale * m_sceneBounds.Radius;
	float t = sphereCenterLS.y + verticalScale * m_sceneBounds.Radius;
	float n = sphereCenterLS.z - m_sceneBounds.Radius;
	float f = sphereCenterLS.z + m_sceneBounds.Radius;


	m_lightProjTM = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
}

