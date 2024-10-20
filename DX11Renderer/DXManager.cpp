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
	//�̰� �̸� �����ָ� ���ܰ� �߻��Ѵ�.
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

	InitializeDXGIFactory(numerator, denominater);	// ��������ȭ�� ���� �ػ� ���� ������
	InitializeSwapChain(numerator, denominater);	// ����ü�� ����
	InitializeRenderTargetView();					// ����Ÿ�� �� ����
	InitializeDepthBuffer();						// ���� ���ۿ� �ؽ��� ����
	InitializeDepthStencilBuffer();					// ����&���ٽ� ����
	InitializeStencilView();						// ���� ���� �� ����
	m_device->GetDeviceContext()->OMSetRenderTargets(	// ����� ��� ���ձ� �ܰ迡 ����
		1, m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get());
	InitializeRasterizerState();					// �����Ͷ����� �ܰ� ����
	InitializeViewPort();							// ����Ʈ ����
	InitializeSamplerState();						// ���÷� ���� ����
	InitializeD2D();								// D2D �ʱ�ȭ
	
	// ��Ʈ �ҷ����� 
// 	const std::wstring& path = L"..\\Resource\\Font\\SEOLLEIMcool.ttf";
// 	//result = AddFontResourceExW(path.c_str(), FR_PRIVATE, NULL);
// 	AddFontResource(path.c_str());
// 	::SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);


#ifdef DEBUG_MODE
	ErrorLogger::GetInstance()->Initialize();
#endif

	// �⺻ ���� ����
	m_device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ȭ�� �ʱ�ȭ
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
	InitializeDXGIFactory(numerator, denominater);	// ��������ȭ�� ���� �ػ� ���� ������
	InitializeSwapChain(numerator, denominater);	// ����ü�� ����

	InitializeRenderTargetView();
	InitializeDepthBuffer();
	InitializeStencilView();

	InitializeD2D();

	// ����Ʈ ����
	InitializeViewPort();
}

void DXManager::ResetViewport()
{
	m_device->GetDeviceContext()->RSSetViewports(1, &m_viewport);
}

void DXManager::CreateTextFormat(wstring name, UINT size)
{
	ComPtr<IDWriteTextFormat> format;

	// ����Ʈ �ؽ�Ʈ ������ �����
	HRESULT hr;
	HR(m_d2dWriteFactory->CreateTextFormat(
		L"�ÿ��� ������",					// ��Ʈ �йи� �̸��� ���ڿ�
		NULL,							// ��Ʈ �÷��� ��ü, NULL=�ý��� ��Ʈ �÷���
		DWRITE_FONT_WEIGHT_SEMI_BOLD,	// ��Ʈ ����. LIGHT, NORMAL, BOLD ��.
		DWRITE_FONT_STYLE_NORMAL,		// ��Ʈ ��Ÿ��. NORMAL, OBLIQUE, ITALIC.
		DWRITE_FONT_STRETCH_NORMAL,		// ��Ʈ ����. CONDENSED, NORMAL, MEDIUM, EXTEXDED ��.
		size,								// ��Ʈ ũ��.
		L"ko-kr",						// �������� ���ڿ��� ���.  ����-�̱�=L"en-us", �ѱ���-�ѱ�=L"ko-kr"
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

/// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
///                                               ������� �ʱ�ȭ �Լ� 
/// �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

void DXManager::InitializeDXGIFactory(unsigned int& numerator, unsigned int& denominater)
{
	HRESULT hr;
	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIOutput> adapterOutput;
	unsigned int numModes = 0;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	// dx11 �׷��� �������̽� ���丮�� �����.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_factory.GetAddressOf());
	if (FAILED(hr))		assert(false && L"DXGI���丮 �ʱ�ȭ ����");

	// factory�� �⺻ �׷��� �������̽��� ���� adapter�� �����
	hr = m_factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(hr))		assert(false && L"DXGI���丮 �ʱ�ȭ ����");

	// �⺻ adapter ��� ����
	hr = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if (FAILED(hr))		assert(false && L"DXGI���丮 �ʱ�ȭ ����");

	// ����� ��¿� ���� DXGI_FORMAT_R8G8B8A_UNORM ǥ�� ���Ŀ� �´� ��� ���� �����´�
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))		assert(false && L"DXGI���丮 �ʱ�ȭ ����");

	// �����/���� ī�� ���տ� ���� ������ ��� ��带 ���� ����� ����ϴ�.
	displayModeList = new DXGI_MODE_DESC[numModes];

	// display mode list�� ä���
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))		assert(false && L"display mode list ������ ����");


	for (UINT i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (UINT)m_screenWidth
			&& displayModeList[i].Height == (UINT)m_screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominater = displayModeList[i].RefreshRate.Denominator;
		}
	}

	/// 2024.02.01 23:39 ����
	/// ��Ʈ�Ͽ��� 1920 1080 �����ϴµ� m_screenHeight�� 1061�̶� assert�� �ɷȴ�.
	/// ����������� ����ü �� ������ �ʾ�����? 

	if (numerator == 0 && denominater == 0)
	{
		m_isVsyncEnabled = false;
		//assert(false && L"��������ȭ �ػ󵵰� �������� ����");
	}

	//adapter description�� �����´�.
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))		assert(false && L"DXGI���丮 �ʱ�ȭ ����");

	// �׷���ī�� �Ÿ𸮸� �����´�	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷���ī�� �̸��� ���� �����Ѵ�.
	m_videoCardDescription = std::wstring(adapterDesc.Description);

	// �޸� ����
	delete[] displayModeList;
}

void DXManager::InitializeSwapChain(const unsigned int& numerator, const unsigned int& denominator)
{
	/// SwapChain ����
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	HRESULT hr;

	// ����ä�� �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_screenWidth;
	swapChainDesc.BufferDesc.Height = m_screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	//��������ȭ ����
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

	swapChainDesc.SampleDesc.Count = 1;		//��Ƽ ���ø� off
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = !m_isFullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;


	/// ����ü�� ����--------------------------------

	hr = m_factory->CreateSwapChain(
		m_device->GetDevice().Get(),
		&swapChainDesc,
		m_swapChain.GetAddressOf()
	);
	
	if (FAILED(hr))	assert(false && L"����ü�� �ʱ�ȭ ����");

}

void DXManager::InitializeRenderTargetView()
{
	HRESULT hr;

	// ����� ������ �޾ƿ���
	ComPtr<ID3D11Texture2D> backBufferPtr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf());
	if (FAILED(hr))		assert(false && L"����� ������ �޾ƿ��� ����");

	// ����Ÿ�ٺ� ����
	hr = m_device->GetDevice()->CreateRenderTargetView(backBufferPtr.Get(), 0, m_renderTargetView.GetAddressOf());
	if (FAILED(hr))		assert(false && L"����Ÿ�ٺ� ���� ����");
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

	if (FAILED(hr))		assert(false && L"Depth Buffer ���� ����");
}

void DXManager::InitializeDepthStencilBuffer()
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// ���� ���°� ����
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //ť����� ����

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//�ȼ��� front-face�� �� stencil�� �ൿ
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//�ȼ��� back-face�� �� stencil�� �ൿ
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	/// ����&���ٽ� State ���� �� bind
	ComPtr<ID3D11DepthStencilState> DSState;
	hr = m_device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, DSState.GetAddressOf());
	m_device->GetDeviceContext()->OMSetDepthStencilState(DSState.Get(), 1u);

	if (FAILED(hr))		assert(false && L"DepthStencil���� ���� ����");
}

void DXManager::InitializeStencilView()
{
	HRESULT hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	//desc �ʱ�ȭ
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //���� ���ۿ� �ؽ��ķ� ���ƾ� �Ѵ�.
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	// depth Stencil View  ����
	hr = m_device->GetDevice()->CreateDepthStencilView(
		m_depthStencilBuffer.Get(),   // �ʱ�ȭ�� ������ ��� �ȴ�
		&depthStencilViewDesc,
		m_depthStencilView.GetAddressOf());

	if (FAILED(hr))		assert(false && L"depth Stencil View ���� ����");
}

void DXManager::InitializeRasterizerState()
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC rasterDesc;

	// Desc�ʱ�ȭ  
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;		//� ���� �����Ұ��ΰ�? NONE, FRONT, BACK
	rasterDesc.DepthBias = 0;					//���̰� ����(�׸��� �� ������Ʈ ��ħ�� ���) �����ϼ��� �ڷΰ���
	rasterDesc.DepthBiasClamp = 0.0f;			//depthBias�� �ʹ� ū ��츦 ����
	rasterDesc.DepthClipEnable = true;			//���� Ŭ������ Ȱ��ȭ �Ǵ� ��Ȱ��ȭ
	rasterDesc.FillMode = D3D11_FILL_SOLID;		//D3D11_FILL_WIREFRAME
	rasterDesc.FrontCounterClockwise = false;	// true : ������ �ð� ����, false : ������ �ð�ݴ�
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;			//Ŭ���� �簢���� ����Ͽ� Ŭ����
	rasterDesc.SlopeScaledDepthBias = 0.0f;		//��� ����� ���� ���̾ ����. 

	// �����Ͷ����� �ܰ� ����
	ComPtr<ID3D11RasterizerState> cullBack;
	hr = m_device->GetDevice()->CreateRasterizerState(&rasterDesc, cullBack.GetAddressOf());
	if (FAILED(hr))		assert(false && L"Rasterizer State ���� ����");

	// �����Ͷ����� ����
	m_device->GetDeviceContext()->RSSetState(cullBack.Get());
}

void DXManager::InitializeViewPort()
{
	//�ĸ鿵�� ��ü�� �׸���
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

	// ���� ���°� ����
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//�ȼ��� front-face�� �� stencil�� �ൿ
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//�ȼ��� back-face�� �� stencil�� �ൿ
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create disable state
	hr = m_device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, m_depthDisabledStensilState.GetAddressOf());
	if (FAILED(hr))		assert(false && L"ZBuffer ���� ����");
}

void DXManager::InitializeD2D()
{
	HRESULT hr;

	///2d2��. �����۸� �޾ƿͼ� ����Ÿ���� ����������
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_dxgiSurface.GetAddressOf()));
	if (FAILED(hr))		assert(false && L"d2d ����� �޾ƿ��� ����");

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_d2dFactory.GetAddressOf());
	if (FAILED(hr))		assert(false && L"d2d factory ���� ����");

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
	if (FAILED(hr))		assert(false && L"d2d surfaceRenderTarget ���� ����");

	//Direct2D�� ����Ϸ��� IDXGISurface�� �����ϴ� Direct3D ����̽��� 
	//D3D10_CREATE_DEVICE_BGRA_SUPPORT �÷��׷� ������ �մϴ�.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_d2dWriteFactory),
		reinterpret_cast<IUnknown**>(m_d2dWriteFactory.GetAddressOf()));
	if (FAILED(hr))		assert(false && L"d2d write factory ���� ����");
}

void DXManager::InitializeSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT hr;

	// ���÷� desc �����
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

	// ���÷�  state �����
	hr = m_device->GetDevice()->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
	if (FAILED(hr)) assert(false && L"Sampler State ���� ����");

	// ���÷� ���� ����
	m_device->GetDeviceContext()->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}