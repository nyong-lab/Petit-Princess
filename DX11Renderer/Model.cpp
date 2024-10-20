#include "renderpch.h"
#include "Model.h"
#include "Mesh.h"
#include <stack>

Model::Model()
{

}

Model::~Model()
{

}

bool Model::Load()
{
	return true;
}

void Model::UnLoad()
{

}

void Model::Clone(std::shared_ptr<Model> other)
{
	// Node / Animation 복사
	if (m_hasAnimation)
	{
		std::map<std::string, const std::shared_ptr<Node>> nodeMap;
		other->m_rootNode = DeepCopyNode(m_rootNode, nodeMap, other);
		LinkBone(nodeMap, other->m_rootNode);

		other->m_hasAnimation = true;
		other->m_animationTime = 0;
		for (auto iter : m_animationList)
		{
			other->m_animationList.insert(std::pair(iter.first, iter.second));
		}
	}
	else
	{
		for(auto iter : m_meshes)
			other->m_meshes.push_back(iter);
	}

	other->m_materials = m_materials;

	other->m_vertexShader = m_vertexShader;
	other->m_pixelShader = m_pixelShader;

	other->m_vertexCbufferTypes = m_vertexCbufferTypes;
	other->m_pixelCbufferTypes = m_pixelCbufferTypes;

	other->m_bindState = m_bindState;
	other->m_hasShadow = m_hasShadow;
	other->m_isPlayer = m_isPlayer;

}

std::shared_ptr<Node> Model::DeepCopyNode(const std::shared_ptr<Node> source, std::map<std::string, const std::shared_ptr<Node>>& nodeMap, std::shared_ptr<Model> other)
{
	if (!source) return nullptr;

	std::shared_ptr<Node> copiedNode = std::make_shared<Node>();
	copiedNode->name = source->name;
	copiedNode->transformation = source->transformation;
	copiedNode->localMatrix = source->localMatrix;
	copiedNode->worldMatrix = source->worldMatrix;

	nodeMap.insert(std::pair(copiedNode->name, copiedNode));

	for (auto iter : source->meshes)
	{
		auto copiedMesh = DeepCopyMesh(iter);
		copiedNode->meshes.push_back(copiedMesh);
		other->m_meshes.push_back(copiedMesh);
	}

	for (auto iter : source->children)
	{
		std::shared_ptr<Node> copiedChild = DeepCopyNode(iter, nodeMap, other);
		if (copiedChild)
		{
			copiedChild->parent = copiedNode;
			copiedNode->children.push_back(copiedChild);
		}
	} 

	return copiedNode;
}

std::shared_ptr<Mesh> Model::DeepCopyMesh(const std::shared_ptr<Mesh> source)
{
	if (!source) return nullptr;

	std::shared_ptr<Mesh> copiedMesh = make_shared<Mesh>();
	copiedMesh->m_meshName = source->m_meshName;
	copiedMesh->m_bones = source->m_bones;
	copiedMesh->m_vertexBuffer = source->m_vertexBuffer;
	copiedMesh->m_vertexCount = source->m_vertexCount;
	copiedMesh->m_vertexStride = source->m_vertexStride;
	copiedMesh->m_vertexOffset = source->m_vertexOffset;
	copiedMesh->m_indexBuffer = source->m_indexBuffer;
	copiedMesh->m_indexCount = source->m_indexCount;
	copiedMesh->m_indexStride = source->m_indexStride;
	copiedMesh->m_indexOffset = source->m_indexOffset;
	copiedMesh->m_primitiveTopology = source->m_primitiveTopology;
	

	return copiedMesh;
}

void Model::LinkBone(std::map<std::string, const std::shared_ptr<Node>>& nodeMap, std::shared_ptr<Node> rootNode)
{
	std::stack<std::shared_ptr<Node>> nodeStack;
	nodeStack.push(rootNode);

	while (!nodeStack.empty())
	{
		std::shared_ptr<Node> currentNode = nodeStack.top();
		nodeStack.pop();

		// 현재 노드 등록
		nodeMap.insert(std::pair(currentNode->name, currentNode));

		// 현재 노드의 메시에 속한 본들을 가져와서 연결
		for (auto mesh : currentNode->meshes)
		{
			for (auto& bone : mesh->m_bones)
			{
				auto it = nodeMap.find(bone.name);
				if (it != nodeMap.end())
					bone.node = it->second;
			}
		}

		// 현재 노드의 자식들을 스택에 추가
		for (auto child : currentNode->children)
		{
			nodeStack.push(child);
		}
	}
}

void Model::PlayAnimation(const std::wstring& animationName)
{
	m_currentAnimation = m_animationList[animationName];
	m_currentAnimation.m_isAnimationEnd = false;
}

