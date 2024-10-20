#pragma once

enum class FoodType
{
	None,

	Bagle,			// 베이글	(포인트) 빵
	BaguettePoint,	// 바게트	(포인트) 빵
	CupCake,		// 컵케이크	(포인트) 디저트
	Crab,			// 게		(포인트) 해산물

	Apple,			// 사과		(키체인) 과일
	CartoonMeat,	// 만화고기	(키체인) 육류

	Ham,			// 햄		(드래그 + 키체인) 육류
	WaterMelon,		// 수박		(드래그 + 키체인) 과일

	Fish,			// 생선		(드래그 + 포인트) 해산물
	Squid,			// 오징어	(드래그 + 포인트) 해산물
	Cake,			// 케이크	(드래그 + 포인트) 디저트
	Sausage,		// 소시지	(드래그 + 포인트) 육류

	BaguetteWheel,	// 바게트	(휠) 빵

	Banana,			// 바나나	(휠 + 포인트) 과일
};

enum class MiniGameType
{
	Point,
	Drag,
	KeyChain,
	Wheel,
};

enum class DragDirection
{
	Down,
	DownLeft,
	Left,
	UpLeft,
	Up,
	UpRight,
	Right,
	DownRight,
};

struct KeyChainUIpos
{
	float width = 1920/2;
	float height = 1080/2;
	float keyWidthSize = 100;
	float stride = 30;

	Vector2 one = { width, height };
	Vector2 two = { width - (stride/2 + keyWidthSize/2) , height};
	Vector2 three = { width - (stride + keyWidthSize), height};
	Vector2 four = { width - (stride / 2 + stride + keyWidthSize / 2 + keyWidthSize), height };
	Vector2 five = { width - (stride * 2 + keyWidthSize * 2), height };
};

struct MiniGameInfo
{
	MiniGameInfo() 
		: stage(0), progress(0), lastStage(0), lastProgress(0)
	{ }

	MiniGameInfo(int stage, int progress, int lastStage, int lastProgress)
		: stage(stage)
		, progress(progress)
		, lastStage(lastStage)
		, lastProgress(lastProgress)
	{}

	MiniGameInfo(const MiniGameInfo& other) = default;

	int stage;			// QTE 단계
	int progress;		// 단계별 진행도
	int lastStage;		// QTE 마지막 단계
	int lastProgress;	// 단계별 마지막 진행도
};