#pragma once
#include "IResource.h"

class Device;

class VertexShader : public IResource
{
public:
	VertexShader();
	virtual ~VertexShader();

public:
	bool Load();
	bool Load(std::wstring filePath, std::shared_ptr<Device> device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDesc);
	void UnLoad() override;

public:
	ComPtr<ID3D11InputLayout> GetInputLayout() { return m_inputLayout; }
	ComPtr<ID3D11VertexShader> GetVertexShader() { return m_vertexShader; }

private:
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11VertexShader> m_vertexShader;
};

