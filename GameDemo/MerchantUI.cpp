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
	//���� �� ���� ���̺�(�ڽ�) �� ���� ���̺� ��ũ��Ʈ
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
		assert(!"Ǫ�� ���̺� ���� ����- ����");
	

	if (!m_foodTable) assert(false && L"Ǫ�����̺� ��ũ��Ʈ�� ������� ����");

	// ���� ���� ���0 ����1 ��2 �ػ깰3 ����Ʈ4
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
	/// ui ��� ��ǥ ������Ʈ
	Vector3 merchPos = GetTransform()->GetWorldPosition();
	Matrix viewProj = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetViewProjectionMatrix();
	Vector2 screenSize = m_owner.lock()->GetCameraManager().lock()->GetMainCamera()->GetScreenSize();
	m_pos = MathUtils::WorldToScreenPixel(merchPos, viewProj, screenSize.x, screenSize.y);
	m_pos += Vector2(0, -150);
	
	/// �켱���� 1�� -> ��ġ������
	// ��ġ ������
	float sence = *(m_foodTable->m_merchantSense);
	if (sence > 0 && sence < 100)
	{
		m_state = 4;
		m_ratio = (1.f - sence * 0.01f);

		//return;
	}

	// ���� ����
	else if (sence >= 100)
	{
		m_state = 5;
	}

	/// ���� ����, ������ ��
	else if (m_foodTable->m_isDeliciousFood)
	{
		// �Ծ��°�?
		if (m_foodTable->m_isPlayerAteFood)
		{
			m_state = 2;
			//return;
		}
		else // �غ����ΰ� -> ���� ���� ����
		{
			m_state = 1;
			//return;
		}
	}
	else /// ������ �ƴ� ��
	{
		// ��湮
		if (m_foodTable->m_reVisitElapsedTime > 0)
		{
			m_state = 3;
			m_ratio = m_foodTable->m_reVisitElapsedTime *0.2f;
		}
		// �ƹ��͵� �ȶ߱�
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
