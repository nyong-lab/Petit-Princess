#pragma once

enum class FoodType
{
	None,

	Bagle,			// ���̱�	(����Ʈ) ��
	BaguettePoint,	// �ٰ�Ʈ	(����Ʈ) ��
	CupCake,		// ������ũ	(����Ʈ) ����Ʈ
	Crab,			// ��		(����Ʈ) �ػ깰

	Apple,			// ���		(Űü��) ����
	CartoonMeat,	// ��ȭ���	(Űü��) ����

	Ham,			// ��		(�巡�� + Űü��) ����
	WaterMelon,		// ����		(�巡�� + Űü��) ����

	Fish,			// ����		(�巡�� + ����Ʈ) �ػ깰
	Squid,			// ��¡��	(�巡�� + ����Ʈ) �ػ깰
	Cake,			// ����ũ	(�巡�� + ����Ʈ) ����Ʈ
	Sausage,		// �ҽ���	(�巡�� + ����Ʈ) ����

	BaguetteWheel,	// �ٰ�Ʈ	(��) ��

	Banana,			// �ٳ���	(�� + ����Ʈ) ����
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

	int stage;			// QTE �ܰ�
	int progress;		// �ܰ躰 ���൵
	int lastStage;		// QTE ������ �ܰ�
	int lastProgress;	// �ܰ躰 ������ ���൵
};