#include "Device.h"

#include <d3d11.h>
#include <cassert>

void Device::CreateDevice()
{
	// D2D�� DX11�� ���� ������ �� flag�� �ʿ��ϴ�.
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDevice(
		0,								// �⺻ �����
		D3D_DRIVER_TYPE_HARDWARE,		// 3���� �׷��� ���� ���
		0,								// ����Ʈ���� ��ġ�� ������� ����
		createDeviceFlags,				// ��ġ ���� �÷��� ����
		0, 0,							// �⺻ ��� ���� �迭(�׻� LEVEL 11�� ���õǰ� ��)
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
