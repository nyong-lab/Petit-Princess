#include "GameDemo_pch.h"
#include "MiniGameManager.h"


MiniGameManager::MiniGameManager()
{
	// 매개변수 역할
	// 1. 현재 스테이지 (QTE 단계 )
	// 2. 스테이지별 진행도 
	// 3. 총 스테이지
	// 4. 스테이지별 총 진행도 수  

	// 베이글
	m_miniGameInformations[FoodType::Bagle] = { { 1, 1, 3, 3 }, { 2, 1, 3, 3}, {3, 1, 3, 3} };
	//m_miniGameInformations[FoodType::Bagle] = { { 1, 1, 5, 4 }, { 2, 1, 5, 4}, {3, 1, 5, 4}, {4, 1, 5, 4}, {5, 1, 5, 4} };

	// 바게트 포인트
	m_miniGameInformations[FoodType::BaguettePoint] = { { 1, 1, 4, 4 }, { 2, 1, 4, 4}, {3, 1, 4, 4}, {4, 1, 4, 4} };

	// 컵케이크
	m_miniGameInformations[FoodType::CupCake] = { { 1, 1, 2, 1 }, { 2, 1, 2, 1} };

	// 게
	m_miniGameInformations[FoodType::Crab] = { { 1, 1, 3, 1 }, { 2, 1, 3, 1}, { 3, 1, 3, 1} };

	// 사과
	m_miniGameInformations[FoodType::Apple] = { { 1, 1, 3, 4 }, { 2, 1, 3, 4}, { 3, 1, 3, 4} };

	// 만화고기
	m_miniGameInformations[FoodType::CartoonMeat] = { { 1, 1, 4, 6 }, { 2, 1, 4, 6}, { 3, 1, 4, 6} };

	// 햄
	m_miniGameInformations[FoodType::Ham] = { { 1, 1, 3, 8 }, { 2, 1, 3, 8}, { 3, 1, 3, 5} };

	// 수박
	m_miniGameInformations[FoodType::WaterMelon] = { { 1, 1, 4, 4 }, { 2, 1, 4, 3}, { 3, 1, 4, 4}, {4, 1, 4, 5} };

	// 생선
	m_miniGameInformations[FoodType::Fish] = { { 1, 1, 3, 1 }, { 2, 1, 3, 3}, { 3, 1, 3, 1} };

	// 오징어
	m_miniGameInformations[FoodType::Squid] = { { 1, 1, 3, 3 }, { 2, 1, 3, 10}, { 3, 1, 3, 1} };

	// 케이크
	m_miniGameInformations[FoodType::Cake] = { { 1, 1, 9, 4 }, { 2, 1, 9, 1}, { 3, 1, 9, 1}, { 4, 1, 9, 1},
	{ 5, 1, 9, 1}, { 6, 1, 9, 1}, { 7, 1, 9, 1}, { 8, 1, 9, 1}, { 9, 1, 9, 1} };

	// 소시지
	m_miniGameInformations[FoodType::Sausage] = { { 1, 1, 3, 2 }, { 2, 1, 3, 2}, { 3, 1, 3, 1} };

	// 바게트 휠 (이미지가 진행도에 따라서 점점 내려와야함)
	m_miniGameInformations[FoodType::BaguetteWheel] = { { 1, 1, 5, 16 }, { 2, 1, 5, 16}, { 3, 1, 5, 16}, { 4, 1, 5, 16}, {5, 1, 5, 16} };
	
	// 바나나
	m_miniGameInformations[FoodType::Banana] = { { 1, 1, 5, 16 }, { 2, 1, 5, 16}, { 3, 1, 5, 16}, { 4, 1, 5, 1}, {5, 1, 5, 1} };
}

MiniGameManager::~MiniGameManager()
{
	m_miniGameInformations.clear();
}

std::vector<MiniGameInfo> MiniGameManager::GetMiniGameInfo(FoodType type)
{
	return m_miniGameInformations.at(type);
}

MiniGameInfo MiniGameManager::GetStageInfo(FoodType type, int stage)
{
	// map 에서 타입에 해당하는 스테이지별 정보들을 꺼내온다.
	auto& stages = m_miniGameInformations.at(type);

	// 매개변수로 들어온 N번째 스테이지 정보를 꺼낸다.
	// stage는 1부터 시작하므로 인덱스는 stage - 1
	return stages.at(stage - 1);
}

MiniGameInfo MiniGameManager::GetStageInfo(std::vector<MiniGameInfo> miniGameInformations, int stage)
{
	return miniGameInformations.at(stage - 1);
}

//void MiniGameManager::AddMiniGame(int id)
//{
//	m_isMiniGamesClear[id] = false;
//}
//
//void MiniGameManager::DestoryMiniGame(int id)
//{
//	m_isMiniGamesClear.erase(id);
//}
//
//void MiniGameManager::SetClearMiniGame(int id)
//{
//	auto iter = m_isMiniGamesClear.find(id);
//
//	if (iter != m_isMiniGamesClear.end())
//		iter->second = true;
//}

bool MiniGameManager::IsClearMiniGame()
{
	//for (const auto& iter : m_isMiniGamesClear)
	//{
	//	// 하나라도 실패했다면 모든 미니게임을 클리어 하지 못한거다.
	//	if (!iter.second)
	//	{
	//		return false;
	//	}
	//}

	//// 여기까지 통과했다면 All clear
	//return true;

	// 맛집은 총 5개 (중복 x)
	// 즉 클리어카운트가 5라면 클리어 
	if (m_clearEatSpot.size() >= 5)
		return true;

	return false;
}
