#include "GameDemo_pch.h"
#include "MerchantUI.h"
#include "FoodTableScript.h"
#include "Entity.h"
#include "MinigameDefine.h"
#include "Transform.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "MathUtils.h"

MerchantUI::MerchantUI()
	:m_state(0), m_foodtype(0), m_ratio(0)
{

}

MerchantUI::~MerchantUI()
{

}

void MerchantUI::Start()
{
	//상인 → 음식 테이블(자식) → 음식 테이블 스크립트
	std::weak_ptr<Entity> merchant = GetTransform()->GetParent().lock()->GetOwner();
	std::weak_ptr<Entity> store = merchant.lock()->GetComponent<Transform>()->GetParent().lock()->GetOwner();

	for (const auto& storeChild : store.lock()->GetComponent<Transform>()->GetChildren())
	{
		if (storeChild->GetOwner().lock()->HasComponent<FoodTableScript>())
		{
			std::weak_ptr<Entity> table = storeChild->GetOwner();
			m_foodTable = table.lock()->GetComponent<FoodTableScript>();
		}
	}

	if (!m_foodTable)
		assert(!"푸드 테이블 연결 실패- 동일");
	

	if (!m_foodTable) assert(false && L"푸드테이블 스크립트가 연결되지 않음");

	// 음식 종류 고기0 과일1 빵2 해산물3 디저트4
	FoodType ty = m_foodTable->m_foodType;
	if (ty == FoodType::Bagle || ty == FoodType::BaguettePoint || ty == FoodType::BaguetteWheel)
		m_foodtype = 2;
	else if (ty == FoodType::CartoonMeat || ty == FoodType::Ham || ty == FoodType::Sausage)
		m_foodtype = 0;
	else if (ty == FoodType::Apple || ty == FoodType::WaterMelon || ty == FoodType::Banana)
		m_foodtype = 1;
	else if (ty == FoodType::Crab || ty == FoodType::Fish || ty == FoodType::Squid)
		m_foodtype = 3;
	else if (ty == FoodType::CupCake || ty == FoodType::Cake)
		m_foodtype = 4;
}

void MerchantUI::Update(double dt)
{
	/// ui 띄울 좌표 업데이트
	Vector3 merchPos = GetTransform()->GetWorldPosition();
	Matrix viewProj = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetViewProjectionMatrix();
	Vector2 screenSize = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetScreenSize();
	m_pos = MathUtils::WorldToScreenPixel(merchPos, viewProj, screenSize.x, screenSize.y);
	m_pos += Vector2(0, -150);
	
	/// 우선순위 1번 -> 눈치게이지
	// 눈치 게이지
	float sence = *(m_foodTable->m_merchantSense);
	if (sence > 0 && sence < 100)
	{
		m_state = 4;
		m_ratio = (1.f - sence * 0.01f);

		//return;
	}

	// 적발 상태
	else if (sence >= 100)
	{
		m_state = 5;
	}

	/// 맛집 여부, 맛집일 때
	else if (m_foodTable->m_isDeliciousFood)
	{
		// 먹었는가?
		if (m_foodTable->m_isPlayerAteFood)
		{
			m_state = 2;
			//return;
		}
		else // 준비중인가 -> 맛집 종류 판정
		{
			m_state = 1;
			//return;
		}
	}
	else /// 맛집이 아닐 때
	{
		// 재방문
		if (m_foodTable->m_reVisitElapsedTime > 0)
		{
			m_state = 3;
			m_ratio = m_foodTable->m_reVisitElapsedTime *0.2f;
		}
		// 아무것도 안뜨기
		else
		{
			m_state = 0;
		}
	}

	if (m_state == 0) m_owner.lock()->SetRender(false);
	else m_owner.lock()->SetRender(true);
}

void MerchantUI::Render(double dt)
{

}

std::shared_ptr<Component> MerchantUI::Clone() const
{
	auto clone = std::make_shared<MerchantUI>();
	return clone;
}
