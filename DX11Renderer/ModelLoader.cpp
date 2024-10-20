#include "renderpch.h"
#include "ModelLoader.h"
#include "Device.h"
#include "StringHelper.h"
#include "Animation.h"

ModelLoader::ModelLoader(std::shared_ptr<DXResourceManager> resourceManager, std::shared_ptr<Device> device)
	: m_resourceManager(resourceManager)
	, m_device(device)
	, m_directory("")
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::LoadAnimation(const std::wstring animationName, const std::string& filePath, AnimationState state)
{
	m_directory = m_basePath + filePath;

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

	processAnimation(pScene, animationName, state);
}

void ModelLoader::processAnimation(const aiScene* scene, const std::wstring animationName, AnimationState state)
{
	// MEMO:: fbx 파일당 하나의 애니메이션을 가졌다고 가정하고 짰음
	//auto animation = m_resourceManager->LoadResource<Animation>(animationName);
	Animation animation;
	animation.m_state = state;
	m_model->m_hasAnimation = true;
	
	auto totalFrame = scene->mAnimations[0]->mDuration;
	auto framePerSeconds = 1 / (scene->mAnimations[0]->mTicksPerSecond);
	animation.m_duration = totalFrame * framePerSeconds;

	for (UINT i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
	{
		AnimationNode animationNode;

		auto& currentChannel = scene->mAnimations[0]->mChannels[i];
		for (UINT k = 0; k < currentChannel->mNumPositionKeys; k++)
		{
			AnimationKey keyAnimation;

			keyAnimation.time = currentChannel->mPositionKeys[k].mTime * framePerSeconds;

			keyAnimation.position.x = currentChannel->mPositionKeys[k].mValue.x;
			keyAnimation.position.y = currentChannel->mPositionKeys[k].mValue.y;
			keyAnimation.position.z = currentChannel->mPositionKeys[k].mValue.z;

			keyAnimation.rotation.x = currentChannel->mRotationKeys[k].mValue.x;
			keyAnimation.rotation.y = currentChannel->mRotationKeys[k].mValue.y;
			keyAnimation.rotation.z = currentChannel->mRotationKeys[k].mValue.z;
			keyAnimation.rotation.w = currentChannel->mRotationKeys[k].mValue.w;

			keyAnimation.scale.x = currentChannel->mScalingKeys[k].mValue.x;
			keyAnimation.scale.y = currentChannel->mScalingKeys[k].mValue.y;
			keyAnimation.scale.z = currentChannel->mScalingKeys[k].mValue.z;

			animationNode.keyAnimations.push_back(keyAnimation);
		}

		animation.m_animationNodes.insert(std::pair(currentChannel->mNodeName.C_Str(), animationNode));
	}

	m_model->m_animationList.insert(std::pair(animationName, animation));
}

TextureStorageType ModelLoader::DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType)
{
	if (pMat->GetTextureCount(textureType) == 0)
		return TextureStorageType::None;

	aiString path;
	pMat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();

	//Check if texture is an embedded indexed texture by seeing if the file path is an index #
	if (texturePath[0] == '*')
	{
		if (pScene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}

	//Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
	{
		if (pTex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}

	//Lastly check if texture is a filepath by checking for period before extension name
	if (texturePath.find('.') != std::string::npos)
	{
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None; // No texture exists
}

int ModelLoader::GetTextureIndex(aiString* pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}

std::vector<std::wstring> ModelLoader::LoadMaterialTextures(std::wstring key, aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene)
{
	std::vector<std::wstring> materialTextures;
	TextureStorageType storetype = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);

	if (textureCount == 0)
	{
		storetype = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType)
		{
		case aiTextureType_DIFFUSE:
		{
			auto textureKey = L"Color" + m_modelKey + L"TX";

			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack()) //If color = black, just use grey
			{
				m_resourceManager->LoadResource<Material>(
					textureKey, m_device, Colors::UnloadedTextureColor, textureType);

				materialTextures.push_back(textureKey);
				return materialTextures;
			}
			else
			{
				m_resourceManager->LoadResource<Material>(
					textureKey, m_device, MaterialColor(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType);

				materialTextures.push_back(textureKey);
				return materialTextures;
			}
		}
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			TextureStorageType storetype = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
			std::string filename = m_basePath + path.C_Str();
			std::wstring wideString(m_basePath.begin(), m_basePath.end());
			std::wstring aiPath = ConvertAiStringToWideString(path);
			wideString = wideString + aiPath;

			auto textureName = key + m_modelKey + L"TX" + std::to_wstring(m_index);

			switch (storetype)
			{
			case TextureStorageType::EmbeddedIndexCompressed:
			{
				int index = GetTextureIndex(&path);

				m_resourceManager->LoadResource<Material>(
					textureName, wideString, m_device,
					reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
					pScene->mTextures[index]->mWidth, textureType);

				materialTextures.push_back(textureName);
			}
			break;
			case TextureStorageType::EmbeddedCompressed:
			{
				const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());

				m_resourceManager->LoadResource<Material>(
					textureName, wideString, m_device,
					reinterpret_cast<uint8_t*>(pTexture->pcData),
					pTexture->mWidth, textureType);

				materialTextures.push_back(textureName);
			}
			break;
			case TextureStorageType::Disk:
			{
				bool isSame = false;
				if (filename.find("jpg") != std::string::npos ||
					filename.find("jpeg") != std::string::npos ||
					filename.find("png") != std::string::npos)
					isSame = true;

				if (isSame)
				{
					m_resourceManager->LoadResource<Material>(
						textureName, wideString, m_device);

					materialTextures.push_back(textureName);
				}
				else
				{
					m_resourceManager->LoadResource<Material>(textureName, StringHelper::StringToWide(filename), m_device);

					materialTextures.push_back(textureName);
				}
				break;
			}
			}
		}

	}

	return materialTextures;
}

