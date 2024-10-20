#include "renderpch.h"
#include "GraphicsEngine.h"

#include "DXManager.h"

#include "BufferData.h"
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Material.h"
#include "BindState.h"

#include "GraphicsTransfer.h"
using namespace GraphicsTransfer;

#include <sstream>

GraphicsEngine::GraphicsEngine()
	: m_hwnd(), m_screenHeight(), m_screenWidth()
	, m_dxManager(nullptr)
	, m_isVsyncEnabled(true)
{

}

GraphicsEngine::~GraphicsEngine()
{

}

void GraphicsEngine::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	RECT rect;
	GetClientRect(hwnd, &rect);

	m_screenHeight = rect.bottom - rect.top;
	m_screenWidth = rect.right - rect.left;

	// Device 생성
	m_device = std::make_shared<Device>();
	m_device->CreateDevice();

	m_dxManager = std::make_shared<DXManager>(m_device);
	m_dxManager->Initialize(m_screenWidth, m_screenHeight, m_isVsyncEnabled, m_hwnd, false);

	m_resourceManager = std::make_shared<DXResourceManager>(L"../Resource/", m_device);
	m_resourceManager->Initialize();

	// Render Path 생성
	m_renderPath = std::make_unique<RenderPath>();
	m_renderPath->Initialize(m_device, m_screenWidth, m_screenHeight, m_resourceManager, m_dxManager);
}

void GraphicsEngine::Finalize()
{
	m_resourceManager->Finalize();
	m_dxManager->Finalize();
	m_device->Finalize();

	// 폰트 리소스 해제

	const std::wstring& path = L"..\\Resource\\Font\\NeoDunggeunmoPro-Regular.ttf";
	RemoveFontResource(path.c_str());
	::SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
}

void GraphicsEngine::Update(float deltaTime)
{
	int a = 0;

	for (auto& iter : m_activeModelList)
	{
		auto model = iter.second;

		// 애니메이션이 없거나 종료 상태면 넘어감
		if (!model->m_hasAnimation || model->m_currentAnimation.m_isAnimationEnd) continue;

		model->m_animationTime += deltaTime;

		if (model->m_animationTime > model->m_currentAnimation.m_duration)
		{
			// Loop일 경우, 시간을 0으로 되돌림
			if (model->m_currentAnimation.m_state == AnimationState::Loop)
			{
				model->m_animationTime = 0.f;
			}
			// 아닐경우 애니메이션 End
			else
			{
				model->m_animationTime = 0.f;
				model->m_currentAnimation.m_isAnimationEnd = true;
			}
		}

		if (model->m_currentAnimation.m_isAnimationEnd) continue;

		model->m_rootNode->Update(model->m_animationTime, model->m_currentAnimation);

		// Bone Palette Update
		for (UINT i = 0; i < model->m_meshes.size(); i++)
		{
			const auto currentMesh = model->m_meshes[i];
			for (UINT j = 0; j < currentMesh->m_bones.size(); ++j)
			{
				Matrix currentMatirx = currentMesh->m_bones[j].BoneOffset * currentMesh->m_bones[j].node->worldMatrix;
				currentMesh->m_boneTransforms[j] = XMMatrixTranspose(currentMatirx);
			}
		}
	}
}

void GraphicsEngine::Resize(int width, int height)
{
	m_dxManager->ResizeStart();
	m_renderPath->ResizeStart();

	m_dxManager->ResizeEnd(width, height);
	m_renderPath->ResizeEnd(width, height);
}

void GraphicsEngine::BeginDraw()
{
	//화면을 검정색으로 초기화
	const float color[] = { 0.f, 0.f, 0.f, 1.f };
	m_device->GetDeviceContext()->ClearRenderTargetView(m_dxManager->m_renderTargetView.Get(), color);
	m_device->GetDeviceContext()->ClearDepthStencilView(m_dxManager->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0u);

	m_renderPath->ClearRenderTarget();
}

void GraphicsEngine::DefferedDrawTest()
{
	m_renderPath->ProcessDeffered();
}

void GraphicsEngine::EndDraw()
{
	if (m_isVsyncEnabled)	m_dxManager->m_swapChain->Present(1, 0);    // 스크린 refresh rate를 고정
	else					m_dxManager->m_swapChain->Present(0, 0);	// 가능한 빠르게 화면 전환
}

void GraphicsEngine::FillBackground(int colorCode, float alpha)
{
	// 화면을 특정 색으로 채우기
	int r = (colorCode >> 16) & 0xFF;
	int g = (colorCode >> 8) & 0xFF;
	int b = colorCode & 0xFF;

	float r_normalized = static_cast<float>(r) / 255.0f;
	float g_normalized = static_cast<float>(g) / 255.0f;
	float b_normalized = static_cast<float>(b) / 255.0f;

	float color[] = { r_normalized, g_normalized, b_normalized,alpha };
	m_device->GetDeviceContext()->ClearRenderTargetView(m_dxManager->m_renderTargetView.Get(), color);
}

void GraphicsEngine::FillBackground(float r, float g, float b, float alpha)
{
	float color[] = { r, g, b, alpha };
	m_device->GetDeviceContext()->ClearRenderTargetView(m_dxManager->m_renderTargetView.Get(), color);
}

void GraphicsEngine::DrawChar(std::wstring text, Vector2 pos, UINT fontsize /*=20*/,
	Vector4 color /*= Vector4(1.f,1.f,1.f,1.f)*/,
	Vector2 boxSize /*= Vector2(300, 50)*/)
{
	// 폰트가 생성되었는지 찾는다. 없으면 새로 생성한다. 리소스메니저로 넘기고싶은데 다 고치기가 귀찮다...흑
	auto& formats = m_dxManager->m_textFormat;
	auto iter = formats.find(fontsize);
	if (iter == formats.end())
	{
		m_dxManager->CreateTextFormat(L"시원한 설레임 SemiBold", fontsize);
	}

	// 랜더큐에 넣는다
	Text textInfo;
	textInfo.text = text;
	textInfo.pos = pos;
	textInfo.fontSize = fontsize;
	textInfo.color = color;
	textInfo.boxSize = boxSize;

	m_renderPath->m_texts.push_back(textInfo);
}

void GraphicsEngine::SetAlphaBlendingMode(bool enable)
{

}

void GraphicsEngine::SetZBufferUsing(bool enable)
{

}

std::shared_ptr<ConstantBuffer> GraphicsEngine::CreateConstantBuffer(void* addressKey, UINT bufferSize)
{
	// 받은 주소를 키로 변환해 상수버퍼를 만든다.
	std::wstringstream ss;
	ss << std::hex << addressKey;
	std::wstring addressString = ss.str();

	auto constantBuffer = m_resourceManager->LoadResource<ConstantBuffer>(addressString, m_device, bufferSize);
	constantBuffer->SetKey(addressString);
	return constantBuffer;
}

void GraphicsEngine::UpdateConstantBuffer(ComPtr<ID3D11Buffer> buffer, void* pData, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	m_device->GetDeviceContext()->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, pData, size);
	m_device->GetDeviceContext()->Unmap(buffer.Get(), NULL);
}

std::wstring GraphicsEngine::LoadModel(ModelKey modelKey)
{
	std::wstring newKey = L"";
	auto model = GetModel(modelKey, newKey);
	m_modelID++;

	// 기존 모델 복사
	auto newModel = m_resourceManager->LoadResource<Model>(newKey);
	model->Clone(newModel);
	newModel->m_modelName = newKey;

	// 그래픽스 엔진이 관리하는 model vector에 추가
	m_activeModelList.push_back(std::pair(newKey, newModel));

	UINT bufferSize = 0;

	// Create Constant Buffer
	auto modelVCB = model->m_vertexCbufferTypes;
	newModel->m_vertexCB.resize(modelVCB.size());
	for (int i = 0; i < modelVCB.size(); i++)
	{
		switch (modelVCB[i])
		{
		case VertexCBufferType::Transform:
			bufferSize = sizeof(CBuffer::Transform);
			break;
		case VertexCBufferType::UITransform:
			bufferSize = sizeof(CBuffer::UITransform);
			break;
		case VertexCBufferType::BonePalette:
			bufferSize = sizeof(CBuffer::BonePalette);
			break;
		case VertexCBufferType::MiniGameUIMove:
			bufferSize = sizeof(CBuffer::MiniGameUIMove);
			break;
		case VertexCBufferType::TimerUI:
			bufferSize = sizeof(CBuffer::TimerUI);
			break;

		default:
			break;
		}

		auto cb = CreateConstantBuffer(&newModel->m_vertexCB[i], bufferSize);
		newModel->m_vertexCB[i] = cb;
	}

	auto modelPCB = model->m_pixelCbufferTypes;
	newModel->m_pixelCB.resize(modelPCB.size());
	for (int i = 0; i < modelPCB.size(); i++)
	{
		switch (modelPCB[i])
		{
		case PixelCBufferType::PBRMaterial:
			bufferSize = sizeof(CBuffer::PBRMaterial);
			break;
		case PixelCBufferType::TextureTransform:
			bufferSize = sizeof(CBuffer::TextureTransform);
			break;
		case PixelCBufferType::CollisionCheck:
			bufferSize = sizeof(CBuffer::CollisionCheck);
			break;
		case PixelCBufferType::MiniGameButton:
			bufferSize = sizeof(CBuffer::MiniGameButton);
			break;
		case PixelCBufferType::MiniGameProgress:
			bufferSize = sizeof(CBuffer::MiniGameProgress);
			break;
		case PixelCBufferType::MiniGameKeyChain:
			bufferSize = sizeof(CBuffer::MiniGameKeyChain);
			break;
		case PixelCBufferType::MainUI:
			bufferSize = sizeof(CBuffer::MainUI);
			break;
		case PixelCBufferType::MerchantUI:
			bufferSize = sizeof(CBuffer::MerchantUI);
			break;
		default:
			break;
		}

		auto cb = CreateConstantBuffer(&newModel->m_pixelCB[i], bufferSize);
		newModel->m_pixelCB[i] = cb;
	}

	return newKey;
}

void GraphicsEngine::ReleaseModel()
{
	for (auto& model : m_activeModelList)
	{
		auto vcb = model.second->m_vertexCB;
		for (auto& buffer : vcb)
		{
			m_resourceManager->UnLoadResource(buffer.lock()->GetKey());
		}

		auto pcb = model.second->m_pixelCB;
		for (auto& buffer : pcb)
		{
			m_resourceManager->UnLoadResource(buffer.lock()->GetKey());
		}

		m_resourceManager->UnLoadResource(model.first);
	}

	m_activeModelList.clear();
	m_modelID = 0;
}

/// <summary>
/// 에디터의 안전/편의성을 위한 하드코딩....
/// 모델이 추가될 때마다 무한대로 늘어난다! ^^!
/// </summary>
shared_ptr<Model> GraphicsEngine::GetModel(ModelKey modelKey, std::wstring& newKey)
{
	std::wstring key = L"";

	switch (modelKey)
	{

	// Primitive ------------------------------------------------

	case GraphicsTransfer::ModelKey::Grid:
		key = L"Grid";
		break;
	case GraphicsTransfer::ModelKey::TextureBox:
		key = L"TextureBox";
		break;
	case GraphicsTransfer::ModelKey::RedBox:
		key = L"RedBox";
		break;
	case GraphicsTransfer::ModelKey::YellowBox:
		key = L"YellowBox";
		break;
	case GraphicsTransfer::ModelKey::PinkBox:
		key = L"PinkBox";
		break;
	case GraphicsTransfer::ModelKey::WireFrameBox:
		key = L"WireFrameBox";
		break;
	case GraphicsTransfer::ModelKey::WireFrameRay:
		key = L"WireFrameRay";
		break;
	case GraphicsTransfer::ModelKey::WireFrameSphere:
		key = L"WireFrameSphere";
		break;
	case GraphicsTransfer::ModelKey::Swablu:
		key = L"Swablu";
		break;
	case GraphicsTransfer::ModelKey::SfBox:
		key = L"SfBox";
		break;
	case GraphicsTransfer::ModelKey::Erika:
		key = L"Erika";
		break;

	/// 실제 아트 리소스 ----------------------------------------------

	// Character ----------------------------------------------------

	case GraphicsTransfer::ModelKey::Player:
		key = L"Player";
		break;
	case GraphicsTransfer::ModelKey::Patrol:
		key = L"Patrol";
		break;
	case GraphicsTransfer::ModelKey::Merchant:
		key = L"Merchant";
		break;

	// Store ----------------------------------------------------

	case GraphicsTransfer::ModelKey::BagleStore:
		key = L"BreadStore";
		break;
	case GraphicsTransfer::ModelKey::BaguettePointStore:
		key = L"BreadStore";
		break;
	case GraphicsTransfer::ModelKey::CupCakeStore:
		key = L"CupCakeStore";
		break;
	case GraphicsTransfer::ModelKey::CrabStore:
		key = L"SeaFoodStore";
		break;
	case GraphicsTransfer::ModelKey::AppleStore:
		key = L"FruitStore";
		break;
	case GraphicsTransfer::ModelKey::HamStore:
		key = L"ButcherStore";
		break;
	case GraphicsTransfer::ModelKey::WaterMelonStore:
		key = L"FruitStore";
		break;
	case GraphicsTransfer::ModelKey::FishStore:
		key = L"SeaFoodStore";
		break;
	case GraphicsTransfer::ModelKey::SausageStore:
		key = L"ButcherStore";
		break;
	case GraphicsTransfer::ModelKey::BaguetteWheelStore:
		key = L"BreadStore";
		break;
	case GraphicsTransfer::ModelKey::BananaStore:
		key = L"FruitStore";
		break;

	// Prop ----------------------------------------------------------

	case GraphicsTransfer::ModelKey::Floor:
		key = L"Floor";
		break;
	case GraphicsTransfer::ModelKey::Wall:
		key = L"Wall";
		break;
	case GraphicsTransfer::ModelKey::Box1:
		key = L"Box1";
		break;
	case GraphicsTransfer::ModelKey::Box2:
		key = L"Box1";
		break;
	case GraphicsTransfer::ModelKey::Tree1:
		key = L"Tree1";
		break;
	case GraphicsTransfer::ModelKey::Tree2:
		key = L"Tree1";
		break;
	case GraphicsTransfer::ModelKey::Candy:
		key = L"Candy1";
		break;
	case GraphicsTransfer::ModelKey::Flag:
		key = L"Flag";
		break;
	case GraphicsTransfer::ModelKey::Barrel:
		key = L"Barrel";
		break;
	case GraphicsTransfer::ModelKey::Bush:
		key = L"Bush";
		break;
	case GraphicsTransfer::ModelKey::Fountain:
		key = L"Fountain";
		break;
	case GraphicsTransfer::ModelKey::Grass:
		key = L"Grass";
		break;
	case GraphicsTransfer::ModelKey::Lamp:
		key = L"Lamp";
		break;
	case GraphicsTransfer::ModelKey::Potion:
		key = L"Potion";
		break;
	case GraphicsTransfer::ModelKey::FoodTable:
		key = L"FoodTable";
		break;
	case GraphicsTransfer::ModelKey::Table:
		key = L"Table";
		break;
	case GraphicsTransfer::ModelKey::Candle1:
		key = L"Candle1";
		break;
	case GraphicsTransfer::ModelKey::Candle2:
		key = L"Candle2";
		break;

	// QTE -----------------------------------------------------------

	case  GraphicsTransfer::ModelKey::QTE_InteractiveKey:
		key = L"QTE_InteractiveKey";
		break;
	case  GraphicsTransfer::ModelKey::QTE_MiniGame:
		key = L"QTE_MiniGame";
		break;

	// UI ------------------------------------------------------------

	case GraphicsTransfer::ModelKey::UI:
		key = L"UI";
		break;
	case GraphicsTransfer::ModelKey::UIGauge:
		key = L"UIGauge";
		break;
	case GraphicsTransfer::ModelKey::UI_Timer:
		key = L"TimerUI";
		break;
	case GraphicsTransfer::ModelKey::UIHungry:
		key = L"UIHungry";
		break;
	case GraphicsTransfer::ModelKey::UI_Main:
		key = L"MainUI";
		break;
	case GraphicsTransfer::ModelKey::UI_Sense:
		key = L"UI_Sense";
		break;
	case GraphicsTransfer::ModelKey::UI_Score:
		key = L"ScoreUI";
		break;
	case GraphicsTransfer::ModelKey::UI_Minimap:
		key = L"UI_Minimap";
		break;
	case GraphicsTransfer::ModelKey::UI_MinimapCurPos:
		key = L"UI_MinimapCurPos";
		break;
	case GraphicsTransfer::ModelKey::UI_MiniMapIcon:
		key = L"UI_MiniMapIcon";
		break;
	case GraphicsTransfer::ModelKey::UI_Skill:
		key = L"UI_Skill";
		break;
	case GraphicsTransfer::ModelKey::UI_Black:
		key = L"UI_Black";
		break;
	case GraphicsTransfer::ModelKey::UI_IngamePopUp:
		key = L"IngamePopUpUI";
		break;
	case GraphicsTransfer::ModelKey::UI_DeliciousFood:
		key = L"UI_DeliciousFood";
		break;
	case GraphicsTransfer::ModelKey::UI_ReVisitCoolTime:
		key = L"UI_ReVisitCoolTime";
		break;
	case GraphicsTransfer::ModelKey::UI_Merchant:
		key = L"MerchantUI";
		break;
	case GraphicsTransfer::ModelKey::UI_EatSpot:
		key = L"EatSpotCheckUI";
		break;
	case GraphicsTransfer::ModelKey::UI_IngameInterKey:
		key = L"HideInteractiveKey";
		break;
	case GraphicsTransfer::ModelKey::UI_GuardFSM:
		key = L"GuardFSM";
		break;

	// Food ----------------------------------------------------

	case GraphicsTransfer::ModelKey::Meat:
		key = L"SampleFood";
		break;
	case GraphicsTransfer::ModelKey::Seafood:
		key = L"SampleFood";
		break;
	case GraphicsTransfer::ModelKey::Bread:
		key = L"SampleFood";
		break;
	case GraphicsTransfer::ModelKey::Fruit:
		key = L"SampleFood";
		break;
	case GraphicsTransfer::ModelKey::Dessert:
		key = L"SampleFood";
		break;
	case GraphicsTransfer::ModelKey::SampleFood:
		key = L"SampleFood";
		break;

	// ETC ----------------------------------------------------

	default:
		break;
	}


	newKey = L"[" + std::to_wstring(m_modelID) + L"] " + key + L"_clone";
	return m_resourceManager->LoadResource<Model>(key);
}

void GraphicsEngine::ObjectRender(ModelData modelData)
{
	m_renderPath->m_models.push_back(modelData);
}

void GraphicsEngine::UIRender(ModelData uiData)
{
	m_renderPath->m_UIs.push_back(uiData);
}

void GraphicsEngine::LightRender(DirectionalLightData lightData)
{
	m_renderPath->m_directionalLight = lightData;
}

void GraphicsEngine::LightRender(PointLightData lightData)
{
	m_renderPath->m_pointLights.push_back(lightData);
}

void GraphicsEngine::SetWorldLightProperty(LightWorldProperty property)
{
	m_renderPath->m_worldPropertyCb = property;
}

void GraphicsEngine::AnimationControl(std::wstring modelKey, std::wstring animationName)
{
	auto model = m_resourceManager->LoadResource<Model>(modelKey);
	model->m_animationTime = 0.f;
	model->PlayAnimation(animationName);
}

void GraphicsEngine::AnimationStop(std::wstring modelKey, bool isStop)
{
	auto model = m_resourceManager->LoadResource<Model>(modelKey);
	model->m_currentAnimation.m_isAnimationEnd = isStop;
}

bool GraphicsEngine::IsAnimationEnd(std::wstring modelKey)
{
	auto model = m_resourceManager->LoadResource<Model>(modelKey);
	return model->m_currentAnimation.m_isAnimationEnd;
}
