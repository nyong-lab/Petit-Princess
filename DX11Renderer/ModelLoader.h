#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>

#include "DXResourceManager.h"
#include "Material.h"
#include "BufferData.h"
#include <type_traits>

#include "Bone.h"
#include "Material.h"
#include "Model.h"
#include "Node.h"
#include <stack>

class Model;
class Device;

/// <summary>
/// 3D Model Importer
/// 선웅님이 완성해주신 코드를
/// 구조에 맞게 이식했다
/// 
/// 24.01.31 박연하
/// </summary>
class ModelLoader
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 binormal;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 tangent;
		float weight[4] = {};
		int boneIndices[4] = { -1,-1,-1,-1 };
	};

public:
	ModelLoader(const shared_ptr<DXResourceManager> resourceManager, const shared_ptr<Device> device);
	~ModelLoader();

public:
	template <typename VERTEX_T>
	std::shared_ptr<Model> LoadModel(const std::wstring key, const std::string& filePath);

	void LoadAnimation(const std::wstring animationName, const std::string& filePath, AnimationState state);

private:
	template <typename VERTEX_T>
	void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<Node> parentNode, std::map<std::string, const std::shared_ptr<Node>>& nodeMap);

	template <typename VERTEX_T>
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

	template <typename VERTEX_T>
	std::shared_ptr<Mesh> ProcessBuffer(std::string meshName, std::vector<Vertex>& vertexList, std::vector<UINT>& indexList);

	template<typename VERTEX_T>
	void processBones(std::map<std::string, const std::shared_ptr<Node>>& nodeMap);

	void processAnimation(const aiScene* scene, const std::wstring animationName, AnimationState state);

private:
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);

	std::wstring ConvertAiStringToWideString(const aiString& aiStr) {
		return std::wstring(aiStr.C_Str(), aiStr.C_Str() + aiStr.length); }
	int GetTextureIndex(aiString* pStr);
	std::vector<std::wstring> LoadMaterialTextures(std::wstring key, aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);

private:
	std::shared_ptr<Model> m_model;

	std::wstring m_modelKey;

	shared_ptr<DXResourceManager> m_resourceManager;
	shared_ptr<Device> m_device;

	std::string m_basePath = "../Resource/Model/";
	std::string m_directory;

	int m_index = 0;
};

template <typename VERTEX_T>
std::shared_ptr<Model> ModelLoader::LoadModel(const std::wstring key, const std::string& filePath)
{
	// 이전 데이터 초기화
	m_index = 0;
	m_modelKey = key;
	m_directory = m_basePath + filePath;
	if (m_model) m_model = nullptr;

	std::unique_ptr<Assimp::Importer> importer = std::make_unique<Assimp::Importer>();
	importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
	const aiScene* pScene = importer->ReadFile(m_directory,
		aiProcess_Triangulate |
		aiProcess_GenNormals |                              // 노말 생성
		aiProcess_GenUVCoords |                             // UV 생성
		aiProcess_CalcTangentSpace |                        // 탄젠트 생성
		aiProcess_LimitBoneWeights |
		aiProcess_ConvertToLeftHanded
	);

	if (pScene == nullptr)
		assert(false && "[Assimp] 씬 정보 불러오기 실패");

	// 모델 만들기
	m_model = m_resourceManager->LoadResource<Model>(m_modelKey);
	m_model->m_materials.resize(pScene->mNumMeshes);

	std::map<std::string, const std::shared_ptr<Node>> nodeMap;
	m_model->m_rootNode = std::make_shared<Node>();

	ProcessNode<VERTEX_T>(pScene->mRootNode, pScene, m_model->m_rootNode, nodeMap);
	processBones<VERTEX_T>(nodeMap);

	return m_model;
}

template <typename VERTEX_T>
void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<Node> parentNode, std::map<std::string, const std::shared_ptr<Node>>& nodeMap)
{
	if (node == nullptr) return;

	std::shared_ptr<Node> currentNode = std::make_shared<Node>();
	currentNode->parent = parentNode;
	if (parentNode != nullptr)
	{
		parentNode->children.push_back(currentNode);
	}

	// 현재 노드의 이름
	currentNode->name = node->mName.C_Str();

	// 현재 노드 등록
	nodeMap.insert(std::pair(currentNode->name, currentNode));

	// 행렬 변환
	currentNode->transformation = Matrix(&node->mTransformation.a1).Transpose();

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		currentNode->meshes.push_back(ProcessMesh<VERTEX_T>(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode<VERTEX_T>(node->mChildren[i], scene, currentNode, nodeMap);
	}
}

template <typename VERTEX_T>
std::shared_ptr<Mesh> ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertexList;
	std::vector<UINT> indexList;
	std::vector<Bone> boneList;
	vertexList.resize(mesh->mNumVertices);
	boneList.clear();

	/// Get vertices ---------------------------------------------------------------------------------

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.binormal = Vector3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}

		if (mesh->HasTextureCoords(0))
		{
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiColor3D color;

			//요소들
			//mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			//AI_MATKEY_OPACITY,
			//AI_MATKEY_COLOR_AMBIENT
			//AI_MATKEY_COLOR_SPECULAR

		}
		vertexList[i] = vertex;
	}

	/// ----------------------------------------------------------------------------------------------

	/// Get indices ----------------------------------------------------------------------------------

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indexList.push_back(face.mIndices[j]);
	}

	/// ----------------------------------------------------------------------------------------------

	/// Get Bones ------------------------------------------------------------------------------------

	std::unordered_map<std::string, bool> boneNameMap;
	for (UINT i = 0; i < mesh->mNumBones; ++i)
	{
		Bone bone;
		bone.name = mesh->mBones[i]->mName.C_Str();

		// 중복 체크
		if (boneNameMap.find(bone.name) != boneNameMap.end())
		{
			// 중복된 본 이름이 있음
			continue;
		}

		bone.BoneOffset = Matrix(&mesh->mBones[i]->mOffsetMatrix.a1).Transpose();
		boneList.push_back(bone);
		boneNameMap[bone.name] = true;

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
		{
			unsigned int vertexIndex = mesh->mBones[i]->mWeights[j].mVertexId;
			float vertexWeight = mesh->mBones[i]->mWeights[j].mWeight;

			for (UINT k = 0; k < 4; ++k)
			{
				if (vertexList[vertexIndex].boneIndices[k] == -1)
				{
					vertexList[vertexIndex].boneIndices[k] = i;
					vertexList[vertexIndex].weight[k] = vertexWeight;
					break;
				}
			}
		}
	}

	/// Create Buffer Resource
	auto pMesh = ProcessBuffer<VERTEX_T>(mesh->mName.C_Str(), vertexList, indexList);
	pMesh->m_bones = boneList;

	/// ----------------------------------------------------------------------------------------------

	std::vector<std::wstring> textureKeys;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<std::wstring> diffuseTextures = LoadMaterialTextures(L"Diffuse", material, aiTextureType::aiTextureType_DIFFUSE, scene);
	std::vector<std::wstring> nomalTextures = LoadMaterialTextures(L"Normal", material, aiTextureType::aiTextureType_NORMALS, scene);
	std::vector<std::wstring> metalicTextures = LoadMaterialTextures(L"Metallic", material, aiTextureType::aiTextureType_METALNESS, scene);
	textureKeys.insert(textureKeys.end(), diffuseTextures.begin(), diffuseTextures.end());
	textureKeys.insert(textureKeys.end(), nomalTextures.begin(), nomalTextures.end());
	textureKeys.insert(textureKeys.end(), metalicTextures.begin(), metalicTextures.end());

	for (int i = 0; i < textureKeys.size(); i++)
	{
		auto material = m_resourceManager->LoadResource<Material>(textureKeys[i]);
		m_model->m_materials[m_index].push_back(material);
	}

	m_index++;

	return pMesh;
}

template <typename VERTEX_T>
std::shared_ptr<Mesh> ModelLoader::ProcessBuffer(std::string meshName, std::vector<Vertex>& vertexList, std::vector<UINT>& indexList)
{
	std::vector<VERTEX_T> modelVertexList;
	modelVertexList.resize(vertexList.size());

	if constexpr (std::is_same_v<VERTEX_T, VertexStruct::Position>)
	{
		for (UINT i = 0; i < vertexList.size(); i++)
		{
			VERTEX_T vertex;
			vertex.position = vertexList[i].pos;
			modelVertexList[i] = vertex;
		}
	}
	else if constexpr (std::is_same_v<VERTEX_T, VertexStruct::PositionTexture>)
	{
		for (UINT i = 0; i < vertexList.size(); i++)
		{
			VERTEX_T vertex;
			vertex.position = vertexList[i].pos;
			vertex.texture = vertexList[i].texCoord;
			modelVertexList[i] = vertex;
		}
	}
	else if constexpr (std::is_same_v<VERTEX_T, VertexStruct::PositionTextureNormal>)
	{
		for (UINT i = 0; i < vertexList.size(); i++)
		{
			VERTEX_T vertex;
			vertex.position = vertexList[i].pos;
			vertex.texture = vertexList[i].texCoord;
			vertex.normal = vertexList[i].normal;
			modelVertexList[i] = vertex;
		}
	}
	else if constexpr (std::is_same_v<VERTEX_T, VertexStruct::Skinning>)
	{
		for (int i = 0; i < vertexList.size(); i++)
		{
			VERTEX_T vertex;
			vertex.position = vertexList[i].pos;
			vertex.texture = vertexList[i].texCoord;
			vertex.normal = vertexList[i].normal;

			for (UINT k = 0; k < 4; k++)
			{
				vertex.weights[k] = vertexList[i].weight[k];
				vertex.boneIndices[k] = vertexList[i].boneIndices[k];
			}

			modelVertexList[i] = vertex;
		}
	}
	else if constexpr (std::is_same_v<VERTEX_T, VertexStruct::DefferedStatic>)
	{
		for (int i = 0; i < vertexList.size(); i++)
		{
			VERTEX_T vertex;
			vertex.position = vertexList[i].pos;
			vertex.texture = vertexList[i].texCoord;
			vertex.normal = vertexList[i].normal;
			vertex.binormal = vertexList[i].binormal;
			vertex.tangent = vertexList[i].tangent;
			
			modelVertexList[i] = vertex;
		}
	}
	else if constexpr (std::is_same_v<VERTEX_T, VertexStruct::DefferedSkinning>)
	{
		for (int i = 0; i < vertexList.size(); i++)
		{
			VERTEX_T vertex;
			vertex.position = vertexList[i].pos;
			vertex.texture = vertexList[i].texCoord;
			vertex.normal = vertexList[i].normal;
			vertex.binormal = vertexList[i].binormal;
			vertex.tangent = vertexList[i].tangent;

			for (UINT k = 0; k < 4; k++)
			{
				vertex.weights[k] = vertexList[i].weight[k];
				vertex.boneIndices[k] = vertexList[i].boneIndices[k];
			}

			modelVertexList[i] = vertex;
		}
	}

	auto mesh = m_resourceManager->LoadResource<Mesh>(
		m_modelKey + L"Buffer" + std::to_wstring(m_index),
		m_device, modelVertexList, indexList,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh->m_meshName = meshName;

	m_model->m_meshes.push_back(mesh);
	
	return mesh;
}

template<typename VERTEX_T>
void ModelLoader::processBones(std::map<std::string, const std::shared_ptr<Node>>& nodeMap)
{
	std::stack<std::shared_ptr<Node>> nodeStack;
	nodeStack.push(m_model->m_rootNode);

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

