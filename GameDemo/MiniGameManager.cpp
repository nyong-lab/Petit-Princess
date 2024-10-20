#include "GameDemo_pch.h"
#include "MiniGameManager.h"


MiniGameManager::MiniGameManager()
{
	// �Ű����� ����
	// 1. ���� �������� (QTE �ܰ� )
	// 2. ���������� ���൵ 
	// 3. �� ��������
	// 4. ���������� �� ���൵ ��  

	// ���̱�
	m_miniGameInformations[FoodType::Bagle] = { { 1, 1, 3, 3 }, { 2, 1, 3, 3}, {3, 1, 3, 3} };
	//m_miniGameInformations[FoodType::Bagle] = { { 1, 1, 5, 4 }, { 2, 1, 5, 4}, {3, 1, 5, 4}, {4, 1, 5, 4}, {5, 1, 5, 4} };

	// �ٰ�Ʈ ����Ʈ
	m_miniGameInformations[FoodType::BaguettePoint] = { { 1, 1, 4, 4 }, { 2, 1, 4, 4}, {3, 1, 4, 4}, {4, 1, 4, 4} };

	// ������ũ
	m_miniGameInformations[FoodType::CupCake] = { { 1, 1, 2, 1 }, { 2, 1, 2, 1} };

	// ��
	m_miniGameInformations[FoodType::Crab] = { { 1, 1, 3, 1 }, { 2, 1, 3, 1}, { 3, 1, 3, 1} };

	// ���
	m_miniGameInformations[FoodType::Apple] = { { 1, 1, 3, 4 }, { 2, 1, 3, 4}, { 3, 1, 3, 4} };

	// ��ȭ���
	m_miniGameInformations[FoodType::CartoonMeat] = { { 1, 1, 4, 6 }, { 2, 1, 4, 6}, { 3, 1, 4, 6} };

	// ��
	m_miniGameInformations[FoodType::Ham] = { { 1, 1, 3, 8 }, { 2, 1, 3, 8}, { 3, 1, 3, 5} };

	// ����
	m_miniGameInformations[FoodType::WaterMelon] = { { 1, 1, 4, 4 }, { 2, 1, 4, 3}, { 3, 1, 4, 4}, {4, 1, 4, 5} };

	// ����
	m_miniGameInformations[FoodType::Fish] = { { 1, 1, 3, 1 }, { 2, 1, 3, 3}, { 3, 1, 3, 1} };

	// ��¡��
	m_miniGameInformations[FoodType::Squid] = { { 1, 1, 3, 3 }, { 2, 1, 3, 10}, { 3, 1, 3, 1} };

	// ����ũ
	m_miniGameInformations[FoodType::Cake] = { { 1, 1, 9, 4 }, { 2, 1, 9, 1}, { 3, 1, 9, 1}, { 4, 1, 9, 1},
	{ 5, 1, 9, 1}, { 6, 1, 9, 1}, { 7, 1, 9, 1}, { 8, 1, 9, 1}, { 9, 1, 9, 1} };

	// �ҽ���
	m_miniGameInformations[FoodType::Sausage] = { { 1, 1, 3, 2 }, { 2, 1, 3, 2}, { 3, 1, 3, 1} };

	// �ٰ�Ʈ �� (�̹����� ���൵�� ���� ���� �����;���)
	m_miniGameInformations[FoodType::BaguetteWheel] = { { 1, 1, 5, 16 }, { 2, 1, 5, 16}, { 3, 1, 5, 16}, { 4, 1, 5, 16}, {5, 1, 5, 16} };
	
	// �ٳ���
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
	// map ���� Ÿ�Կ� �ش��ϴ� ���������� �������� �����´�.
	auto& stages = m_miniGameInformations.at(type);

	// �Ű������� ���� N��° �������� ������ ������.
	// stage�� 1���� �����ϹǷ� �ε����� stage - 1
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
	//	// �ϳ��� �����ߴٸ� ��� �̴ϰ����� Ŭ���� ���� ���ѰŴ�.
	//	if (!iter.second)
	//	{
	//		return false;
	//	}
	//}

	//// ������� ����ߴٸ� All clear
	//return true;

	// ������ �� 5�� (�ߺ� x)
	// �� Ŭ����ī��Ʈ�� 5��� Ŭ���� 
	if (m_clearEatSpot.size() >= 5)
		return true;

	return false;
}
