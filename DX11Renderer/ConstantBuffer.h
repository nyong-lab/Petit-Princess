#pragma once
#include "IResource.h"

#include "Device.h"

namespace CBuffer
{
	struct Transform
	{
		Matrix world;
		Matrix view;
		Matrix proj;
		Matrix wvp;
		Matrix worldInv;
	};

	struct UITransform
	{
		Matrix world;
		Matrix worldViewProj;
		Matrix worldInv;
	};

	struct TextureTransform
	{
		float textureTranslation;
		int typeNum;
		BOOL isFirst;
		float pad;
	};

	struct MiniGameButton
	{
		BOOL isActive;	// 상호작용 키 활성화 유무
		Vector3 pad;
	};

	struct TimerUI
	{
		Vector2 pos;
		Vector2 size;
		float angle;
		Vector3 pad;
	};

	struct MainUI
	{
		BOOL onExit;
		BOOL onCredit;
		float fade;
		float pad;
	};

	struct MerchantUI
	{
		int state;
		int foodtype;
		float ratio;
		float pad;
	};

	struct EatSpotUI
	{
		BOOL meat;
		BOOL crab;
		BOOL apple;
		BOOL cupcake;

		BOOL baguette;
		int count;
		Vector2 pad2;
	};

	struct MiniGameProgress
	{
		int foodtype;	// 음식 타입
		int gametype;	// 미니게임 타입
		int stage;		// QTE 단계
		int progress;	// 단계별 진행도
		int meshCount;
		int direction;  // 드래그 방향
		Vector2 pad;
	};

	struct MiniGameKeyChain
	{
		int key1;
		int key2;
		int key3;
		int key4;
		int key5;
		Vector3 pad;
	};

	struct MiniGameUIMove
	{
		Vector2 UIcenter;
		Vector2 UIsize;
		int meshCount;
		Vector3 pad;
	};

	struct BonePalette
	{
		Matrix gBoneTransforms[512];
	};

	struct CollisionCheck
	{
		int isCollision;
		Vector3 pad;
	};

	struct PBRMaterial
	{
		BOOL useBaseColorMap;
		BOOL useNormalMap;
		BOOL useMetallicMap;
		BOOL useRoughnessMap;

		BOOL useAOMap;
		Vector3 pad3;

		Vector4 BaseColor;

		float metallic;
		float roughness;
		float AO;
		float pad;
	};
}


class ConstantBuffer : public IResource
{
public:
	ConstantBuffer();
	virtual ~ConstantBuffer();

public:
	bool Load() override;
	bool Load(std::shared_ptr<Device> device, UINT bufferSize);

	void UnLoad() override;

public:
	void SetKey(std::wstring key) { m_key = key; }
	std::wstring GetKey() { return m_key; }

public:
	ComPtr<ID3D11Buffer> GetConstantBuffer() { return m_constantBuffer; }

private:
	ComPtr<ID3D11Buffer> m_constantBuffer;
	std::wstring m_key;
};



