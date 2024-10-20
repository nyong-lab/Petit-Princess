#include "GameDemo_pch.h"
#include "InterKeyScript.h"

#include "FoodTableScript.h"
#include "TechniquesScript.h"

#include "Entity.h"

void InterKeyScript::Update(double dt)
{

}

void InterKeyScript::Render(double dt)
{

}

std::shared_ptr<Component> InterKeyScript::Clone() const
{
	// ���� ���Ը� Ŭ�� �������� �׿� ���õ� ��Ʈ ģ������ ���� Ŭ�н�Ű�� �ʴ´�.
	// ���� ������ ���͸� ���� ���õ� ģ������ ��� ������ֱ� ����
	std::shared_ptr<InterKeyScript> clone = std::make_shared<InterKeyScript>();

	return clone;
}

bool InterKeyScript::GetInteractive()
{
	if (m_foodTableScript)
		return m_foodTableScript->GetInteractive();
	else if (m_techniquesScript)
		return m_techniquesScript->GetInteractive();
	else 
		return false;
}

DirectX::SimpleMath::Vector3 InterKeyScript::GetObjectPosition()
{
	return GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();
}
