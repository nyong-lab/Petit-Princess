#pragma once
#include "IResource.h"

class BindState : public IResource
{
public:
	BindState();
	~BindState();

public:
	bool Load();
	void UnLoad();

public:
	void SetSamplerState(ComPtr<ID3D11SamplerState> sampler) { m_sampler = sampler; m_hasSampler = true; }
	void SetDepthStencilState(ComPtr<ID3D11DepthStencilState> depthStencil) { m_depthStencil = depthStencil; m_hasDepthStencil = true; }
	void SetRasterizerState(ComPtr<ID3D11RasterizerState> rasterizer) { m_rasterizer = rasterizer; m_hasRasterizer = true; }
	void SetBlendState(ComPtr<ID3D11BlendState> blender) { m_blender = blender; m_hasBlender = true; }

public:
	ComPtr<ID3D11SamplerState> m_sampler;
	ComPtr<ID3D11DepthStencilState> m_depthStencil;
	ComPtr<ID3D11RasterizerState> m_rasterizer;
	ComPtr<ID3D11BlendState> m_blender;

	bool m_hasBlender = false;
	bool m_hasSampler = false;
	bool m_hasDepthStencil = false;
	bool m_hasRasterizer = false;
};