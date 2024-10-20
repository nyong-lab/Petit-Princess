#pragma once

#include "ISingleton.h"

#include "MinigameDefine.h"

/// <summary>
/// 모든 미니게임에 대한 모든 스테이지에 대한 정보들을 가지고 있는 클래스
/// 푸드테이블스크립트에서는 여기서 정의된 정보들을 복사해서 사용한다.
/// 또한 게임매니저에서 필요로 하는 미니게임에 클리어 여부도 체크 가능
/// 작성자 : 김동일
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
	
	//// 씬에 등록된 미니게임 추가 
	//void AddMiniGame(int id);

	//// 씬에서 파괴된 미니게임 파괴
	//void DestoryMiniGame(int id);

	//// 클리어된 미니게임 bool값 true
	//void SetClearMiniGame(int id);

	void ClearEatSpot(FoodType foodType) { m_clearEatSpot.push_back(foodType); }

	// 모든 미니게임 클리어 체크
	bool IsClearMiniGame();

private:
	//
	std::unordered_map<FoodType, std::vector<MiniGameInfo>> m_miniGameInformations;

	//// 미니게임 클리어 여부
	//// 푸드테이블이 생성되는 만큼 늘어나며 
	//// 푸드테이블이 클리어 되지않고 도중에 파괴되면 동적으로 해당 키값도 삭제시킨다.
	//// (실제로 존재하는 푸드테이블에 수 만큼 bool 값을 체크하기 위함)
	//std::unordered_map<int, bool> m_isMiniGamesClear;

	std::vector<FoodType> m_clearEatSpot;
};
