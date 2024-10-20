#pragma once
#include "IDevice.h"
#include <d3d11.h>

#include <wrl/client.h> //COM 스마트포인터 사용
using namespace Microsoft::WRL;

/// <summary>
/// 디바이스 인터페이스를 상속받은
/// DX11용 Device
/// 
/// 24.01.23
/// 박연하
/// </summary>
class Device : public IDevice
{
public:
	void CreateDevice();
	void Finalize();

public:
	ComPtr<ID3D11Device> GetDevice() { return m_device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_context; }

private:
	ComPtr<ID3D11Device>			m_device;
	ComPtr<ID3D11DeviceContext>		m_context;
};

