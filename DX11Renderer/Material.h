#pragma once
#include "IResource.h"
#include "MaterialColor.h"
#include <assimp/material.h>

class Device;

enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

enum class TextureType
{
	Diffuse,
	NormalMap,
	Metallic,
	Roughness,
	failed
};

/// <summary>
/// Material, Texture 리소스
/// 
/// 24.01.31
/// 박연하
/// </summary>
class Material : public IResource
{
public:
	Material();
	~Material();

public:
	bool Load();
	bool Load(std::shared_ptr<Device> device, const MaterialColor& color, aiTextureType type);
	bool Load(std::shared_ptr<Device> device, const MaterialColor* colorData, UINT width, UINT height, aiTextureType type);
	bool Load(std::wstring filePath, std::shared_ptr<Device> device, aiTextureType type);
	bool Load(std::wstring filePath, std::shared_ptr<Device> device);
	bool Load(std::wstring filePath, std::shared_ptr<Device> device, const uint8_t* pData, size_t size, aiTextureType type);
	bool Load(std::shared_ptr<Device> device, ID3D11ShaderResourceView* srv, aiTextureType textureType);

	void UnLoad() override;

private:
	void Initialize1x1ColorTexture(std::shared_ptr<Device> device, const MaterialColor& colorData, aiTextureType type);
	void InitializeColorTexture(std::shared_ptr<Device> device, const MaterialColor* colorData, UINT width, UINT height, aiTextureType type);

public:
	ComPtr<ID3D11ShaderResourceView> GetTextureResourceView() { return m_textureResourceView; }
	ComPtr<ID3D11Texture2D> GetTexture() { return m_texture; }

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

public:
	ComPtr<ID3D11Texture2D> m_texture = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_textureResourceView = nullptr;

	int m_width = 0;
	int m_height = 0;
};