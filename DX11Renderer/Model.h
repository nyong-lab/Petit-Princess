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
/// �޽� ������ �ʹ� �����ϴ�..
/// Assimp�� �̽��ϴ� ��
/// ���� ���ҽ��� �ϳ��� ���� Model�� ������ �ߴ�
/// 
/// 24.01.31 �ڿ��� 
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

	std::vector<std::shared_ptr<Mesh>> m_meshes;						// ����
	std::vector<std::vector<std::weak_ptr<Material>>> m_materials;		// �ؽ���

	std::weak_ptr<VertexShader> m_vertexShader;							// ���ؽ� ���̴�
	std::weak_ptr<PixelShader> m_pixelShader;							// �ȼ� ���̴�

	// ������۸� �����ϱ� ���� Ÿ������
	std::vector<VertexCBufferType> m_vertexCbufferTypes;				// ���ؽ� �������
	std::vector<PixelCBufferType> m_pixelCbufferTypes;					// �ȼ� �������

	std::vector<std::weak_ptr<ConstantBuffer>> m_vertexCB;
	std::vector<std::weak_ptr<ConstantBuffer>> m_pixelCB;

	// ����Ÿ�ٰ��� Ư���� State�� ������ �� ���
	std::weak_ptr<BindState> m_bindState;								// ���� ���ٽ�, �����Ͷ����� ��

	std::shared_ptr<Node> m_rootNode;

	std::map<std::wstring, Animation> m_animationList;										// �ִϸ��̼�
	Animation m_currentAnimation;

	float m_animationTime = 0.f;
	bool m_hasAnimation = false;
	bool m_hasShadow = true;
	bool m_isPlayer = false;
};

