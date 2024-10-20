#pragma once
#include "IResource.h"

class Device;

class PixelShader : public IResource
{
public:
	PixelShader();
	virtual ~PixelShader();

public:
	bool Load() override;
	bool Load(std::wstring filePath, std::shared_ptr<Device> device);
	void UnLoad();
	
public:
	ComPtr<ID3D11PixelShader> GetPixelShader() { return m_pixelShader; }

private:
	ComPtr<ID3D11PixelShader> m_pixelShader;

};

