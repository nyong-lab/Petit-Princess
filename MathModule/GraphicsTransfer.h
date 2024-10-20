#pragma once

#include "MathHeaders.h"

/// <summary>
/// 자원을 생성하기 위해 게임엔진 -> 그래픽스 엔진으로 
/// 데이터를 보낼 때 사용하는 클래스, 구조체들을 모아둔 곳
/// 
/// 24.01.21
/// 박연하
/// </summary>
namespace GraphicsTransfer
{
	enum class ModelKey
	{
		None,

		// Primitive ----------

		Grid,			//1
		TextureBox,		//2
		RedBox,			//3
		YellowBox,		//4
		PinkBox,		//5
		WireFrameBox,	//6
		WireFrameRay,	//7
		WireFrameSphere,//8
		Swablu,			//9
		SfBox,			//10
		Erika,			//11
		SampleTable,	//12
		SampleFood,		//13
		SampleStore,	//14
		
		// Character -------------

		Player,		//15
		Patrol,		//16
		Merchant,	//17

		// Prop ------------------

		Floor,		//18
		Wall,		//19
		Box1,		//20
		Box2,		//21
		Tree1,		//22
		Tree2,		//23
		Barrel,		//24
		Bush,		//25
		Fountain,	//26
		Grass,		//27
		Lamp,		//28
		Potion,		//29
		FoodTable,	//30

		// Building --------------

		BagleStore,			//31
		BaguettePointStore,	//32
		CupCakeStore,		//33
		CrabStore,			//34
		AppleStore,			//35
		CartoonMeatStore,	//36
		HamStore,			//37
		WaterMelonStore,	//38
		FishStore,			//39
		SquidStore,			//40
		CakeStore,			//41
		SausageStore,		//42
		BaguetteWheelStore, //43
		BananaStore,		//44

		// Food ------------------

		Meat,		//45
		Seafood,	//46
		Bread,		//47
		Fruit,		//48
		Dessert,	//49

		// QTE -------------------

		QTE_InteractiveKey, //50
		QTE_MiniGame,		//51

		// UI --------------------

		UI,					//52
		UIGauge,			//53
		UI_Timer,			//54
		UIHungry,			//55
		UI_Main,			//56
		UI_Sense,			//57
		UI_Score,			//58
		UI_Minimap,			//59
		UI_IngamePopUp,		//60
		UI_ReVisitCoolTime,	//61
		UI_DeliciousFood,	//62
		UI_MinimapCurPos,	//63
		UI_Merchant,		//64
		UI_EatSpot,			//65

		Candy,				//66
		Table,				//67
		Flag,				//68
		UI_MiniMapIcon,		//69
		UI_Skill,			//70
		UI_Black,			//71
		Candle1,			//72
		Candle2,			//73
		UI_IngameInterKey,
		UI_GuardFSM,
	};

	// ----------------------------------------------------

	// struct ---------------------------------------------

	/// <summary>
	/// 상수버퍼 데이터 전달 구조체
	/// </summary>

	struct CbTransform
	{
		Matrix world;
		Matrix cameraWorld;
		Matrix viewProj;
		Matrix view;
		Matrix proj;
	};

	struct UITransform
	{
		Matrix world;
		Matrix worldViewProj;
		Matrix worldInv;
	};

	struct CbMaterial
	{
		bool useMap[5] = { false, false, false, false, false };
		Vector4 baseColor;
		float metallic;
		float roughness;
		float AO;
	};

	struct CbUI
	{
		float textureTranslation;
		int typeNum;
		int typeNum2;
		bool isActive;
		int stage;
		int progress;
		int meshCount;
		int direction;
		Vector2 pos;
		Vector2 size;
		std::vector<int> keyChains;
		bool isActive2;
		bool isFirst;
		std::vector<bool> eatFoods;
		float fade;

	};

	struct CbPhysics
	{
		int isCollision;
	};

	// UI랑 쪼개고싶지만
	// 일단은 인터페이스 통일을 위해..
	struct ModelData
	{
		std::wstring modelKey;
		CbTransform transform;
		CbMaterial meterial;
		CbPhysics physics;

		// ---------------------

		UITransform uiTransform;
		CbUI ui;
	};

	struct DirectionalLightData
	{
		Vector4 direction = Vector4(0.f, -1.f, 0.5f, 1.f);
		Vector3 color = Vector3(1.f, 1.f, 1.f);
		float intensity = 0.f;
		Vector4 eyePos;
		Vector4 ambient = Vector4(1.f, 1.f, 1.f, 0.f);
		Matrix gShadowTransform;
	};

	struct PointLightData
	{
		Vector4 color = Vector4(1.f, 1.f, 1.f, 1.f);
		Vector3 position;
		float intensity;
		Vector3 attenuation;
		float range;
	};

	struct LightWorldProperty
	{
		float bloomIntensity;
		float bloomThreshold;
		float tonemapThreshold;

		float pad;
	};

	struct Text
	{
		std::wstring text;
		Vector2 pos;
		Vector4 color;
		Vector2 boxSize;
		UINT fontSize;
	};


}

enum class VertexCBufferType
{
	Transform,
	UITransform,
	BonePalette,
	MiniGameUIMove,
	TimerUI,
};

enum class PixelCBufferType
{
	PBRMaterial,
	TextureTransform,
	CollisionCheck,
	MiniGameButton,
	MiniGameProgress,
	MiniGameKeyChain,
	MainUI,
	MerchantUI,
	EatSpotUI,
};

typedef std::pair<std::wstring, VertexCBufferType> VertexCBInfo;
typedef std::pair<std::wstring, PixelCBufferType> PixelCBInfo;