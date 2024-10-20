#include "GameDemo_pch.h"
#include "QuickTimeEventScript.h"

#include "../GameEngine/InputManager.h"
#include "SoundManager.h"

#include "Transform.h"

#include <random>


void QuickTimeEventScript::Initialize(std::shared_ptr<MiniGameInfo> miniGameInfo, FoodType foodType, double* merchantSense)
{
	m_miniGameInfo = miniGameInfo;
	m_foodType = foodType;
	m_merchantSense = merchantSense;
}

void QuickTimeEventScript::SetMiniGameInfo(std::shared_ptr<MiniGameInfo> miniGameInfo)
{
	m_miniGameInfo = miniGameInfo;

	// 반드시 초기화 시켜줘야 한다.
	m_keychains.clear();
}
void QuickTimeEventScript::Update(double dt)
{
	switch (m_foodType)
	{
		/// 베이글
	case FoodType::Bagle:
	{
		Bagle();
		break;
	}

	/// 바게트 (포인트)
	case FoodType::BaguettePoint:
	{
		BaguettePoint();
		break;
	}

	/// 컵케이크
	case FoodType::CupCake:
	{
		CupCake();
		break;
	}

	/// 게
	case FoodType::Crab:
	{
		Crab();
		break;
	}

	/// 사과
	case FoodType::Apple:
	{
		Apple();
		break;
	}

	/// 만화고기
	case FoodType::CartoonMeat:
	{
		CartoonMeat();
		break;
	}

	/// 햄
	case FoodType::Ham:
	{
		Ham();
		break;
	}

	/// 수박
	case FoodType::WaterMelon:
	{
		WaterMelon();
		break;
	}

	/// 생선
	case FoodType::Fish:
	{
		Fish();
		break;
	}

	/// 오징어
	case FoodType::Squid:
	{
		Squid();
		break;
	}

	/// 케이크
	case FoodType::Cake:
	{
		Cake();
		break;
	}

	/// 소시지
	case FoodType::Sausage:
	{
		Sausage();
		break;
	}

	/// 바게트 (휠)
	case FoodType::BaguetteWheel:
	{
		BaguetteWheel();
		break;
	}

	/// 바나나
	case FoodType::Banana:
	{
		Banana();
		break;
	}

	// 예외처리
	default:
		assert(!"미니게임 분기 늘려야 합니다. -동일");
	}

	// 포인트 클릭 실패시 눈치 게이지
	if (m_isMistakePoint)
	{
		m_isMistakePoint = false;

		if (m_merchantSense)
			*m_merchantSense += 8;
		else
			assert(!"상인 Sense -> 푸드 테이블 스크립트 연동 해야합니다.");
	}

	// 키 체인 실패시 눈치 게이지
	if (m_isMistakeKeyChain)
	{
		m_isMistakeKeyChain = false;

		if (m_merchantSense)
			*m_merchantSense += 10;
		else
			assert(!"상인 Sense -> 푸드 테이블 스크립트 연동 해야합니다.");
	}
}

void QuickTimeEventScript::Render(double dt)
{

}

std::shared_ptr<Component> QuickTimeEventScript::Clone() const
{
	std::shared_ptr<QuickTimeEventScript> clone = std::make_shared<QuickTimeEventScript>();

	return clone;
}

void QuickTimeEventScript::CalculateButtonRange(Vector2 pos)
{
	m_leftTop = Vector2(pos.x - m_uiSize.x / 2, pos.y - m_uiSize.y / 2);
	m_rightBottom = Vector2(pos.x + m_uiSize.x / 2, pos.y + m_uiSize.y / 2);
}

/// <summary>
/// 베이글 (포인트)
/// </summary>
void QuickTimeEventScript::Bagle()
{
	// 베이글은 모든 스테이지 및 단계가 Point 타입
	SetMiniGameType(MiniGameType::Point);

	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_uiPosition = { 1088, 337 };

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1088, 337 });
				CheckPoint({979, 330});
			}
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 979, 330 });
				CheckPoint({ 1075, 444 });
			}
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1075, 444 });
				CheckPoint({ 854, 342 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 854, 342 });
				CheckPoint({ 1036, 470 });
			}
		}
		// 2-2
		else if (m_miniGameInfo->progress == 2)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1036, 470 });
				CheckPoint({ 941, 476 });
			}
		}
		// 2-3
		else if (m_miniGameInfo->progress == 3)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 941, 476 });
				CheckPoint({ 790, 453 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 790, 453 });
				CheckPoint({ 908, 562 });
			}
		}
		// 3-2
		else if (m_miniGameInfo->progress == 2)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 908, 562 });
				CheckPoint({ 812, 521 });
			}
		}
		// 3-3
		else if (m_miniGameInfo->progress == 3)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 812, 521 });
				CheckPoint({0,0});
			}
		}
		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 바게트 (포인트)
/// </summary>
void QuickTimeEventScript::BaguettePoint()
{
	// 바게트(포인트)는  모든 스테이지 및 단계가 Point 타입
	SetMiniGameType(MiniGameType::Point);

	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_uiPosition = { 1321, 489 };

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1321, 489 });
				CheckPoint({ 1285, 532 });
			}
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1285, 532 });
				CheckPoint({ 1227, 447 });
			}
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1227, 447 });
				CheckPoint({ 1221, 490 });
			}
		}
		// 1-4
		else if (m_miniGameInfo->progress == 4)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1221, 490 });
				CheckPoint({ 1130, 481 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1130, 481 });
				CheckPoint({ 1076, 427 });
			}
		}
		// 2-2
		else if (m_miniGameInfo->progress == 2)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1076, 427 });
				CheckPoint({ 1023, 514 });
			}
		}
		// 2-3
		else if (m_miniGameInfo->progress == 3)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1023, 514 });
				CheckPoint({ 984, 430 });
			}
		}
		// 2-4
		else if (m_miniGameInfo->progress == 4)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 984, 430 });
				CheckPoint({ 927, 459 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 927, 459 });
				CheckPoint({ 850, 522 });
			}
		}
		// 3-2
		else if (m_miniGameInfo->progress == 2)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 850, 522 });
				CheckPoint({ 798, 438 });
			}
		}
		// 3-3
		else if (m_miniGameInfo->progress == 3)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 798, 438 });
				CheckPoint({ 777, 499 });
			}
		}
		// 3-4
		else if (m_miniGameInfo->progress == 4)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{

				CalculateButtonRange({ 777, 499 });
				CheckPoint({ 664, 494 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 4
	else if (m_miniGameInfo->stage == 4)
	{
		// 4-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{

				CalculateButtonRange({ 664, 494 });
				CheckPoint({ 616, 528 });
			}
		}
		// 4-2
		else if (m_miniGameInfo->progress == 2)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 616, 528 });
				CheckPoint({ 603, 458 });
			}
		}
		// 4-3
		else if (m_miniGameInfo->progress == 3)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 603, 458 });
				CheckPoint({ 561, 496 });
			}
		}
		// 4-4
		else if (m_miniGameInfo->progress == 4)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 561, 496 });
				CheckPoint({0,0});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 컵케이크 (포인트)
/// </summary>
void QuickTimeEventScript::CupCake()
{
	// 컵케이크는 모든 스테이지 및 단계가 Point 타입
	SetMiniGameType(MiniGameType::Point);

	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		m_uiPosition = { 998, 394 };

		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 998, 394 });
				CheckPoint({ 872, 568 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 872, 568 });
				CheckPoint({0,0});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 게 (포인트)
/// </summary>
void QuickTimeEventScript::Crab()
{
	// 게는 모든 스테이지 및 단계가 Point 타입
	SetMiniGameType(MiniGameType::Point);

	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_uiPosition = { 1121, 443 };

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1121, 443 });
				CheckPoint({ 862, 506 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 862, 506 });
				CheckPoint({ 860, 578 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 860, 578 });
				CheckPoint({0,0});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 사과 (키 체인)
/// </summary>
void QuickTimeEventScript::Apple()
{
	// 사과는 모든 스테이지 및 단계가 KeyChain 타입
	SetMiniGameType(MiniGameType::KeyChain);

	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(4);

		m_uiPosition = m_keychainsPosInfo.four;

		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 1-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(4);

		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 2-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 2-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 2-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(4);

		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 3-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 3-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 3-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 만화고기 (키 체인) - 후순위(UI 미구현) 
/// </summary>
void QuickTimeEventScript::CartoonMeat()
{
	// 만화고기는 모든 스테이지 및 단계가 KeyChain 타입
	SetMiniGameType(MiniGameType::KeyChain);

	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(6);

		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 1-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}
		// 1-5
		else if (m_miniGameInfo->progress == 5)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(4);
		}
		// 1-6
		else if (m_miniGameInfo->progress == 6)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(5);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(6);

		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 2-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 2-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 2-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}
		// 2-5
		else if (m_miniGameInfo->progress == 5)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(4);
		}
		// 2-6
		else if (m_miniGameInfo->progress == 6)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(5);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(6);

		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 3-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 3-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 3-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}
		// 3-5
		else if (m_miniGameInfo->progress == 5)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(4);
		}
		// 3-6
		else if (m_miniGameInfo->progress == 6)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(5);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 4
	else if (m_miniGameInfo->stage == 4)
	{
		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(6);

		// 4-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 4-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 4-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 4-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}
		// 4-5
		else if (m_miniGameInfo->progress == 5)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(4);
		}
		// 4-6
		else if (m_miniGameInfo->progress == 6)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(5);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 햄 (드래그 + 키 체인)
/// </summary>
void QuickTimeEventScript::Ham()
{
	float height = 470.f;

	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		SetMiniGameType(MiniGameType::Drag);
		m_uiPosition = { 1150, height };

		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1145, 298 });
			m_uiPosition = { 1135, height };
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1130, 298 });
			m_uiPosition = { 1120, height };
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1115, 298 });
			m_uiPosition = { 1105, height };
		}
		// 1-4
		else if (m_miniGameInfo->progress == 4)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1100, 298 });
			m_uiPosition = { 1090, height };

		}
		// 1-5
		else if (m_miniGameInfo->progress == 5)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1085, 298 });
			m_uiPosition = { 1075, height };

		}
		// 1-6
		else if (m_miniGameInfo->progress == 6)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1050, 298 });
			m_uiPosition = { 1060, height };

		}
		// 1-7
		else if (m_miniGameInfo->progress == 7)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1035, 298 });
			m_uiPosition = { 1045, height };

		}
		// 1-8
		else if (m_miniGameInfo->progress == 8)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 1020, 298 });
			m_uiPosition = { 1030, height };
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		SetMiniGameType(MiniGameType::Drag);
		m_uiPosition = { 900, height };

		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 875, 298 });
			m_uiPosition = { 885, height };
		}
		// 2-2
		else if (m_miniGameInfo->progress == 2)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 880, 298 });
			m_uiPosition = { 870, height };
		}
		// 2-3
		else if (m_miniGameInfo->progress == 3)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 865, 298 });
			m_uiPosition = { 855, height };
		}
		// 2-4
		else if (m_miniGameInfo->progress == 4)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 850, 298 });
			m_uiPosition = { 840, height };
		}
		// 2-5
		else if (m_miniGameInfo->progress == 5)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 815, 298 });
			m_uiPosition = {825, height };
		}
		// 2-6
		else if (m_miniGameInfo->progress == 6)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 800, 298 });
			m_uiPosition = {810, height };
		}
		// 2-7
		else if (m_miniGameInfo->progress == 7)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 785, 298 });
			m_uiPosition = {795, height };
		}
		// 2-8
		else if (m_miniGameInfo->progress == 8)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 770, 298 });
			m_uiPosition = { 780, height };
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		SetMiniGameType(MiniGameType::KeyChain);

		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(5);

		m_uiPosition = m_keychainsPosInfo.five;

		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 3-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 3-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 3-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}
		// 3-5
		else if (m_miniGameInfo->progress == 5)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(4);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 수박 (드래그 + 키 체인)
/// </summary>
void QuickTimeEventScript::WaterMelon()
{
	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		SetMiniGameType(MiniGameType::Drag);
		m_uiPosition = { 955, 480 };

		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_direction = DragDirection::Down;
			CheckDrag({ 954,312 });
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			m_direction = DragDirection::Right;
			CheckDrag({ 766, 477 });
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			m_direction = DragDirection::DownRight;
			CheckDrag({ 836, 359 });
		}
		// 1-4
		else if (m_miniGameInfo->progress == 4)
		{
			m_direction = DragDirection::DownLeft;
			CheckDrag({ 1074 ,362 });
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		SetMiniGameType(MiniGameType::KeyChain);

		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(3);

		m_uiPosition = m_keychainsPosInfo.three;

		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 3-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 3-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		SetMiniGameType(MiniGameType::KeyChain);

		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(4);

		m_uiPosition = m_keychainsPosInfo.four;

		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 3-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 3-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 3-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 4
	else if (m_miniGameInfo->stage == 4)
	{
		SetMiniGameType(MiniGameType::KeyChain);

		// 랜덤 키 체인 생성
		if (m_keychains.size() == 0)
			MakeRandomKeyChains(5);

		m_uiPosition = m_keychainsPosInfo.five;

		// 4-1
		if (m_miniGameInfo->progress == 1)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(0);
		}
		// 4-2
		else if (m_miniGameInfo->progress == 2)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(1);
		}
		// 4-3
		else if (m_miniGameInfo->progress == 3)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(2);
		}
		// 4-4
		else if (m_miniGameInfo->progress == 4)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(3);
		}
		// 4-5
		else if (m_miniGameInfo->progress == 5)
		{
			// 키 체인에 해당하는 마우스 클릭 성공 여부
			CheckKeyChain(4);
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 생선 (드래그 + 포인트)
/// </summary>
void QuickTimeEventScript::Fish()
{
	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		SetMiniGameType(MiniGameType::Drag);
		
		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_uiPosition = { 750, 500 };
			m_direction = DragDirection::Down;
			CheckDrag({ 751, 343 });
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		SetMiniGameType(MiniGameType::Drag);
		
		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			m_uiPosition = { 1040, 420 };
			m_direction = DragDirection::Right;
			CheckDrag({ 876, 418 });
		}
		// 2-2
		else if (m_miniGameInfo->progress == 2)
		{
			m_uiPosition = { 1040, 480 };
			m_direction = DragDirection::Right;
			CheckDrag({ 876, 478 });
		}
		// 2-3
		else if (m_miniGameInfo->progress == 3)
		{
			m_uiPosition = { 1040, 540 };
			m_direction = DragDirection::Right;
			CheckDrag({ 876, 538 });
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		SetMiniGameType(MiniGameType::Point);
		m_uiPosition = { 951, 407 };
		
		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 951, 407 });
				CheckPoint({0,0});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 오징어 (드래그 + 포인트) - 후순위(UI 미구현) 
/// </summary>
void QuickTimeEventScript::Squid()
{
	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		SetMiniGameType(MiniGameType::Drag);

		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		SetMiniGameType(MiniGameType::Drag);

		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			m_direction = DragDirection::DownRight;
			//CheckDrag({ XXX, YYY });
		}
		// 2-2
		else if (m_miniGameInfo->progress == 2)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-3
		else if (m_miniGameInfo->progress == 3)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-4
		else if (m_miniGameInfo->progress == 4)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-5
		else if (m_miniGameInfo->progress == 5)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-6
		else if (m_miniGameInfo->progress == 6)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-7
		else if (m_miniGameInfo->progress == 7)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-8
		else if (m_miniGameInfo->progress == 8)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-9
		else if (m_miniGameInfo->progress == 9)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 2-10
		else if (m_miniGameInfo->progress == 10)
		{
			m_direction = DragDirection::UpRight;
			//CheckDrag({ XXX, YYY });
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 케이크 (드래그 + 포인트) - 후순위(UI 미구현) 
/// </summary>
void QuickTimeEventScript::Cake()
{
	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		SetMiniGameType(MiniGameType::Drag);

		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			m_direction = DragDirection::Down;
			//CheckDrag({ XXX, YYY });
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			m_direction = DragDirection::Right;
			//CheckDrag({ XXX, YYY });
		}
		// 1-3
		else if (m_miniGameInfo->progress == 3)
		{
			m_direction = DragDirection::DownRight;
			//CheckDrag({ XXX, YYY });
		}
		// 1-4
		else if (m_miniGameInfo->progress == 4)
		{
			m_direction = DragDirection::DownLeft;
			//CheckDrag({ XXX, YYY });
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 4
	else if (m_miniGameInfo->stage == 3)
	{
		// 4-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 5
	else if (m_miniGameInfo->stage == 3)
	{
		// 5-1
		if (m_miniGameInfo->progress == 1)
		{
			m_miniGameType = MiniGameType::Point;

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 6
	else if (m_miniGameInfo->stage == 3)
	{
		// 6-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 7
	else if (m_miniGameInfo->stage == 3)
	{
		// 7-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{


				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 8
	else if (m_miniGameInfo->stage == 3)
	{
		// 8-1
		if (m_miniGameInfo->progress == 1)
		{
			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}
	}
	/// 스테이지 9
	else if (m_miniGameInfo->stage == 3)
	{
		// 9-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				//CalculateRange({ 마우스 좌표 리소스 없어서 못했다. });
				//CheckPoint();
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 소시지 (드래그 + 포인트)
/// </summary>
void QuickTimeEventScript::Sausage()
{
	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 1-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Drag);

			m_uiPosition = { 1080, 520 };
			m_direction = DragDirection::Down;
			CheckDrag({ 1077, 353 });
		}
		// 1-2
		else if (m_miniGameInfo->progress == 2)
		{
			SetMiniGameType(MiniGameType::Point);
			m_uiPosition = { 1185, 523 };

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 1185, 523 });
				CheckPoint({0,0});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Drag);

			m_uiPosition = { 900, 520 };
			m_direction = DragDirection::Down;
			CheckDrag({ 897, 354 });
		}
		// 2-1
		else if (m_miniGameInfo->progress == 2)
		{
			SetMiniGameType(MiniGameType::Point);
			m_uiPosition = { 960, 546 };

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 960, 546 });
				CheckPoint({785,562});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 785,562 });
				CheckPoint({0,0});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 바게트 (휠) - 후순위(UI 미구현) 
/// </summary>
void QuickTimeEventScript::BaguetteWheel()
{
	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 1-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);
			m_uiPosition = { 1100, 460 };

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 4
	else if (m_miniGameInfo->stage == 4)
	{
		// 4-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 5
	else if (m_miniGameInfo->stage == 5)
	{
		// 5-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

/// <summary>
/// 바나나 (휠 + 포인트)
/// </summary>
void QuickTimeEventScript::Banana()
{
	/// 스테이지 1
	if (m_miniGameInfo->stage == 1)
	{
		// 1-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);
			m_uiPosition = { 1100, 460 };

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 2
	else if (m_miniGameInfo->stage == 2)
	{
		// 2-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	/// 스테이지 3
	else if (m_miniGameInfo->stage == 3)
	{
		// 3-1
		if (m_miniGameInfo->progress >= 1 && m_miniGameInfo->progress <= 16)
		{
			SetMiniGameType(MiniGameType::Wheel);

			if (InputManager::GetInstance()->GetWheelState() == WheelState::Down)
				++m_miniGameInfo->progress;
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 4
	else if (m_miniGameInfo->stage == 4)
	{
		// 4-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);
			m_uiPosition = { 893, 343 };

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 893, 343 });
				CheckPoint({ 958, 605 });
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}
	/// 스테이지 5
	else if (m_miniGameInfo->stage == 5)
	{
		// 5-1
		if (m_miniGameInfo->progress == 1)
		{
			SetMiniGameType(MiniGameType::Point);

			if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
			{
				CalculateButtonRange({ 958, 605 });
				CheckPoint({0,0});
			}
		}

		else
			assert(!"progress 값이 이상합니다. -동일");
	}

	else
		assert(!"stage 값이 이상합니다. -동일");
}

void QuickTimeEventScript::SetMiniGameType(MiniGameType type)
{
	m_miniGameType = type;

	switch (type)
	{
	case MiniGameType::Point:
		m_uiSize = Vector2(70.f, 70.f);
		break;
	case MiniGameType::Drag:
		m_uiSize = Vector2(350.f, 350.f);
		break;
	case MiniGameType::KeyChain:
		m_uiSize = Vector2(100.f, 100.f);
		break;
	case MiniGameType::Wheel:
		m_uiSize = Vector2(70.f, 105.f);
		break;
	default:
		break;
	}
}

void QuickTimeEventScript::MakeRandomKeyChains(int keyChainNum)
{
	m_keychains.clear();

	// 시드값을 얻기 위한 random_device 생성.
	std::random_device rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	std::mt19937 gen(rd());

	// 1 부터 4 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
	std::uniform_int_distribution<int> dis(0, 1);

	for (int i = 0; i < keyChainNum; ++i)
	{
		m_keychains.push_back(dis(gen));
	}
}

void QuickTimeEventScript::CheckKeyChain(int keyChainLocation)
{
	// 좌 클릭
	if (m_keychains[keyChainLocation] == 0)
	{
		if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
		{
			SoundManager::GetInstance()->PlaySFX("QTE_Eat 1.wav", false);
			m_keychains[keyChainLocation] = 2;
			++m_miniGameInfo->progress;
		}
		else if (InputManager::GetInstance()->IsKeyDown(VK_RBUTTON))
		{
			m_isMistakeKeyChain = true;
		}
	}

	// 우 클릭
	else if (m_keychains[keyChainLocation] == 1)
	{
		if (InputManager::GetInstance()->IsKeyDown(VK_RBUTTON))
		{
			SoundManager::GetInstance()->PlaySFX("QTE_Eat 2.wav", false);
			m_keychains[keyChainLocation] = 2;
			++m_miniGameInfo->progress;
		}
		else if (InputManager::GetInstance()->IsKeyDown(VK_LBUTTON))
		{
			m_isMistakeKeyChain = true;
		}
	}

	else
		assert(!"progress에 알맞지 않은 keyChain 배열에 접근했습니다. - 동일");
}

void QuickTimeEventScript::CheckPoint(Vector2 nextUIpos)
{
	Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();

	// 마우스 포지션이 해당하는 버튼 범위에 들어가면 성공
	if (mousePos.x > m_leftTop.x && mousePos.x < m_rightBottom.x && mousePos.y > m_leftTop.y && mousePos.y < m_rightBottom.y)
	{
		static bool soundEven = false;

		if (soundEven)
		{
			SoundManager::GetInstance()->PlaySFX("QTE_Eat 2.wav", false);
			soundEven = false;
		}
		else
		{
			SoundManager::GetInstance()->PlaySFX("QTE_Eat 1.wav", false);
			soundEven = true;
		}

		++m_miniGameInfo->progress;
		m_uiPosition = nextUIpos;
	}

	// 미니게임 UI창 내부에서 클릭해야 하는 위치가 아닌 곳을 클릭했을 때, 눈치 게이지가 오른다.
	else if (mousePos.x > 210 && mousePos.x < 1710 && mousePos.y > 120 && mousePos.y < 960)
	{
		SoundManager::GetInstance()->PlaySFX("QTE_Wrong.mp3", false);
		m_isMistakePoint = true;
	}
}

bool QuickTimeEventScript::CheckDragStartPoint(Vector2 startDragPos)
{
	// 마우스 포지션이 해당하는 버튼 범위에 들어가면 성공
	if (startDragPos.x > m_leftTop.x && startDragPos.x < m_rightBottom.x && startDragPos.y > m_leftTop.y && startDragPos.y < m_rightBottom.y)
		return true;
	else
		return false;
}

void QuickTimeEventScript::CheckDrag(Vector2 directuonStartPos)
{
	// 드래그 시작 위치가 잘못되었다면 드래그를 중지할때까지 바로 리턴시킨다.
	if (m_isFailedStartPoint)
	{
		if (InputManager::GetInstance()->IsKeyUp(VK_LBUTTON))
			m_isFailedStartPoint = false;
		else
			return;
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_LBUTTON))
	{
		if (!m_isDragging)
		{
			// ui 좌표를 토대로 드래그 시작 인정 범위를 만든다.
			CalculateDragStartRange(directuonStartPos);

			// 드래그 시작점 기록
			m_dragStartPos = InputManager::GetInstance()->GetMousePosition();

			// 드래그 시작 좌표와 인정 범위를 토대로 드래깅 가능한지 체크한다.
			if (CheckDragStartPoint(m_dragStartPos))
				m_isDragging = true;
			else
				m_isFailedStartPoint = true;
		}
	}
	else
	{
		if (m_isDragging)
		{
			// 드래그 종료점 기록
			m_dragEndPos = InputManager::GetInstance()->GetMousePosition();

			// 드래그 유효성 체크 및 처리
			if (CheckDragValid(m_dragStartPos, m_dragEndPos))
			{
				++m_miniGameInfo->progress;
				SoundManager::GetInstance()->PlaySFX("QTE_Slice.mp3", false);
			}

			m_isDragging = false;
		}
	}
}

bool QuickTimeEventScript::CheckDragValid(const Vector2& start, const Vector2& end)
{
	float x = std::abs(start.x - end.x);
	float y = std::abs(start.y - end.y);

	float z = std::sqrt(x * x + y * y);

	// 드래그 시작 지점과 끝 지점의 길이가 400픽셀 보다 작다면 실패 처리
	if (z < 220.f)
	{
		SoundManager::GetInstance()->PlaySFX("QTE_Wrong.mp3", false);
		return false;
	}
		

	// 드래그 각도 계산
	float angle = atan2(end.y - start.y, end.x - start.x) * 180.0f / DirectX::XM_PI;

	// 드래그 각도가 유효한지 확인
	return CalculateDragAngle(angle);
}

bool QuickTimeEventScript::CalculateDragAngle(float angle)
{
	// 인정하는 각도 범위
	const float angleRecognition = 22.5f;



	// 8방향에 해당하는 각도 범위 정의
	float validDirection = 0.f;

	switch (m_direction)
	{
	case DragDirection::Down:
		validDirection = 90.f;
		break;

	case DragDirection::DownLeft:
		validDirection = 135.f;
		break;

	case DragDirection::Left:
		validDirection = 180.f;
		break;

	case DragDirection::UpLeft:
		validDirection = 135.f;
		break;

	case DragDirection::Up:
		validDirection = 90.f;
		break;

	case DragDirection::UpRight:
		validDirection = 45.f;
		break;

	case DragDirection::Right:
		validDirection = 0.f;
		break;

	case DragDirection::DownRight:
		validDirection = 45.f;
		break;
	};


	if (std::abs(angle - validDirection) <= angleRecognition)
		return true;

	SoundManager::GetInstance()->PlaySFX("QTE_Wrong.mp3", false);
	return false;
}

void QuickTimeEventScript::CalculateDragStartRange(Vector2 pos)
{
	m_leftTop = Vector2(pos.x - 125, pos.y - 150);
	m_rightBottom = Vector2(pos.x + 125, pos.y + 150);
}
