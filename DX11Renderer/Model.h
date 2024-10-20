#pragma once
#include "IResource.h"
#include "GraphicsTransfer.h"
using namespace GraphicsTransfer;

#include "Node.h"
#include "Bone.h"
#include "Animation.h"

class Mesh;
class Material;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class BindState;

/// <summary>
/// 메시 셋팅이 너무 복잡하다..
/// Assimp도 이식하는 겸
/// 여러 리소스를 하나로 묶어 Model을 만들기로 했다
/// 
/// 24.01.31 박연하 
/// </summary>
class Model : public IResource
{
public:
	Model();
	~Model();

public:
	bool Load() override;
	void UnLoad() override;

public:
	void Clone(std::shared_ptr<Model> other);
	std::shared_ptr<Node> DeepCopyNode(const std::shared_ptr<Node> source, std::map<std::string, const std::shared_ptr<Node>>& nodeMap, std::shared_ptr<Model> other);
	std::shared_ptr<Mesh> DeepCopyMesh(std::shared_ptr<Mesh> source);
	void LinkBone(std::map<std::string, const std::shared_ptr<Node>>& nodeMap, std::shared_ptr<Node> rootNode);

public:
	void PlayAnimation(const std::wstring& animationName);

public:
	std::wstring m_modelName;

	std::vector<std::shared_ptr<Mesh>> m_meshes;						// 정점
	std::vector<std::vector<std::weak_ptr<Material>>> m_materials;		// 텍스쳐

	std::weak_ptr<VertexShader> m_vertexShader;							// 버텍스 쉐이더
	std::weak_ptr<PixelShader> m_pixelShader;							// 픽셀 쉐이더

	// 상수버퍼를 생성하기 위한 타입정보
	std::vector<VertexCBufferType> m_vertexCbufferTypes;				// 버텍스 상수버퍼
	std::vector<PixelCBufferType> m_pixelCbufferTypes;					// 픽셀 상수버퍼

	std::vector<std::weak_ptr<ConstantBuffer>> m_vertexCB;
	std::vector<std::weak_ptr<ConstantBuffer>> m_pixelCB;

	// 렌더타겟같이 특수한 State를 묶어줄 때 사용
	std::weak_ptr<BindState> m_bindState;								// 뎁스 스텐실, 레스터라이저 등

	std::shared_ptr<Node> m_rootNode;

	std::map<std::wstring, Animation> m_animationList;										// 애니메이션
	Animation m_currentAnimation;

	float m_animationTime = 0.f;
	bool m_hasAnimation = false;
	bool m_hasShadow = true;
	bool m_isPlayer = false;
};

