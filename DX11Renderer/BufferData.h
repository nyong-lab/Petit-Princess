#pragma once

/// 버퍼 생성과 관련된 데이터 구조체들을 모아두는 곳 
/// Vertex, InputLayout
/// 
/// 2024.01.21
/// 박연하
namespace VertexStruct
{
	struct Position
	{
		Vector3 position;

		Position() : position(0.f, 0.f, 0.f){ }
		Position(float x, float y, float z) : position(x, y, z){ }
		Position(Vector3 v) : position(v){ }
	};

	struct PositionTexture
	{
		Vector3 position;
		Vector2 texture;
	};

	struct PositionTextureNormal
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texture;
	};

	struct Skinning
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texture;
		float weights[4];
		int boneIndices[4];
	};

	struct DefferedStatic
	{
		DefferedStatic() {}
		DefferedStatic(struct Position pos)
		{
			position = pos.position;
		}
		DefferedStatic(struct PositionTexture posTex)
		{
			position = posTex.position;
			texture = posTex.texture;
		}
		DefferedStatic(struct PositionTextureNormal posTexNor)
		{
			position = posTexNor.position;
			texture = posTexNor.texture;
			normal = posTexNor.normal;
		}

		Vector3 position;
		Vector3 normal;
		Vector3 binormal;
		Vector3 tangent;
		Vector2 texture;
	};

	struct DefferedSkinning
	{
		Vector3 position;
		Vector3 normal;
		Vector3 binormal;
		Vector3 tangent;
		Vector2 texture;
		float weights[4];
		int boneIndices[4];
	};
} 

class InputLayoutDesc
{
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> Position;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> PositionTexture;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> PositionTextureNormal;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> Skinning;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> DefferedStatic;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> DefferedSkinning;
	
};