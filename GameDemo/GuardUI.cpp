#include "GameDemo_pch.h"
#include "GuardUI.h"
#include "Transform.h"
#include "Entity.h"

GuardUI::GuardUI()
{

}

GuardUI::~GuardUI()
{

}

void GuardUI::Start()
{
	std::weak_ptr<Entity> guard = GetTransform()->GetParent().lock()->GetOwner();
	m_guard = guard.lock()->GetComponent<GuardFSM>();
}

void GuardUI::Update(double dt)
{
	m_state = static_cast<int>(m_guard->GetCurState());
	
	if (m_state == 2 || m_state == 3 || m_state == 4 || m_state == 5)
		m_owner.lock()->SetRender(true);
	else
		m_owner.lock()->SetRender(false);

	m_pos = GetOwner().lock()->GetComponent<Transform>()->GetWorldPosition();
}

void GuardUI::Render(double dt)
{

}

std::shared_ptr<Component> GuardUI::Clone() const
{
	auto clone = std::shared_ptr<GuardUI>();
	return clone;
}