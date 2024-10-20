#pragma once
#include "IDevice.h"
#include <d3d11.h>

#include <wrl/client.h> //COM ����Ʈ������ ���
using namespace Microsoft::WRL;

/// <summary>
/// ����̽� �������̽��� ��ӹ���
/// DX11�� Device
/// 
/// 24.01.23
/// �ڿ���
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

