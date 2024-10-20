#include "GameDemo_pch.h"
#include "HideInterKeyScript.h"
#include "Entity.h"

#include "HideInteractionScript.h"

void HideInterKeyScript::Update(double dt)
{
}

void HideInterKeyScript::Render(double dt)
{
}

std::shared_ptr<Component> HideInterKeyScript::Clone() const
{
	// ���� ���Ը� Ŭ�� �������� �׿� ���õ� ��Ʈ ģ������ ���� Ŭ�н�Ű�� �ʴ´�.
// ���� ������ ���͸� ���� ���õ� ģ������ ��� ������ֱ� ���� <����: ���ش� �Ȱ����� �����ؿԴ�
	std::shared_ptr<HideInterKeyScript> clone = std::make_shared<HideInterKeyScript>();

	return clone;
}

bool HideInterKeyScript::GetInteractive()
{
	if (m_hideInteractionScript)
		return m_hideInteractionScript->GetInteractive();
	//else if (m_techniquesScript)
	//	return m_techniquesScript->GetInteractive();
	else
		return false;
}

Vector3 HideInterKeyScript::GetObjectPosition()
{
	return GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();
}
