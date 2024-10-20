#pragma once
#include "GraphicsTransfer.h"

class Device;

using namespace GraphicsTransfer;

/// <summary>
/// DX11 �ʱ�ȭ �� �ڿ��� ������ �ִ� Ŭ����
/// �ռ���, �ڿ���
/// </summary>
class DXManager
{
// �����ڿ� �Ҹ��� ---------------------------------------------------------------------------------
public:
	DXManager(std::shared_ptr<Device> device);
	~DXManager();
	void Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
	void Finalize();

	void ResizeStart();
	void ResizeEnd(int width, int height);
	void ResetViewport();

	void CreateTextFormat(wstring name, UINT size);
	ComPtr<ID2D1SolidColorBrush> CreateSolidColorBrush(Vector4 color);

// �����̺� �Լ� ------------------------------------------------------------------------------------
private:
	void InitializeDXGIFactory(unsigned int& numorator, unsigned int& denominator);
	void InitializeSwapChain(const unsigned int& numerator, const unsigned int& denominator);
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeDepthStencilBuffer();
	void InitializeStencilView();
	void InitializeRasterizerState();
	void InitializeViewPort();
	void InitializeZBuffer();
	void InitializeD2D();
	void InitializeSamplerState();

// ��� ���� ----------------------------------------------------------------------------------------
public:
	HWND m_hwnd;

	int m_videoCardMemory;
	std::wstring m_videoCardDescription;

	bool m_isVsyncEnabled;			// ��������ȭ
	bool m_isFullScreen;
	int m_screenWidth;
	int m_screenHeight;
	D3D11_VIEWPORT m_viewport;

	shared_ptr<Device> m_device;

	ComPtr<IDXGISwapChain>			m_swapChain;
	ComPtr<IDXGIFactory>			m_factory;
	ComPtr<ID3D11RenderTargetView>	m_renderTargetView;
	ComPtr<ID3D11Texture2D>			m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView>	m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	ComPtr<ID3D11BlendState>		m_alphaEnableBlendingState;
	ComPtr<ID3D11BlendState>		m_alphaDisabledBlendingState;
	ComPtr<ID3D11DepthStencilState> m_depthDisabledStensilState;
	ComPtr<ID3D11SamplerState>		m_samplerState;
	
	//d2d
	ComPtr<IDXGISurface> m_dxgiSurface;
	ComPtr<ID2D1Factory> m_d2dFactory;
	ComPtr<IDWriteFactory> m_d2dWriteFactory;
	ComPtr<ID2D1RenderTarget> m_d2dRenderTarget;

	std::unordered_map<UINT, ComPtr<IDWriteTextFormat>> m_textFormat;
};

