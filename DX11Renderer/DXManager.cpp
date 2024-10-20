#include "renderpch.h"
#include "ErrorLogger.h"
#include "DXManager.h"
#include "Device.h"
#include "DebugMacro.h"

#ifndef DEBUG_MODE
#define _DEBUG
#endif

DXManager::DXManager(std::shared_ptr<Device> device)
	: m_isVsyncEnabled(true), m_videoCardMemory(0), m_videoCardDescription()
	, m_screenHeight(0), m_screenWidth(0), m_hwnd(0), m_isFullScreen(false)
	, m_swapChain(nullptr), m_renderTargetView(nullptr)
	, m_depthStencilBuffer(nullptr), m_depthStencilState(nullptr), m_depthStencilView(nullptr)
	,  m_alphaEnableBlendingState(nullptr), m_alphaDisabledBlendingState(nullptr), m_depthDisabledStensilState(nullptr)
	, m_d2dFactory(nullptr), m_d2dRenderTarget(nullptr), m_d2dWriteFactory(nullptr), m_dxgiSurface(nullptr)
	, m_samplerState(nullptr), m_device(device), m_viewport()
{

}

DXManager::~DXManager()
{
	//이걸 미리 안해주면 예외가 발생한다.
// 	if (m_swapChain) 
// 		m_swapChain->SetFullscreenState(false, NULL);
}

void DXManager::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
{
	m_isVsyncEnabled = vsync;
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;
	m_hwnd = hwnd;
	m_isFullScreen = fullscreen;

	unsigned int numerator = 0, denominater = 0;

	InitializeDXGIFactory(numerator, denominater);	// 수직동기화를 위한 해상도 정보 얻어오기
	InitializeSwapChain(numerator, denominater);	// 스왑체인 생성
	InitializeRenderTargetView();					// 랜더타겟 뷰 생성
	InitializeDepthBuffer();						// 깊이 버퍼용 텍스쳐 생성
	InitializeDepthStencilBuffer();					// 깊이&스텐실 생성
	InitializeStencilView();						// 깊이 버퍼 뷰 생성
	m_device->GetDeviceContext()->OMSetRenderTargets(	// 뷰들을 출력 병합기 단계에 묶기
		1, m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get());
	InitializeRasterizerState();					// 레스터라이즈 단계 설정
	InitializeViewPort();							// 뷰포트 설정
	InitializeSamplerState();						// 샘플러 상태 설정
	InitializeD2D();								// D2D 초기화
	
	// 폰트 불러오기 
// 	const std::wstring& path = L"..\\Resource\\Font\\SEOLLEIMcool.ttf";
// 	//result = AddFontResourceExW(path.c_str(), FR_PRIVATE, NULL);
// 	AddFontResource(path.c_str());
// 	::SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);


#ifdef DEBUG_MODE
	ErrorLogger::GetInstance()->Initialize();
#endif

	// 기본 도형 설정
	m_device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 화면 초기화
	//float color[] = { 0.78f,0.886f,0.929f,1.f };
	float color[] = { 0,0,0,1.f };
	m_device->GetDeviceContext()->ClearRenderTargetView(m_renderTargetView.Get(), color);
	m_swapChain->Present(0, 0);
}

void DXManager::Finalize()
{
	const std::wstring& path = L"..\\Resource\\Font\\SEOLLEIMcool.ttf";
	RemoveFontResourceExW(path.c_str(), FR_PRIVATE, NULL);
}

void DXManager::ResizeStart()
{
	m_device->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);

	if (m_renderTargetView.Get())	m_renderTargetView->Release();
	if (m_depthStencilBuffer.Get()) m_depthStencilBuffer->Release();
	if (m_depthStencilView.Get())	m_depthStencilView->Release();
	
	if (m_dxgiSurface.Get()) m_dxgiSurface->Release();
	if (m_d2dFactory.Get()) m_d2dFactory->Release();
	if (m_d2dWriteFactory.Get()) m_d2dWriteFactory->Release();
	if (m_d2dRenderTarget.Get()) m_d2dRenderTarget->Release();

	m_textFormat.clear();
}

void DXManager::ResizeEnd(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;

	m_swapChain->Release();
	unsigned int numerator = 0, denominater = 0;
	InitializeDXGIFactory(numerator, denominater);	// 수직동기화를 위한 해상도 정보 얻어오기
	InitializeSwapChain(numerator, denominater);	// 스왑체인 생성

	InitializeRenderTargetView();
	InitializeDepthBuffer();
	InitializeStencilView();

	InitializeD2D();

	// 뷰포트 셋팅
	InitializeViewPort();
}

void DXManager::ResetViewport()
{
	m_device->GetDeviceContext()->RSSetViewports(1, &m_viewport);
}

void DXManager::CreateTextFormat(wstring name, UINT size)
{
	ComPtr<IDWriteTextFormat> format;

	// 디폴트 텍스트 포멧을 만든다
	HRESULT hr;
	HR(m_d2dWriteFactory->CreateTextFormat(
		L"시원한 설레임",					// 폰트 패밀리 이름의 문자열
		NULL,							// 폰트 컬렉션 객체, NULL=시스템 폰트 컬렉션
		DWRITE_FONT_WEIGHT_SEMI_BOLD,	// 폰트 굵기. LIGHT, NORMAL, BOLD 등.
		DWRITE_FONT_STYLE_NORMAL,		// 폰트 스타일. NORMAL, OBLIQUE, ITALIC.
		DWRITE_FONT_STRETCH_NORMAL,		// 폰트 간격. CONDENSED, NORMAL, MEDIUM, EXTEXDED 등.
		size,								// 폰트 크기.
		L"ko-kr",						// 로케일을 문자열로 명시.  영어-미국=L"en-us", 한국어-한국=L"ko-kr"
		format.GetAddressOf()
	));

	m_textFormat[size] = format;
}

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> DXManager::CreateSolidColorBrush(Vector4 color)
{
	ComPtr<ID2D1SolidColorBrush> brush;
	m_d2dRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(color.x, color.y, color.z, color.w),
		brush.GetAddressOf());

	return brush;
}

/// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
///                                               여기부터 초기화 함수 
/// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

void DXManager::InitializeDXGIFactory(unsigned int& numerator, unsigned int& denominater)
{
	HRESULT hr;
	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIOutput> adapterOutput;
	unsigned int numModes = 0;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	// dx11 그래픽 인터페이스 팩토리를 만든다.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_factory.GetAddressOf());
	if (FAILED(hr))		assert(false && L"DXGI팩토리 초기화 실패");

	// factory로 기본 그래픽 인터페이스를 위한 adapter를 만든다
	hr = m_factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(hr))		assert(false && L"DXGI팩토리 초기화 실패");

	// 기본 adapter 출력 열거
	hr = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if (FAILED(hr))		assert(false && L"DXGI팩토리 초기화 실패");

	// 어댑터 출력에 대한 DXGI_FORMAT_R8G8B8A_UNORM 표시 형식에 맞는 모드 수를 가져온다
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))		assert(false && L"DXGI팩토리 초기화 실패");

	// 모니터/비디오 카드 조합에 대해 가능한 모든 모드를 담을 목록을 만듭니다.
	displayModeList = new DXGI_MODE_DESC[numModes];

	// display mode list를 채운다
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))		assert(false && L"display mode list 얻어오기 실패");


	for (UINT i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (UINT)m_screenWidth
			&& displayModeList[i].Height == (UINT)m_screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominater = displayModeList[i].RefreshRate.Denominator;
		}
	}

	/// 2024.02.01 23:39 동일
	/// 노트북에서 1920 1080 지원하는데 m_screenHeight가 1061이라서 assert에 걸렸다.
	/// 인재원에서는 도대체 왜 터지지 않았을까? 

	if (numerator == 0 && denominater == 0)
	{
		m_isVsyncEnabled = false;
		//assert(false && L"수직동기화 해상도가 적절하지 않음");
	}

	//adapter description을 가져온다.
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))		assert(false && L"DXGI팩토리 초기화 실패");

	// 그래픽카드 매모리를 가져온다	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽카드 이름을 따로 저장한다.
	m_videoCardDescription = std::wstring(adapterDesc.Description);

	// 메모리 해제
	delete[] displayModeList;
}

void DXManager::InitializeSwapChain(const unsigned int& numerator, const unsigned int& denominator)
{
	/// SwapChain 설정
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	HRESULT hr;

	// 스왑채인 초기화
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_screenWidth;
	swapChainDesc.BufferDesc.Height = m_screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	//수직동기화 설정
	if (m_isVsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_hwnd;

	swapChainDesc.SampleDesc.Count = 1;		//멀티 샘플링 off
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = !m_isFullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;


	/// 스왑체인 생성--------------------------------

	hr = m_factory->CreateSwapChain(
		m_device->GetDevice().Get(),
		&swapChainDesc,
		m_swapChain.GetAddressOf()
	);
	
	if (FAILED(hr))	assert(false && L"스왑체인 초기화 실패");

}

void DXManager::InitializeRenderTargetView()
{
	HRESULT hr;

	// 백버퍼 포인터 받아오기
	ComPtr<ID3D11Texture2D> backBufferPtr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf());
	if (FAILED(hr))		assert(false && L"백버퍼 포인터 받아오기 실패");

	// 랜더타겟뷰 생성
	hr = m_device->GetDevice()->CreateRenderTargetView(backBufferPtr.Get(), 0, m_renderTargetView.GetAddressOf());
	if (FAILED(hr))		assert(false && L"랜더타겟뷰 생성 실패");
}

void DXManager::InitializeDepthBuffer()
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = m_screenWidth;
	depthBufferDesc.Height = m_screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //DXGI_FORMAT_D24_UNORM_S8_UINT; DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1u;
	depthBufferDesc.SampleDesc.Quality = 0u;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = m_device->GetDevice()->CreateTexture2D(&depthBufferDesc, nullptr, m_depthStencilBuffer.GetAddressOf());

	if (FAILED(hr))		assert(false && L"Depth Buffer 생성 실패");
}

void DXManager::InitializeDepthStencilBuffer()
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 깊이 상태값 설정
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //큐브맵을 위한

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//픽셀이 front-face일 때 stencil의 행동
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//픽셀이 back-face일 때 stencil의 행동
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	/// 깊이&스텐실 State 생성 및 bind
	ComPtr<ID3D11DepthStencilState> DSState;
	hr = m_device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, DSState.GetAddressOf());
	m_device->GetDeviceContext()->OMSetDepthStencilState(DSState.Get(), 1u);

	if (FAILED(hr))		assert(false && L"DepthStencil버퍼 생성 실패");
}

void DXManager::InitializeStencilView()
{
	HRESULT hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	//desc 초기화
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //깊이 버퍼용 텍스쳐량 같아야 한다.
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	// depth Stencil View  생성
	hr = m_device->GetDevice()->CreateDepthStencilView(
		m_depthStencilBuffer.Get(),   // 초기화가 끝나면 없어도 된다
		&depthStencilViewDesc,
		m_depthStencilView.GetAddressOf());

	if (FAILED(hr))		assert(false && L"depth Stencil View 생성 실패");
}

void DXManager::InitializeRasterizerState()
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC rasterDesc;

	// Desc초기화  
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;		//어떤 면을 제거할것인가? NONE, FRONT, BACK
	rasterDesc.DepthBias = 0;					//깊이값 조절(그림자 및 오브젝트 겹침에 사용) 음수일수록 뒤로간다
	rasterDesc.DepthBiasClamp = 0.0f;			//depthBias가 너무 큰 경우를 방지
	rasterDesc.DepthClipEnable = true;			//깊이 클리핑을 활성화 또는 비활성화
	rasterDesc.FillMode = D3D11_FILL_SOLID;		//D3D11_FILL_WIREFRAME
	rasterDesc.FrontCounterClockwise = false;	// true : 정면은 시계 방향, false : 정면은 시계반대
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;			//클리핑 사각형을 사용하여 클리핑
	rasterDesc.SlopeScaledDepthBias = 0.0f;		//경사 기반의 깊이 바이어스 적용. 

	// 래스터라이즈 단계 생성
	ComPtr<ID3D11RasterizerState> cullBack;
	hr = m_device->GetDevice()->CreateRasterizerState(&rasterDesc, cullBack.GetAddressOf());
	if (FAILED(hr))		assert(false && L"Rasterizer State 생성 실패");

	// 래스터라이저 설정
	m_device->GetDeviceContext()->RSSetState(cullBack.Get());
}

void DXManager::InitializeViewPort()
{
	//후면영역 전체에 그리기
	m_viewport = {};

	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = static_cast<float>(m_screenWidth);
	m_viewport.Height = static_cast<float>(m_screenHeight);
	m_viewport.MinDepth = 0.f;   // 
	m_viewport.MaxDepth = 1.f;   // 

	m_device->GetDeviceContext()->RSSetViewports(1, &m_viewport);
}

void DXManager::InitializeZBuffer()
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 깊이 상태값 설정
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//픽셀이 front-face일 때 stencil의 행동
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//픽셀이 back-face일 때 stencil의 행동
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create disable state
	hr = m_device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, m_depthDisabledStensilState.GetAddressOf());
	if (FAILED(hr))		assert(false && L"ZBuffer 생성 실패");
}

void DXManager::InitializeD2D()
{
	HRESULT hr;

	///2d2용. 벡버퍼를 받아와서 랜더타겟을 설정해주자
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_dxgiSurface.GetAddressOf()));
	if (FAILED(hr))		assert(false && L"d2d 백버퍼 받아오기 실패");

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_d2dFactory.GetAddressOf());
	if (FAILED(hr))		assert(false && L"d2d factory 생성 실패");

	int dpi = GetDpiForSystem();

	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			static_cast<FLOAT>(dpi),
			static_cast<FLOAT>(dpi)
		);

	hr = m_d2dFactory->CreateDxgiSurfaceRenderTarget(
		m_dxgiSurface.Get(),
		&props,
		m_d2dRenderTarget.GetAddressOf()
	);
	if (FAILED(hr))		assert(false && L"d2d surfaceRenderTarget 생성 실패");

	//Direct2D를 사용하려면 IDXGISurface를 제공하는 Direct3D 디바이스를 
	//D3D10_CREATE_DEVICE_BGRA_SUPPORT 플래그로 만들어야 합니다.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_d2dWriteFactory),
		reinterpret_cast<IUnknown**>(m_d2dWriteFactory.GetAddressOf()));
	if (FAILED(hr))		assert(false && L"d2d write factory 생성 실패");
}

void DXManager::InitializeSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT hr;

	// 샘플러 desc 만들기
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;  //?
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 샘플러  state 만들기
	hr = m_device->GetDevice()->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
	if (FAILED(hr)) assert(false && L"Sampler State 생성 실패");

	// 샘플러 상태 설정
	m_device->GetDeviceContext()->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}