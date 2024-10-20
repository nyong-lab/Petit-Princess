#pragma once

#include "ISingleton.h"

#include "MinigameDefine.h"

/// <summary>
/// ��� �̴ϰ��ӿ� ���� ��� ���������� ���� �������� ������ �ִ� Ŭ����
/// Ǫ�����̺�ũ��Ʈ������ ���⼭ ���ǵ� �������� �����ؼ� ����Ѵ�.
/// ���� ���ӸŴ������� �ʿ�� �ϴ� �̴ϰ��ӿ� Ŭ���� ���ε� üũ ����
/// �ۼ��� : �赿��
/// </summary>
class MiniGameManager : public ISingleton<MiniGameManager>
{
private:
	friend class ISingleton<MiniGameManager>;
	MiniGameManager();
	virtual ~MiniGameManager();

public:
	std::vector<MiniGameInfo> GetMiniGameInfo(FoodType type);
	MiniGameInfo GetStageInfo(FoodType type, int stage);
	MiniGameInfo GetStageInfo(std::vector<MiniGameInfo> miniGameInformations, int stage);
	std::vector<bool> GetClearEatSpotList()
	{
		std::vector<bool> list(5, false);
		for (auto iter : m_clearEatSpot)
		{
			if (iter == FoodType::Ham)				list[0] = true;
			if (iter == FoodType::Crab)				list[1] = true;
			if (iter == FoodType::Apple)			list[2] = true;
			if (iter == FoodType::CupCake)			list[3] = true;
			if (iter == FoodType::BaguetteWheel)	list[4] = true;
		}

		return list;
	}
	
	//// ���� ��ϵ� �̴ϰ��� �߰� 
	//void AddMiniGame(int id);

	//// ������ �ı��� �̴ϰ��� �ı�
	//void DestoryMiniGame(int id);

	//// Ŭ����� �̴ϰ��� bool�� true
	//void SetClearMiniGame(int id);

	void ClearEatSpot(FoodType foodType) { m_clearEatSpot.push_back(foodType); }

	// ��� �̴ϰ��� Ŭ���� üũ
	bool IsClearMiniGame();

private:
	//
	std::unordered_map<FoodType, std::vector<MiniGameInfo>> m_miniGameInformations;

	//// �̴ϰ��� Ŭ���� ����
	//// Ǫ�����̺��� �����Ǵ� ��ŭ �þ�� 
	//// Ǫ�����̺��� Ŭ���� �����ʰ� ���߿� �ı��Ǹ� �������� �ش� Ű���� ������Ų��.
	//// (������ �����ϴ� Ǫ�����̺� �� ��ŭ bool ���� üũ�ϱ� ����)
	//std::unordered_map<int, bool> m_isMiniGamesClear;

	std::vector<FoodType> m_clearEatSpot;
};
