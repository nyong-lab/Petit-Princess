#include "GameEngine_pch.h"
#include "CollisionFilter.h"

CollisionFilter::CollisionFilter()
{
	InitializeCollisionFilter();
	InitializeQueryFilter();
}

void CollisionFilter::SetupFiltering(PxShape* shape, LayerCollision layer)
{
	PxFilterData filterData;
	filterData.word0 = static_cast<PxU32> (layer);		// word0 = own ID
	filterData.word1 = m_simulFilters[layer];		    // word1 = ID mask to filter pairs that trigger a

// 	PxFilterData queryData;
// 	queryData.word0 = static_cast<PxU32> (layer);
// 	queryData.word1 = m_queryFilters[layer];

	// contact callback;
	shape->setSimulationFilterData(filterData);
	//shape->setQueryFilterData(queryData);
}

void CollisionFilter::InitializeCollisionFilter()
{
	// 충돌되지 않아야 할 오브젝트를 여기에 넣으면 됩니다.
	m_simulFilters[LayerCollision::Default] = static_cast<PxU32>
		((UINT)LayerCollision::UI
			| (UINT)LayerCollision::OnlyPlayer);

	// UI
	m_simulFilters[LayerCollision::UI] = static_cast<PxU32>
		((UINT)LayerCollision::Default
			| (UINT)LayerCollision::Ground
			| (UINT)LayerCollision::Player
			| (UINT)LayerCollision::Interactable
			| (UINT)LayerCollision::Wall
			| (UINT)LayerCollision::OnlyPlayer);

	// 바닥
	m_simulFilters[LayerCollision::Ground] = static_cast<PxU32>
		((UINT)LayerCollision::UI
			| (UINT)LayerCollision::OnlyPlayer);

	// 플레이어
	m_simulFilters[LayerCollision::Player] = static_cast<PxU32>
		(
			(UINT)LayerCollision::UI
			| (UINT)LayerCollision::CallingRange);

	// 상호작용
	m_simulFilters[LayerCollision::Interactable] = static_cast<PxU32>
		((UINT)LayerCollision::UI 
			| (UINT)LayerCollision::Merchant
			| (UINT)LayerCollision::CallingRange
			| (UINT)LayerCollision::Wall
			| (UINT)LayerCollision::Interactable
			| (UINT)LayerCollision::Store
			| (UINT)LayerCollision::OnlyPlayer);

	// 벽
	m_simulFilters[LayerCollision::Wall] = static_cast<PxU32>
		((UINT)LayerCollision::UI
			| (UINT)LayerCollision::Wall
			| (UINT)LayerCollision::CallingRange
			| (UINT)LayerCollision::Merchant
			| (UINT)LayerCollision::Store
			| (UINT)LayerCollision::OnlyPlayer);

	// 상인의 호출범위
	m_simulFilters[LayerCollision::CallingRange] = static_cast<PxU32>
		(
			(UINT)LayerCollision::Ground
			| (UINT)LayerCollision::Interactable
			| (UINT)LayerCollision::CallingRange
			| (UINT)LayerCollision::Wall
			| (UINT)LayerCollision::Store
			| (UINT)LayerCollision::Merchant
			| (UINT)LayerCollision::Player
			| (UINT)LayerCollision::OnlyPlayer);

	// 상점
	m_simulFilters[LayerCollision::Store] = static_cast<PxU32>
		((UINT)LayerCollision::UI
			| (UINT)LayerCollision::Merchant
			| (UINT)LayerCollision::CallingRange
			| (UINT)LayerCollision::Wall
			| (UINT)LayerCollision::OnlyPlayer
			);

	// 상인
	m_simulFilters[LayerCollision::Merchant] = static_cast<PxU32>
		((UINT)LayerCollision::UI
			| (UINT)LayerCollision::Store
			| (UINT)LayerCollision::CallingRange
			| (UINT)LayerCollision::Wall
			| (UINT)LayerCollision::OnlyPlayer);

	// 경비병
	m_simulFilters[LayerCollision::Guard] = static_cast<PxU32>
		((UINT)LayerCollision::UI
			| (UINT)LayerCollision::Store
			| (UINT)LayerCollision::Merchant
			| (UINT)LayerCollision::OnlyPlayer);

	// 플레이어만 찾을 것
	m_simulFilters[LayerCollision::OnlyPlayer] = static_cast<PxU32>
		((UINT)LayerCollision::Default
			| (UINT)LayerCollision::UI
			| (UINT)LayerCollision::Ground
			| (UINT)LayerCollision::Interactable
			| (UINT)LayerCollision::Wall
			| (UINT)LayerCollision::CallingRange
			| (UINT)LayerCollision::Store
			| (UINT)LayerCollision::Merchant
			| (UINT)LayerCollision::Guard
			| (UINT)LayerCollision::OnlyPlayer);
}

void CollisionFilter::InitializeQueryFilter()
{
	// 씬 쿼리
	m_queryFilters[LayerCollision::Default] = static_cast<PxU32>
		((UINT)LayerCollision::Default);

	m_queryFilters[LayerCollision::UI] = static_cast<PxU32>
		((UINT)LayerCollision::UI);

	m_queryFilters[LayerCollision::Ground] = static_cast<PxU32>
		((UINT)LayerCollision::Ground);

	m_queryFilters[LayerCollision::Player] = static_cast<PxU32>
		((UINT)LayerCollision::Player);

	m_queryFilters[LayerCollision::Interactable] = static_cast<PxU32>
		((UINT)LayerCollision::Interactable);

	m_queryFilters[LayerCollision::Wall] = static_cast<PxU32>
		((UINT)LayerCollision::Wall);
}

