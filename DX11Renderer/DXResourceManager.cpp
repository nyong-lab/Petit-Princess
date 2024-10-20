#include "renderpch.h"
#include "DXResourceManager.h"

#include "ModelLoader.h"

#include "Mesh.h"
#include "BufferData.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Material.h"
#include "BindState.h"

#include "GraphicsTransfer.h"
using namespace GraphicsTransfer;

DXResourceManager::DXResourceManager(std::wstring basePath, std::shared_ptr<Device> device)
	: m_device(device)
	, m_basePath(basePath)
{

}

void DXResourceManager::Initialize()
{
	CreateGeoMetry();

	CreateVertexShaderAndInputLayout();
	CreatePixelShader();

	CreateDepthStencilState();
	CreateSamplerState();			// Material보다 먼저 초기화할 것
	CreateRasterizerState();
	CreateBlendState();
	CreateBindState();

	CreateMaterial();

	CreateModel();					// Model은 모든 리소스가 생성된 마지막에
}

void DXResourceManager::Finalize()
{

}

void DXResourceManager::UnLoadResource(std::wstring key)
{
	auto iter = m_resources.find(key);
	if (iter == m_resources.end())
	{
		assert(false && "없는 리소스 해제!");
	}
	else
	{
		m_resources.erase(key);
	}
}

void DXResourceManager::CreateVertexShaderAndInputLayout()
{
	LoadResource<VertexShader>(L"GridVS", m_basePath + L"Shader/GridVS.cso", m_device, InputLayoutDesc::Position);
	LoadResource<VertexShader>(L"SkyBoxVS", m_basePath + L"Shader/SkyboxVS.cso", m_device, InputLayoutDesc::Position);
	LoadResource<VertexShader>(L"TextureBoxVS", m_basePath + L"Shader/SimpleBoxVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"UIVS", m_basePath + L"Shader/UIVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"SkinnedVS", m_basePath + L"Shader/SkinnedVS.cso", m_device, InputLayoutDesc::Skinning);
	LoadResource<VertexShader>(L"AttachedUIVS", m_basePath + L"Shader/AttachedUIVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"MiniGameVS", m_basePath + L"Shader/MiniGameVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"TimerVS", m_basePath + L"Shader/TimerVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"MainUIVS", m_basePath + L"Shader/MainUIVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"SingleImgUIVS", m_basePath + L"Shader/SingleImgUIVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"EatSpotVS", m_basePath + L"Shader/EatSpotVS.cso", m_device, InputLayoutDesc::PositionTexture);

	/// Deffered
	LoadResource<VertexShader>(L"DefferedProcessVS", m_basePath + L"Shader/DefferedProcessVS.cso", m_device, InputLayoutDesc::PositionTexture);
	LoadResource<VertexShader>(L"DefferedGbufferVS", m_basePath + L"Shader/DefferedGbufferVS.cso", m_device, InputLayoutDesc::DefferedStatic);
	LoadResource<VertexShader>(L"DefferedSkinnedVS", m_basePath + L"Shader/DefferedSkinnedVS.cso", m_device, InputLayoutDesc::DefferedSkinning);
	LoadResource<VertexShader>(L"DeferredShadowDepthVS", m_basePath + L"Shader/DeferredShadowDepthVS.cso", m_device, InputLayoutDesc::DefferedSkinning);
	LoadResource<VertexShader>(L"DeferredShadowStaticVS", m_basePath + L"Shader/DeferredShadowStaticVS.cso", m_device, InputLayoutDesc::DefferedStatic);
	LoadResource<VertexShader>(L"DownSampleVS", m_basePath + L"Shader/DownSampleVS.cso", m_device, InputLayoutDesc::PositionTexture);
}

void DXResourceManager::CreatePixelShader()
{
	LoadResource<PixelShader>(L"GridPS", m_basePath + L"Shader/GridPS.cso", m_device);
	LoadResource<PixelShader>(L"SkyBoxPS", m_basePath + L"Shader/SkyboxPS.cso", m_device);
	LoadResource<PixelShader>(L"TextureBoxPS", m_basePath + L"Shader/SimpleBoxPS.cso", m_device);
	LoadResource<PixelShader>(L"ColliderPS", m_basePath + L"Shader/ColliderPS.cso", m_device);
	LoadResource<PixelShader>(L"UIPS", m_basePath + L"Shader/UIPS.cso", m_device);
	LoadResource<PixelShader>(L"SkinnedPS", m_basePath + L"Shader/SkinnedPS.cso", m_device);
	LoadResource<PixelShader>(L"AttachedUIPS", m_basePath + L"Shader/AttachedUIPS.cso", m_device);
	LoadResource<PixelShader>(L"MiniGamePS", m_basePath + L"Shader/MiniGamePS.cso", m_device);
	LoadResource<PixelShader>(L"TimerPS", m_basePath + L"Shader/TimerPS.cso", m_device);
	LoadResource<PixelShader>(L"HungryPS", m_basePath + L"Shader/HungryPS.cso", m_device);
	LoadResource<PixelShader>(L"MainUIPS", m_basePath + L"Shader/MainUIPS.cso", m_device);
	LoadResource<PixelShader>(L"SensePS", m_basePath + L"Shader/SensePS.cso", m_device);
	LoadResource<PixelShader>(L"DeliciousFoodPS", m_basePath + L"Shader/DeliciousFoodPS.cso", m_device);
	LoadResource<PixelShader>(L"ReVisitCoolTimePS", m_basePath + L"Shader/ReVisitCoolTimePS.cso", m_device);
	LoadResource<PixelShader>(L"SingleImgUIPS", m_basePath + L"Shader/SingleImgUIPS.cso", m_device);
	LoadResource<PixelShader>(L"MinimapPS", m_basePath + L"Shader/MinimapPS.cso", m_device);
	LoadResource<PixelShader>(L"MerchantUIPS", m_basePath + L"Shader/MerchantUIPS.cso", m_device);
	LoadResource<PixelShader>(L"EatSpotPS", m_basePath + L"Shader/EatSpotPS.cso", m_device);
	LoadResource<PixelShader>(L"UISkillPS", m_basePath + L"Shader/UISkillPS.cso", m_device);
	LoadResource<PixelShader>(L"UIBlackPS", m_basePath + L"Shader/UIBlackPS.cso", m_device);
	LoadResource<PixelShader>(L"GuardUIPS", m_basePath + L"Shader/GuardUIPS.cso", m_device);

	/// Deffered
	LoadResource<PixelShader>(L"DefferedDirLightPS", m_basePath + L"Shader/DefferedDirLightPS.cso", m_device);
	LoadResource<PixelShader>(L"DefferedTestPS", m_basePath + L"Shader/DefferedTestPS.cso", m_device);
	LoadResource<PixelShader>(L"DefferedGbufferPS", m_basePath + L"Shader/DefferedGbufferPS.cso", m_device);
	LoadResource<PixelShader>(L"DefferedColliderPS", m_basePath + L"Shader/DefferedColliderPS.cso", m_device);
	LoadResource<PixelShader>(L"DefferedDefaultObjectPS", m_basePath + L"Shader/DefferedDefaultObjectPS.cso", m_device);
	LoadResource<PixelShader>(L"DeferredShadowDepthPS", m_basePath + L"Shader/DeferredShadowDepthPS.cso", m_device);
	LoadResource<PixelShader>(L"DefferedFXAAPS", m_basePath + L"Shader/DefferedFXAAPS.cso", m_device);
	LoadResource<PixelShader>(L"BlurXPS", m_basePath + L"Shader/BlurXPS.cso", m_device);
	LoadResource<PixelShader>(L"BlurYPS", m_basePath + L"Shader/BlurYPS.cso", m_device);
	LoadResource<PixelShader>(L"BloomPS", m_basePath + L"Shader/BloomPS.cso", m_device);
	LoadResource<PixelShader>(L"DownSamplePS", m_basePath + L"Shader/DownSamplePS.cso", m_device);
}

void DXResourceManager::CreateMaterial()
{
	LoadResource<Material>(L"TextureBoxTX", m_basePath + L"Texture/leaves.dds", m_device);
	LoadResource<Material>(L"UITX", m_basePath + L"Texture/testUI.dds", m_device);
	LoadResource<Material>(L"UIGauge1TX", m_basePath + L"Texture/charge_shoot_01.png", m_device);
	LoadResource<Material>(L"UIGauge2TX", m_basePath + L"Texture/charge_shoot_03.png", m_device);
	LoadResource<Material>(L"UIGauge3TX", m_basePath + L"Texture/charge_shoot_02.png", m_device);

	/// Hungry
	LoadResource<Material>(L"UIHungry1TX", m_basePath + L"Texture/hungry_01.png", m_device);
	LoadResource<Material>(L"UIHungry2TX", m_basePath + L"Texture/hungry_02.png", m_device);
	LoadResource<Material>(L"UIHungry3TX", m_basePath + L"Texture/hungry_03.png", m_device);
	LoadResource<Material>(L"UIHungryNoiseTX", m_basePath + L"Texture/noise.png", m_device);

	// 재방문 아이콘
	LoadResource<Material>(L"CooltimeBackground", m_basePath + L"Texture/Merchant/cooltime_return_background.png", m_device);
	LoadResource<Material>(L"CooltimeGauge", m_basePath + L"Texture/Merchant/cooltime_return_gauge.png", m_device);
	LoadResource<Material>(L"MerchantMask", m_basePath + L"Texture/Merchant/mask.png", m_device);

	/// Merchant
	// 맛집 아이콘
	LoadResource<Material>(L"AppleEmpty", m_basePath + L"Texture/Merchant/apple_empty.png", m_device);
	LoadResource<Material>(L"AppleFill", m_basePath + L"Texture/Merchant/apple_fill.png", m_device);
	LoadResource<Material>(L"BagguettEmpty", m_basePath + L"Texture/Merchant/bagguett_empty.png", m_device);
	LoadResource<Material>(L"BagguettFill", m_basePath + L"Texture/Merchant/bagguett_fill.png", m_device);
	LoadResource<Material>(L"CrapEmptry", m_basePath + L"Texture/Merchant/crab_empty.png", m_device);
	LoadResource<Material>(L"CrapFill", m_basePath + L"Texture/Merchant/crab_fill.png", m_device);
	LoadResource<Material>(L"CupcakeEmpty", m_basePath + L"Texture/Merchant/cupcake_empty.png", m_device);
	LoadResource<Material>(L"CupcakeFill", m_basePath + L"Texture/Merchant/cupcake_fill.png", m_device);
	LoadResource<Material>(L"MeatEmpty", m_basePath + L"Texture/Merchant/meat_empty.png", m_device);
	LoadResource<Material>(L"MeatFill", m_basePath + L"Texture/Merchant/meat_fill.png", m_device);

	// 맛집 리스트
	LoadResource<Material>(L"EatSpot_Empty_TX", m_basePath + L"Texture/Hud/EatSpot_Empty.png", m_device);
	LoadResource<Material>(L"EatSpot_Fill_TX", m_basePath + L"Texture/Hud/EatSpot_Fill.png", m_device);

	// 상인 눈치 게이지
	LoadResource<Material>(L"GaugeSense1", m_basePath + L"Texture/Merchant/gauge_sense_01.png", m_device);
	LoadResource<Material>(L"GaugeSense2", m_basePath + L"Texture/Merchant/gauge_sense_02.png", m_device);
	LoadResource<Material>(L"GaugeSense3", m_basePath + L"Texture/Merchant/gauge_sense_03.png", m_device);
	LoadResource<Material>(L"GaugeSense4", m_basePath + L"Texture/Merchant/gauge_sense_04.png", m_device);
	LoadResource<Material>(L"GaugeSensebg", m_basePath + L"Texture/Merchant/gauge_sense_bg.png", m_device);

	// 말풍선 ? 
	LoadResource<Material>(L"RestaurantBubble", m_basePath + L"Texture/Merchant/restaurant_bubble.png", m_device);
	LoadResource<Material>(L"RestaurantBubbleDisabled", m_basePath + L"Texture/Merchant/restaurant_bubble_disabled.png", m_device);

	// 미니맵
	LoadResource<Material>(L"UIMinimapTX", m_basePath + L"Texture/ui/map.png", m_device);
	LoadResource<Material>(L"UIMinimapCloseTX", m_basePath + L"Texture/ui/button_close.png", m_device);
	LoadResource<Material>(L"UIMinimapIconTX", m_basePath + L"Texture/icon_minimap1.png", m_device);

	// 스킬
	LoadResource<Material>(L"UISkillTX", m_basePath + L"Texture/skill_BG.png", m_device);
	LoadResource<Material>(L"UISkill2TX", m_basePath + L"Texture/skill_bottle.png", m_device);

	LoadResource<Material>(L"RedCubeTX", m_basePath + L"Texture/RedCube.png", m_device);
	LoadResource<Material>(L"PinkCubeTX", m_basePath + L"Texture/PinkCube.png", m_device);
	LoadResource<Material>(L"YellowCubeTX", m_basePath + L"Texture/YellowCube.png", m_device);

	/// QTE
	LoadResource<Material>(L"InteractiveKeyTX", m_basePath + L"Texture/QTE/QTE_InterKey.png", m_device);
	LoadResource<Material>(L"KeyChainTX", m_basePath + L"Texture/QTE/QTE_Key.png", m_device);
	LoadResource<Material>(L"ProgressKeyTX", m_basePath + L"Texture/QTE/QTE_ProgressKey.png", m_device);
	LoadResource<Material>(L"DragTX", m_basePath + L"Texture/QTE/QTE_Drag.png", m_device);
	LoadResource<Material>(L"QTEBackgroundTX", m_basePath + L"Texture/QTE/QTE_BG.png", m_device);
	LoadResource<Material>(L"WheelTX", m_basePath + L"Texture/QTE/QTE_Wheel.png", m_device);
	LoadResource<Material>(L"QTEFoodTX", m_basePath + L"Texture/QTE/QTE_Food.png", m_device);
	LoadResource<Material>(L"QTEFoodTX2", m_basePath + L"Texture/QTE/QTE_Food2.png", m_device);

	/// CUBE TEXTURE
	LoadResource<Material>(L"GrassCubeTX", m_basePath + L"Texture/grass.dds", m_device);
	LoadResource<Material>(L"SkyCubeTX", m_basePath + L"Texture/sky.dds", m_device);
	LoadResource<Material>(L"SnowCubeTX", m_basePath + L"Texture/snow.dds", m_device);
	LoadResource<Material>(L"SunsetCubeTX", m_basePath + L"Texture/sunset.dds", m_device);

	/// Timer
	LoadResource<Material>(L"TimerBackgroundTX", m_basePath + L"Texture/timer/background.png", m_device);
	LoadResource<Material>(L"TimerMaskTX", m_basePath + L"Texture/timer/mask.png", m_device);
	LoadResource<Material>(L"TimerOutlineTX", m_basePath + L"Texture/timer/outline.png", m_device);
	LoadResource<Material>(L"TimerSunTX", m_basePath + L"Texture/timer/sun.png", m_device);

	///Main Scene
	LoadResource<Material>(L"MainBackgroundTX", m_basePath + L"Texture/title/background.png", m_device);
	LoadResource<Material>(L"MainExitTX", m_basePath + L"Texture/title/popup_exit.png", m_device);
	LoadResource<Material>(L"MainCreditTX", m_basePath + L"Texture/title/popup_credit.png", m_device);
	LoadResource<Material>(L"HideInterKeyTX", m_basePath + L"Texture/ui/InterKey.png", m_device);

	/// 한장짜리 UI
	LoadResource<Material>(L"ScoreTX", m_basePath + L"Texture/ui/score.png", m_device);
	LoadResource<Material>(L"InGamePopUpTX", m_basePath + L"Texture/ui/popup.png", m_device);

	/// fade
	LoadResource<Material>(L"UIBlackTX", m_basePath + L"Texture/Black.png", m_device);

	/// 순찰병 FSM
	LoadResource<Material>(L"GuardCallTX", m_basePath + L"Texture/status/call.png", m_device);
	LoadResource<Material>(L"GuardChaseTX", m_basePath + L"Texture/status/chase.png", m_device);
	LoadResource<Material>(L"GuardSkillTX", m_basePath + L"Texture/status/skill.png", m_device);
	LoadResource<Material>(L"GuardSuspTX", m_basePath + L"Texture/status/susp.png", m_device);
}

void DXResourceManager::CreateSamplerState()
{
	/// LINEAR
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_device->GetDevice()->CreateSamplerState(&sampDesc, m_linearSamplerState.GetAddressOf());
}

void DXResourceManager::CreateDepthStencilState()
{
	/// Basic -----------------------------------------------------------------------------

	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 깊이 상태값 설정
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//픽셀이 front-face일 때 stencil의 행동
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//픽셀이 back-face일 때 stencil의 행동
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이&스텐실 State 생성
	HR(m_device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_basicDepthState));

	depthStencilDesc.DepthEnable = false;
	HR(m_device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_depthDisabledDepthState));

	/// Cube Map -----------------------------------------------------------------------------
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //큐브맵을 위한
	depthStencilDesc.DepthEnable = true;

	// 깊이&스텐실 State 생성
	HR(m_device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_cubeMapDepthState));
}

void DXResourceManager::CreateRasterizerState()
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC rasterDesc;

	// Desc초기화  
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;		//어떤 면을 제거할것인가? NONE, FRONT, BACK
	rasterDesc.DepthBias = 0;					//깊이값 조절(그림자 및 오브젝트 겹침에 사용) 음수일수록 뒤로간다
	rasterDesc.DepthBiasClamp = 0.0f;			//depthBias가 너무 큰 경우를 방지
	rasterDesc.DepthClipEnable = true;			//깊이 클리핑을 활성화 또는 비활성화
	rasterDesc.FillMode = D3D11_FILL_SOLID;		//D3D11_FILL_WIREFRAME
	rasterDesc.FrontCounterClockwise = false;	// true : 정면은 시계 방향, false : 정면은 시계반대
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;			//클리핑 사각형을 사용하여 클리핑
	rasterDesc.SlopeScaledDepthBias = 0.0f;		//경사 기반의 깊이 바이어스 적용. 

	// 래스터라이즈 단계 생성
	hr = m_device->GetDevice()->CreateRasterizerState(&rasterDesc, m_cullBackRasterState.GetAddressOf());
	if (FAILED(hr))		assert(false && L"Rasterizer State 생성 실패");

	rasterDesc.CullMode = D3D11_CULL_FRONT;
	hr = m_device->GetDevice()->CreateRasterizerState(&rasterDesc, m_cullFrontRasterState.GetAddressOf());

	rasterDesc.CullMode = D3D11_CULL_NONE;
	hr = m_device->GetDevice()->CreateRasterizerState(&rasterDesc, m_cullNoneRasterState.GetAddressOf());
}

void DXResourceManager::CreateBlendState()
{
	HRESULT hr;
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));

	//알파블랜딩 설정
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//알파블랜딩 단계 생성
	hr = m_device->GetDevice()->CreateBlendState(&blendStateDesc, m_alphaEnabledBlendState.GetAddressOf());
	if (FAILED(hr))		assert(false && L"Blend State 생성 실패");

	// 알파 블렌드 비활성화 상태를 생성하도록 수정
	blendStateDesc.RenderTarget[0].BlendEnable = false;

	// 알파블랜딩 비활성화 단계 생성
	hr = m_device->GetDevice()->CreateBlendState(&blendStateDesc, m_alphaDisabledBlendState.GetAddressOf());
	if (FAILED(hr))		assert(false && L"Blend State 생성 실패");
}

void DXResourceManager::CreateBindState()
{
	auto cubeMapState = LoadResource<BindState>(L"CubeState");
	cubeMapState->SetDepthStencilState(m_cubeMapDepthState);

	auto UIState = LoadResource<BindState>(L"UIState");
	UIState->SetRasterizerState(m_cullNoneRasterState);
	UIState->SetDepthStencilState(m_depthDisabledDepthState);
	UIState->SetBlendState(m_alphaEnabledBlendState);
}

void DXResourceManager::CreateGeoMetry()
{
	CreateGrid();
	CreateTextureBox();
	CreateWireFrameBox();
	CreateWireFrameSphere();
	CreateWireFrameCapsule_Top();
	CreateWireFrameCapsule_Middle();
	CreateWireFrameCapsule_Bottom();
	CreateWireFrameRay();
	CreateSkyBox();
	CreateFullScreenRect();
	CreateCanvas();

	CreateUIRect(150, 100, L"UI1");
	CreateUIRect(110, 10, L"UI2");
	CreateUIRect(30, 150, L"UI3");
	CreateUIRect(90, 10, L"Gauge");
	CreateUIRect(210, 210, L"Hungry");
	CreateUIRect(100, 100, L"Sense");
	CreateUIRect(100, 100, L"QTE_InterBT_Buffer");
	CreateUIRect(1400, 730, L"Minimap");
	CreateUIRect(120, 110, L"MinimapClose");
	CreateUIRect(200, 140, L"MinimapIcon");
	CreateUIRect(100, 100, L"DeliciousFood");
	CreateUIRect(100, 100, L"ReVisitCoolTime");
	CreateUIRect(100, 100, L"UISkill");
	CreateUIRect(1920, 1080, L"UIBlack");
	CreateUIRect(60, 60, L"HideInterBT_Buffer");
}

void DXResourceManager::CreateGrid()
{
	int size = 150 + 1;

	std::vector<VertexStruct::Position> vertexList;

	for (unsigned int row = 0; row < size; ++row)
	{
		for (unsigned int col = 0; col < size; ++col)
		{
			vertexList.push_back({ 0.f + row, 0, 0.f + col });
		}
	}

	std::vector<VertexStruct::DefferedStatic> transferDefferedVertex;
	for (auto& vertex : vertexList)
	{
		transferDefferedVertex.push_back(VertexStruct::DefferedStatic(vertex));
	}

	std::vector<UINT> indexList;

	for (unsigned int row = 0; row < size; ++row)
	{
		indexList.push_back(row);
		indexList.push_back(vertexList.size() - size + row);
	}

	for (unsigned int col = 0; col < size; ++col)
	{
		indexList.push_back(col * size);
		indexList.push_back(150 + col * size);
	}

	LoadResource<Mesh>(L"GridBuffer", m_device, transferDefferedVertex, indexList, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DXResourceManager::CreateTextureBox()
{
	std::vector<VertexStruct::PositionTexture> vertexList =
	{
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f} },
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },
		{ {-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
		{ {-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f} },
		{ { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f} },
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f} },
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
		{ {-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f} },
		{ {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },
		{ { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f} },
		{ { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f} },
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },
	};

	std::vector<VertexStruct::DefferedStatic> transferDefferedVertex;
	for (auto& vertex : vertexList)
	{
		transferDefferedVertex.push_back(VertexStruct::DefferedStatic(vertex));
	}

	std::vector<UINT> indexList = {
		4,2,0,		8,7,3,
		6,5,9,		1,7,10,
		11,12,13,	4,14,10,
		4,15,2,		8,15,7,
		6,16,5,		1,17,7,
		11,18,12,	4,19,14,
	};

	LoadResource<Mesh>(L"TextureBoxBuffer", m_device, transferDefferedVertex, indexList);
}

void DXResourceManager::CreateWireFrameBox()
{
	std::vector<VertexStruct::Position> vertexList =
	{
		{ {-0.5f,  0.5f, -0.5f} },  //0
		{ {-0.5f,  0.5f,  0.5f} },	//1
		{ {-0.5f, -0.5f, -0.5f} },	//2
		{ {-0.5f, -0.5f,  0.5f} },	//3
		{ { 0.5f,  0.5f, -0.5f} },  //4
		{ { 0.5f,  0.5f,  0.5f} },  //5
		{ { 0.5f, -0.5f, -0.5f} },	//6
		{ { 0.5f, -0.5f,  0.5f} },  //7

	};

	std::vector<VertexStruct::DefferedStatic> transferDefferedVertex;
	for (auto& vertex : vertexList)
	{
		transferDefferedVertex.push_back(VertexStruct::DefferedStatic(vertex));
	}

	std::vector<UINT> indexList = {
		0,1,	1,5,	5,4,	4,0,
		2,3,	3,7,	7,6,	6,2,
		0,2,	1,3,	4,6,	5,7
	};

	LoadResource<Mesh>(L"WireFrameBoxBuffer", m_device, transferDefferedVertex, indexList, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DXResourceManager::CreateWireFrameSphere()
{
	std::vector<VertexStruct::Position> vertexList;
	std::vector<UINT> indexList;

	float radius = 1.f;
	UINT sliceCount = 10.f;
	UINT stackCount = 5.f;

	for (UINT i = 0; i <= stackCount; ++i)
	{
		float stackTheta = i * 3.141592 / stackCount;
		float stackRadius = radius * sinf(stackTheta);
		float stackY = radius * cosf(stackTheta);

		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float slicePhi = j * 2.0f * 3.141592 / sliceCount;
			float sliceX = stackRadius * cosf(slicePhi);
			float sliceZ = stackRadius * sinf(slicePhi);

			VertexStruct::Position vertex;
			vertex.position = Vector3(sliceX, stackY, sliceZ);
			vertexList.push_back(vertex);
		}
	}

	std::vector<VertexStruct::DefferedStatic> transferDefferedVertex;
	for (auto& vertex : vertexList)
	{
		transferDefferedVertex.push_back(VertexStruct::DefferedStatic(vertex));
	}

	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			UINT index0 = i * (sliceCount + 1) + j;
			UINT index1 = index0 + 1;
			UINT index2 = (i + 1) * (sliceCount + 1) + j;
			UINT index3 = index2 + 1;

			indexList.push_back(index0);
			indexList.push_back(index1);
			indexList.push_back(index1);
			indexList.push_back(index3);
			indexList.push_back(index3);
			indexList.push_back(index2);
			indexList.push_back(index2);
			indexList.push_back(index0);
		}
	}

	LoadResource<Mesh>(L"WireFrameSphereBuffer", m_device, transferDefferedVertex, indexList, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DXResourceManager::CreateWireFrameCapsule_Top()
{
	std::vector<VertexStruct::Position> vertexList;
	std::vector<UINT> indexList;

	float radius = 1.f;
	UINT sliceCount = 10.f;
	UINT stackCount = 5.f;

	for (UINT i = 0; i <= stackCount; ++i)
	{
		float stackTheta = i * DirectX::XM_PI / stackCount;
		float stackRadius = radius * sinf(stackTheta);
		float stackY = radius * cosf(stackTheta);

		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float slicePhi = j * 2.0f * DirectX::XM_PI / sliceCount;
			float sliceX = stackRadius * cosf(slicePhi);
			float sliceZ = stackRadius * sinf(slicePhi);

			VertexStruct::Position vertex;
			vertex.position = Vector3(sliceX, stackY, sliceZ);
			vertexList.push_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			UINT index0 = i * (sliceCount + 1) + j;
			UINT index1 = index0 + 1;
			UINT index2 = (i + 1) * (sliceCount + 1) + j;
			UINT index3 = index2 + 1;

			indexList.push_back(index0);
			indexList.push_back(index1);
			indexList.push_back(index1);
			indexList.push_back(index3);
			indexList.push_back(index3);
			indexList.push_back(index2);
			indexList.push_back(index2);
			indexList.push_back(index0);
		}
	}

	LoadResource<Mesh>(L"WireFrameCapsule_Top", m_device, vertexList, indexList, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DXResourceManager::CreateWireFrameCapsule_Middle()
{
	std::vector<VertexStruct::Position> vertexList;
	std::vector<UINT> indexList;

	float radius = 1.f;
	float stackHeight = 1 / 2.0f;
	UINT sliceCount = 10.f;
	float stackDelta = 2.0f * DirectX::XM_PI / sliceCount;

	// 원통의 상단면 생성
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = radius * cosf(i * stackDelta);
		float z = radius * sinf(i * stackDelta);

		VertexStruct::Position vertex;
		vertex.position = Vector3(x, stackHeight, z);
		vertexList.push_back(vertex);
	}

	// 원통의 아랫면 생성
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = radius * cosf(i * stackDelta);
		float z = radius * sinf(i * stackDelta);

		VertexStruct::Position vertex;
		vertex.position = Vector3(x, -stackHeight, z);
		vertexList.push_back(vertex);
	}

	// 원통의 옆면 생성
	for (UINT i = 0; i < sliceCount; ++i)
	{
		indexList.push_back(i);
		indexList.push_back(i + 1);
		indexList.push_back(i + sliceCount + 1);

		indexList.push_back(i + 1);
		indexList.push_back(i + sliceCount + 2);
		indexList.push_back(i + sliceCount + 1);
	}

	LoadResource<Mesh>(L"WireFrameCapsule_Middle", m_device, vertexList, indexList, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DXResourceManager::CreateWireFrameCapsule_Bottom()
{
	std::vector<VertexStruct::Position> vertexList;
	std::vector<UINT> indexList;

	float radius = 1.f;
	UINT sliceCount = 10.f;
	UINT stackCount = 5.f;

	for (UINT i = 0; i <= stackCount; ++i)
	{
		float stackTheta = i * DirectX::XM_PI / stackCount;
		float stackRadius = radius * sinf(stackTheta);
		float stackY = -radius * cosf(stackTheta);

		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float slicePhi = j * 2.0f * DirectX::XM_PI / sliceCount;
			float sliceX = stackRadius * cosf(slicePhi);
			float sliceZ = stackRadius * sinf(slicePhi);

			VertexStruct::Position vertex;
			vertex.position = Vector3(sliceX, stackY, sliceZ);
			vertexList.push_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			UINT index0 = i * (sliceCount + 1) + j;
			UINT index1 = index0 + 1;
			UINT index2 = (i + 1) * (sliceCount + 1) + j;
			UINT index3 = index2 + 1;

			indexList.push_back(index0);
			indexList.push_back(index3);
			indexList.push_back(index2);
			indexList.push_back(index0);
			indexList.push_back(index1);
			indexList.push_back(index3);
		}
	}

	LoadResource<Mesh>(L"WireFrameCapsule_Bottom", m_device, vertexList, indexList, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DXResourceManager::CreateWireFrameRay()
{
	std::vector<VertexStruct::Position> vertexList =
	{
		{0.0f, 0.f, 0.f},
		{1.f, 0.f, 0.f}
	};

	std::vector<VertexStruct::DefferedStatic> transferDefferedVertex;
	for (auto& vertex : vertexList)
	{
		transferDefferedVertex.push_back(VertexStruct::DefferedStatic(vertex));
	}

	std::vector<UINT> indexList = { 0, 1 };
	LoadResource<Mesh>(L"WireFrameRayBuffer", m_device, transferDefferedVertex, indexList, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DXResourceManager::CreateSkyBox()
{
	std::vector<VertexStruct::Position> vertexList =
	{
		// Front Face
		{-1.0f, -1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{1.0f,  1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},

		// Back Face            
		{-1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f },
		{1.0f,  1.0f, 1.0f },
		{-1.0f,  1.0f, 1.0f},

		// Top Face                
		{-1.0f, 1.0f, -1.0f},
		{-1.0f, 1.0f,  1.0f},
		{1.0f, 1.0f,  1.0f },
		{1.0f, 1.0f, -1.0f },

		// Bottom Face            
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f,  1.0f },
		{-1.0f, -1.0f,  1.0f},

		// Left Face            
		{-1.0f, -1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f},

		// Right Face            
		{1.0f, -1.0f, -1.0f},
		{1.0f,  1.0f, -1.0f},
		{1.0f,  1.0f,  1.0f},
		{1.0f, -1.0f,  1.0f},
	};

	std::vector<UINT> indexList =
	{
		// Front Face
		0,  2,  1,
		0,  3,  2,

		// Back Face
		4,  6,  5,
		4,  7,  6,

		// Top Face
		8, 10,  9,
		8, 11, 10,

		// Bottom Face
		12, 14, 13,
		12, 15, 14,

		// Left Face
		16, 18, 17,
		16, 19, 18,

		// Right Face
		20, 22, 21,
		20, 23, 22
	};

	LoadResource<Mesh>(L"SkyBoxBuffer", m_device, vertexList, indexList);
}

void DXResourceManager::CreateFullScreenRect()
{
	std::vector<VertexStruct::PositionTexture> vertexList =
	{
		{ Vector3{-1.0f, -1.0f, 0.0f}, Vector2{0.0f, 1.0f} }, // Top-left
		{ Vector3{ 1.0f, -1.0f, 0.0f}, Vector2{1.0f, 1.0f} }, // Top-right
		{ Vector3{-1.0f,  1.0f, 0.0f}, Vector2{0.0f, 0.0f} }, // Bottom-left
		{ Vector3{ 1.0f,  1.0f, 0.0f}, Vector2{1.0f, 0.0f} }  // Bottom-right
	};

	std::vector<unsigned int> indexList =
	{
		0,2,1,
		2,3,1
	};

	LoadResource<Mesh>(L"FullScreenRectBuffer", m_device, vertexList, indexList);
}

void DXResourceManager::CreateCanvas()
{
	std::vector<VertexStruct::PositionTexture> vertexList =
	{
		{ Vector3{-1.0f,  1.0f, 0.0f}, Vector2{0.0f, 1.0f} }, // Top-left
		{ Vector3{ 1.0f,  1.0f, 0.0f}, Vector2{1.0f, 1.0f} }, // Top-right
		{ Vector3{-1.0f, -1.0f, 0.0f}, Vector2{0.0f, 0.0f} }, // Bottom-left
		{ Vector3{ 1.0f, -1.0f, 0.0f}, Vector2{1.0f, 0.0f} }  // Bottom-right
	};

	std::vector<unsigned int> indexList =
	{
		0,2,1,
		2,3,1
	};

	LoadResource<Mesh>(L"CanvasBuffer", m_device, vertexList, indexList);
}

void DXResourceManager::CreateUIRect(float width, float height, std::wstring key)
{
	std::vector<VertexStruct::PositionTexture> vertexList =
	{
		{ Vector3{-width / 2.0f,  height / 2.0f, 0.0f}, Vector2{0.0f, 0.0f} }, // Top-left
		{ Vector3{ width / 2.0f,  height / 2.0f, 0.0f}, Vector2{1.0f, 0.0f} }, // Top-right
		{ Vector3{-width / 2.0f, -height / 2.0f, 0.0f}, Vector2{0.0f, 1.0f} }, // Bottom-left
		{ Vector3{ width / 2.0f, -height / 2.0f, 0.0f}, Vector2{1.0f, 1.0f} }  // Bottom-right
	};

	std::vector<unsigned int> indexList =
	{
		0,1,2,
		2,1,3
	};

	LoadResource<Mesh>(key, m_device, vertexList, indexList);

}

void DXResourceManager::CreateModel()
{
	// ModelLoader 생성
	std::unique_ptr<ModelLoader> modelLoader = std::make_unique<ModelLoader>(shared_from_this(), m_device);

	auto defferedDefaultVS = LoadResource<VertexShader>(L"DefferedGbufferVS");
	auto defferedDefaultPS = LoadResource<PixelShader>(L"DefferedGbufferPS");
	auto defferedBasic = LoadResource<PixelShader>(L"DefferedDefaultObjectPS");

	auto uiState = LoadResource<BindState>(L"UIState");

#pragma region primitive
	// [PRIMITIVE] Grid
	{
		auto model = LoadResource<Model>(L"Grid");
		auto buffer = LoadResource<Mesh>(L"GridBuffer");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedBasic;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [PRIMITIVE] Texture Box
	{
		auto model = LoadResource<Model>(L"TextureBox");
		auto buffer = LoadResource<Mesh>(L"TextureBoxBuffer");
		auto texture = LoadResource<Material>(L"TextureBoxTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedBasic;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [PRIMITIVE] Red Box
	{
		auto model = LoadResource<Model>(L"RedBox");
		auto buffer = LoadResource<Mesh>(L"TextureBoxBuffer");
		auto texture = LoadResource<Material>(L"RedCubeTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedBasic;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [PRIMITIVE] Yellow Box
	{
		auto model = LoadResource<Model>(L"YellowBox");
		auto buffer = LoadResource<Mesh>(L"TextureBoxBuffer");
		auto texture = LoadResource<Material>(L"YellowCubeTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedBasic;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [PRIMITIVE] Yellow Box
	{
		auto model = LoadResource<Model>(L"PinkBox");
		auto buffer = LoadResource<Mesh>(L"TextureBoxBuffer");
		auto texture = LoadResource<Material>(L"PinkCubeTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedBasic;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [PRIMITIVE] Box Wireframe
	{
		auto model = LoadResource<Model>(L"WireFrameBox");
		auto buffer = LoadResource<Mesh>(L"WireFrameBoxBuffer");
		auto pixelShader = LoadResource<PixelShader>(L"DefferedColliderPS");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = pixelShader;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::CollisionCheck);
	}

	// [PRIMITIVE] Ray Wireframe
	{
		auto model = LoadResource<Model>(L"WireFrameRay");
		auto buffer = LoadResource<Mesh>(L"WireFrameRayBuffer");
		auto pixelShader = LoadResource<PixelShader>(L"DefferedColliderPS");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = pixelShader;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::CollisionCheck);
	}

	// [PRIMITIVE] Sphere Wireframe
	{
		auto model = LoadResource<Model>(L"WireFrameSphere");
		auto buffer = LoadResource<Mesh>(L"WireFrameSphereBuffer");
		auto pixelShader = LoadResource<PixelShader>(L"DefferedColliderPS");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = pixelShader;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::CollisionCheck);
	}

	// [FBX] Swablu
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Swablu", "wa2.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [FBX] Multie Material Box
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"SfBox", "sfBox.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [FBX] Table
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"SampleTable", "sample_foodtable.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [FBX] Store
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"SampleStore", "sample_store.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [FBX] Erika 
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedSkinning>(L"Erika", "Erika.fbx");
		//modelLoader->LoadAnimation(L"ErikaTurn", "standing turn 90 left.fbx", AnimationState::Once);
		modelLoader->LoadAnimation(L"ErikaIdle", "idle.fbx", AnimationState::Loop);
		auto vertexShader = LoadResource<VertexShader>(L"DefferedSkinnedVS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::BonePalette);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}
#pragma endregion

	/// 실제 아트 리소스 ----------------------------------------------------------

#pragma region UI

	// [PRIMITIVE] UI
	{
		auto model = LoadResource<Model>(L"UI");
		auto buffer = LoadResource<Mesh>(L"UI1");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"UIPS");
		auto texture = LoadResource<Material>(L"UITX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] UI Black
	{
		auto model = LoadResource<Model>(L"UI_Black");
		auto buffer = LoadResource<Mesh>(L"UIBlack");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"UIBlackPS");
		auto texture = LoadResource<Material>(L"UIBlackTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;

	}

	// [PRIMITIVE] UIGauge
	{
		auto model = LoadResource<Model>(L"UIGauge");
		auto buffer = LoadResource<Mesh>(L"Gauge");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"UIPS");
		auto texture = LoadResource<Material>(L"UIGauge1TX");
		auto texture2 = LoadResource<Material>(L"UIGauge2TX");
		auto texture3 = LoadResource<Material>(L"UIGauge3TX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_materials[0].push_back(texture2);
		model->m_materials[0].push_back(texture3);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] UIHungry
	{
		auto model = LoadResource<Model>(L"UIHungry");
		auto buffer = LoadResource<Mesh>(L"Hungry");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"HungryPS");
		auto texture = LoadResource<Material>(L"UIHungry1TX");
		auto texture2 = LoadResource<Material>(L"UIHungry2TX");
		auto texture3 = LoadResource<Material>(L"UIHungry3TX");
		auto texture4 = LoadResource<Material>(L"UIHungryNoiseTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_materials[0].push_back(texture2);
		model->m_materials[0].push_back(texture3);
		model->m_materials[0].push_back(texture4);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] UISense
	{

		auto model = LoadResource<Model>(L"UI_Sense");
		auto buffer = LoadResource<Mesh>(L"Sense");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"SensePS");
		auto texture = LoadResource<Material>(L"GaugeSense1");
		auto texture2 = LoadResource<Material>(L"GaugeSense2");
		auto texture3 = LoadResource<Material>(L"GaugeSense3");
		auto texture4 = LoadResource<Material>(L"GaugeSense4");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_materials[0].push_back(texture2);
		model->m_materials[0].push_back(texture3);
		model->m_materials[0].push_back(texture4);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] UIReVisitCoolTime
	{

		auto model = LoadResource<Model>(L"UI_ReVisitCoolTime");
		auto buffer = LoadResource<Mesh>(L"ReVisitCoolTime");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"ReVisitCoolTimePS");
		auto texture = LoadResource<Material>(L"CooltimeBackground");
		auto texture2 = LoadResource<Material>(L"CooltimeGauge");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_materials[0].push_back(texture2);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] UIDeliciousFood
	{

		auto model = LoadResource<Model>(L"UI_DeliciousFood");
		auto buffer = LoadResource<Mesh>(L"DeliciousFood");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"DeliciousFoodPS");
		auto texture = LoadResource<Material>(L"AppleEmpty");
		auto texture2 = LoadResource<Material>(L"AppleFill");
		auto texture3 = LoadResource<Material>(L"BagguettEmpty");
		auto texture4 = LoadResource<Material>(L"BagguettFill");
		auto texture5 = LoadResource<Material>(L"CrapEmptry");
		auto texture6 = LoadResource<Material>(L"CrapFill");
		auto texture7 = LoadResource<Material>(L"CupcakeEmpty");
		auto texture8 = LoadResource<Material>(L"CupcakeFill");
		auto texture9 = LoadResource<Material>(L"MeatEmpty");
		auto texture10 = LoadResource<Material>(L"MeatFill");
		auto texture11 = LoadResource<Material>(L"RestaurantBubble");
		auto texture12 = LoadResource<Material>(L"RestaurantBubbleDisabled");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_materials[0].push_back(texture2);
		model->m_materials[0].push_back(texture3);
		model->m_materials[0].push_back(texture4);
		model->m_materials[0].push_back(texture5);
		model->m_materials[0].push_back(texture6);
		model->m_materials[0].push_back(texture7);
		model->m_materials[0].push_back(texture8);
		model->m_materials[0].push_back(texture9);
		model->m_materials[0].push_back(texture10);
		model->m_materials[0].push_back(texture11);
		model->m_materials[0].push_back(texture12);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] Minimap
	{
		auto model = LoadResource<Model>(L"UI_Minimap");
		auto buffer = LoadResource<Mesh>(L"Minimap");
		auto vertexShader = LoadResource<VertexShader>(L"AttachedUIVS");
		auto pixelShader = LoadResource<PixelShader>(L"AttachedUIPS");
		auto texture = LoadResource<Material>(L"UIMinimapTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] Hide Interactive Key (Press F)
	{
		auto model = LoadResource<Model>(L"HideInteractiveKey");
		auto buffer = LoadResource<Mesh>(L"HideInterBT_Buffer");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"MinimapPS");
		auto texture = LoadResource<Material>(L"HideInterKeyTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::MiniGameButton);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] Minimap-CurPos
	{
		auto model = LoadResource<Model>(L"UI_MinimapCurPos");
		auto buffer = LoadResource<Mesh>(L"MinimapClose");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"MinimapPS");
		auto texture = LoadResource<Material>(L"UIMinimapCloseTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] Minimap Icon
	{
		auto model = LoadResource<Model>(L"UI_MiniMapIcon");
		auto buffer = LoadResource<Mesh>(L"MinimapIcon");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"UIPS");
		auto texture = LoadResource<Material>(L"UIMinimapIconTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] UI Skill
	{
		auto model = LoadResource<Model>(L"UI_Skill");
		auto buffer = LoadResource<Mesh>(L"UISkill");
		auto vertexShader = LoadResource<VertexShader>(L"UIVS");
		auto pixelShader = LoadResource<PixelShader>(L"UISkillPS");
		auto texture = LoadResource<Material>(L"UISkillTX");
		auto texture2 = LoadResource<Material>(L"UISkill2TX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_materials[0].push_back(texture2);

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::TextureTransform);
		model->m_bindState = uiState;
	}

	// 순찰병 FSM
	{
		auto model = LoadResource<Model>(L"GuardFSM");
		auto buffer = LoadResource<Mesh>(L"QTE_InterBT_Buffer");
		auto vertexShader = LoadResource<VertexShader>(L"AttachedUIVS");
		auto pixelShader = LoadResource<PixelShader>(L"GuardUIPS");
		
		auto texture = LoadResource<Material>(L"GuardCallTX");
		auto texture1 = LoadResource<Material>(L"GuardChaseTX");
		auto texture2 = LoadResource<Material>(L"GuardSkillTX");
		auto texture3 = LoadResource<Material>(L"GuardSuspTX");
		auto texture12 = LoadResource<Material>(L"RestaurantBubble");

		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_materials[0].push_back(texture1);
		model->m_materials[0].push_back(texture2);
		model->m_materials[0].push_back(texture3);
		model->m_materials[0].push_back(texture12);

		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::MerchantUI);
		model->m_bindState = uiState;
	}


	// [PRIMITIVE] QTE - Interactive Key
	{
		auto model = LoadResource<Model>(L"QTE_InteractiveKey");
		auto buffer = LoadResource<Mesh>(L"QTE_InterBT_Buffer");
		auto vertexShader = LoadResource<VertexShader>(L"AttachedUIVS");
		auto pixelShader = LoadResource<PixelShader>(L"AttachedUIPS");
		auto texture = LoadResource<Material>(L"InteractiveKeyTX");
		model->m_meshes.push_back(buffer);
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::UITransform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::MiniGameButton);
		model->m_bindState = uiState;
	}

	// [PRIMITIVE] QTE - MiniGame
	{
		auto model = LoadResource<Model>(L"QTE_MiniGame");

		int meshCount = 7;
		auto mesh = LoadResource<Mesh>(L"CanvasBuffer");

		for (int i = 0; i < meshCount; i++)
			model->m_meshes.push_back(mesh);

		auto textureBG = LoadResource<Material>(L"QTEBackgroundTX");
		auto textureFood1 = LoadResource<Material>(L"QTEFoodTX");
		auto textureFood2 = LoadResource<Material>(L"QTEFoodTX2");

		auto textureProgressBT = LoadResource<Material>(L"ProgressKeyTX");
		auto textureKeyChainBT = LoadResource<Material>(L"KeyChainTX");
		auto textureDragBT = LoadResource<Material>(L"DragTX");
		auto textureWheelBT = LoadResource<Material>(L"WheelTX");

		model->m_materials.resize(meshCount);
		model->m_materials[0].push_back(textureBG);
		model->m_materials[1].push_back(textureFood1);
		model->m_materials[1].push_back(textureFood2);

		for (int i = 2; i < meshCount; i++)
		{
			model->m_materials[i].push_back(textureProgressBT);
			model->m_materials[i].push_back(textureDragBT);
			model->m_materials[i].push_back(textureKeyChainBT);
			model->m_materials[i].push_back(textureWheelBT);
		}

		auto vertexShader = LoadResource<VertexShader>(L"MiniGameVS");
		auto pixelShader = LoadResource<PixelShader>(L"MiniGamePS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::MiniGameUIMove);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::MiniGameProgress);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::MiniGameKeyChain);

		model->m_bindState = uiState;
	}

	// 맛집 체크리스트
	{
		auto model = LoadResource<Model>(L"EatSpotCheckUI");
		auto mesh = LoadResource<Mesh>(L"CanvasBuffer");

		for (int i = 0; i < 5; i++) model->m_meshes.push_back(mesh);
		auto texture1 = LoadResource<Material>(L"EatSpot_Empty_TX");
		auto texture2 = LoadResource<Material>(L"EatSpot_Fill_TX");

		model->m_materials.resize(5);
		for (int i = 0; i < 5; i++)
		{
			model->m_materials[i].push_back(texture1);
			model->m_materials[i].push_back(texture2);
		}

		auto vertexShader = LoadResource<VertexShader>(L"EatSpotVS");
		auto pixelShader = LoadResource<PixelShader>(L"EatSpotPS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::MiniGameUIMove);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::EatSpotUI);

		model->m_bindState = uiState;
	}

	// Timer UI
	{
		auto model = LoadResource<Model>(L"TimerUI");

		auto mesh = LoadResource<Mesh>(L"CanvasBuffer");
		model->m_meshes.push_back(mesh);

		auto textureBG = LoadResource<Material>(L"TimerBackgroundTX");
		auto textureMask = LoadResource<Material>(L"TimerMaskTX");
		auto textureOutline = LoadResource<Material>(L"TimerOutlineTX");
		auto textureSun = LoadResource<Material>(L"TimerSunTX");

		model->m_materials.resize(1);
		model->m_materials[0].push_back(textureOutline);
		model->m_materials[0].push_back(textureSun);
		model->m_materials[0].push_back(textureBG);
		model->m_materials[0].push_back(textureMask);

		auto vertexShader = LoadResource<VertexShader>(L"TimerVS");
		auto pixelShader = LoadResource<PixelShader>(L"TimerPS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;

		//???
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::TimerUI);

		model->m_bindState = uiState;
	}

	// MainUI
	{
		auto model = LoadResource<Model>(L"MainUI");

		auto mesh = LoadResource<Mesh>(L"CanvasBuffer");
		model->m_meshes.push_back(mesh);

		auto textureBG = LoadResource<Material>(L"MainBackgroundTX");
		auto textureExit = LoadResource<Material>(L"MainExitTX");
		auto textureCredit = LoadResource<Material>(L"MainCreditTX");

		model->m_materials.resize(1);
		model->m_materials[0].push_back(textureBG);
		model->m_materials[0].push_back(textureExit);
		model->m_materials[0].push_back(textureCredit);

		auto vertexShader = LoadResource<VertexShader>(L"MainUIVS");
		auto pixelShader = LoadResource<PixelShader>(L"MainUIPS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;

		model->m_pixelCbufferTypes.push_back(PixelCBufferType::MainUI);
		model->m_bindState = uiState;
	}

	// Score
	{
		auto model = LoadResource<Model>(L"ScoreUI");

		auto mesh = LoadResource<Mesh>(L"CanvasBuffer");
		model->m_meshes.push_back(mesh);

		auto textureBG = LoadResource<Material>(L"ScoreTX");
		model->m_materials.resize(1);
		model->m_materials[0].push_back(textureBG);

		auto vertexShader = LoadResource<VertexShader>(L"SingleImgUIVS");
		auto pixelShader = LoadResource<PixelShader>(L"SingleImgUIPS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::TimerUI);
		model->m_bindState = uiState;
	}

	// IngamePopup
	{
		auto model = LoadResource<Model>(L"IngamePopUpUI");

		auto mesh = LoadResource<Mesh>(L"CanvasBuffer");
		model->m_meshes.push_back(mesh);

		auto textureBG = LoadResource<Material>(L"InGamePopUpTX");
		model->m_materials.resize(1);
		model->m_materials[0].push_back(textureBG);

		auto vertexShader = LoadResource<VertexShader>(L"SingleImgUIVS");
		auto pixelShader = LoadResource<PixelShader>(L"SingleImgUIPS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::TimerUI);
		model->m_bindState = uiState;
	}

	// 상인 머리
	{
		auto model = LoadResource<Model>(L"MerchantUI");

		auto mesh = LoadResource<Mesh>(L"CanvasBuffer");
		model->m_meshes.push_back(mesh);

		auto texture0 = LoadResource<Material>(L"MeatFill");
		auto texture1 = LoadResource<Material>(L"AppleFill");
		auto texture2 = LoadResource<Material>(L"BagguettFill");
		auto texture3 = LoadResource<Material>(L"CrapFill");
		auto texture4 = LoadResource<Material>(L"CupcakeFill");

		auto texture5 = LoadResource<Material>(L"CooltimeBackground");
		auto texture6 = LoadResource<Material>(L"CooltimeGauge");

		auto texture7 = LoadResource<Material>(L"GaugeSensebg");
		auto texture8 = LoadResource<Material>(L"GaugeSense4");
		auto texture9 = LoadResource<Material>(L"GaugeSense1");
		auto texture10 = LoadResource<Material>(L"GaugeSense3");
		auto texture11 = LoadResource<Material>(L"GaugeSense2");

		auto texture12 = LoadResource<Material>(L"RestaurantBubble");
		auto texture13 = LoadResource<Material>(L"RestaurantBubbleDisabled");
		auto texture14 = LoadResource<Material>(L"MerchantMask");

		model->m_materials.resize(1);
		model->m_materials[0].push_back(texture0);
		model->m_materials[0].push_back(texture1);
		model->m_materials[0].push_back(texture2);
		model->m_materials[0].push_back(texture3);
		model->m_materials[0].push_back(texture4);
		model->m_materials[0].push_back(texture5);
		model->m_materials[0].push_back(texture6);
		model->m_materials[0].push_back(texture7); //넌 원래 없는게 맞니..?
		model->m_materials[0].push_back(texture8);
		model->m_materials[0].push_back(texture9);
		model->m_materials[0].push_back(texture10);
		model->m_materials[0].push_back(texture11);
		model->m_materials[0].push_back(texture12);
		model->m_materials[0].push_back(texture13);
		model->m_materials[0].push_back(texture14);

		model->m_materials[0].push_back(texture0);

		auto vertexShader = LoadResource<VertexShader>(L"SingleImgUIVS");
		auto pixelShader = LoadResource<PixelShader>(L"MerchantUIPS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = pixelShader;

		model->m_vertexCbufferTypes.push_back(VertexCBufferType::TimerUI);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::MerchantUI);

		model->m_bindState = uiState;
	}

#pragma endregion

#pragma region Prop

	// 바닥
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Floor", "Building/floor.fbx");
		//auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"SampleFloor", "sample_floor.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_hasShadow = false;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 벽
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Wall", "Building/wall.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 분수대
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Fountain", "Building/fountain.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 상자
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Box1", "Building/box1.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 통(수동 부쉬)
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Barrel", "Building/barrel.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 나무
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Tree1", "Building/tree1.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 잔디
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Grass", "Building/grass.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 가로등
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Lamp", "Building/lamp.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 음식 테이블
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"FoodTable", "Building/table.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 잔디(자동 부쉬)
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Bush", "Building/bush.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 물약
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Potion", "Building/potion.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 사탕
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Candy1", "Building/candy1.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 야외 테이블
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Table", "Building/table2.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 깃발
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Flag", "Building/flag.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 음식
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"SampleFood", "Building/food.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 캔들1
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Candle1", "Building/candle1.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// 캔들2
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"Candle2", "Building/candle2.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

#pragma endregion

#pragma region Character

	// [Character] 플레이어
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedSkinning>(L"Player", "Character/player.fbx");
		modelLoader->LoadAnimation(L"PlayerIdle", "Character/player_idle.fbx", AnimationState::Loop);
		modelLoader->LoadAnimation(L"PlayerEat", "Character/player_eat.fbx", AnimationState::Loop);
		modelLoader->LoadAnimation(L"PlayerLifting", "Character/player_lifting.fbx", AnimationState::Once);
		modelLoader->LoadAnimation(L"PlayerThrow", "Character/player_throw.fbx", AnimationState::Once);
		modelLoader->LoadAnimation(L"PlayerRun", "Character/player_run.fbx", AnimationState::Loop);
		auto vertexShader = LoadResource<VertexShader>(L"DefferedSkinnedVS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::BonePalette);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
		model->m_isPlayer = true;
	}

	// [Character] 상인
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedSkinning>(L"Merchant", "Character/merchant.fbx");
		modelLoader->LoadAnimation(L"MerchantIdle", "Character/merchant_idle.fbx", AnimationState::Loop);
		modelLoader->LoadAnimation(L"MerchantWalk", "Character/merchant_walk.fbx", AnimationState::Loop);
		modelLoader->LoadAnimation(L"MerchantShouting", "Character/merchant_shouting.fbx", AnimationState::Once);
		auto vertexShader = LoadResource<VertexShader>(L"DefferedSkinnedVS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::BonePalette);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
		model->m_isPlayer = true;
	}

	// [Character] 순찰병
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedSkinning>(L"Patrol", "Character/patrol.fbx");
		modelLoader->LoadAnimation(L"PatrolWalk", "Character/patrol_walk.fbx", AnimationState::Loop);
		modelLoader->LoadAnimation(L"PatrolRun", "Character/patrol_run.fbx", AnimationState::Loop);
		modelLoader->LoadAnimation(L"PatrolSet", "Character/patrol_set.fbx", AnimationState::Once);
		modelLoader->LoadAnimation(L"PatrolMiss", "Character/patrol_miss.fbx", AnimationState::Once);
		auto vertexShader = LoadResource<VertexShader>(L"DefferedSkinnedVS");
		model->m_vertexShader = vertexShader;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::BonePalette);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
		model->m_isPlayer = true;
	}

#pragma endregion

#pragma region Building

	// [Store] 과일 가게
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"FruitStore", "Building/fruitShop.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [Store] 컵케잌 가게
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"CupCakeStore", "Building/cupcakeShop.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [Store] 빵 가게
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"BreadStore", "Building/breadShop.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [Store] 해산물 가게
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"SeaFoodStore", "Building/seafoodShop.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

	// [Store] 정육점
	{
		auto model = modelLoader->LoadModel<VertexStruct::DefferedStatic>(L"ButcherStore", "Building/butcherShop.fbx");
		model->m_vertexShader = defferedDefaultVS;
		model->m_pixelShader = defferedDefaultPS;
		model->m_vertexCbufferTypes.push_back(VertexCBufferType::Transform);
		model->m_pixelCbufferTypes.push_back(PixelCBufferType::PBRMaterial);
	}

#pragma endregion


}