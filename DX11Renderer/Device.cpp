#include "Device.h"

#include <d3d11.h>
#include <cassert>

void Device::CreateDevice()
{
	// D2D와 DX11을 같이 쓰려면 이 flag가 필요하다.
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDevice(
		0,								// 기본 어댑터
		D3D_DRIVER_TYPE_HARDWARE,		// 3차원 그래픽 가속 사용
		0,								// 소프트웨어 장치를 사용하지 않음
		createDeviceFlags,				// 장치 생성 플래그 지정
		0, 0,							// 기본 기능 수준 배열(항상 LEVEL 11이 선택되게 함)
		D3D11_SDK_VERSION,
		m_device.GetAddressOf(),
		&featureLevel,
		m_context.GetAddressOf()
	);

	if (FAILED(hr))
	{
		assert(false && "D3D11CreateDevice Failed");
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		assert(false && "Direct3D Feature Level 11 unsupported");
	}
}

void Device::Finalize()
{
// 	m_device->Release();
// 	m_context->Release();
}
