#include "GameEngine_pch.h"
#include "Component.h"

#include "Entity.h"

UINT Component::m_lastComponentID = 0;

Component::Component()
{
	m_UID = m_lastComponentID++;
}

Component::Component(const std::string& name)
	: m_name(name)
{
	m_UID = m_lastComponentID++;
}

Component::~Component()
{

}

void Component::Start()
{

}

void Component::Update(double dt)
{

}

void Component::FixedUpdate(double FixedTime)
{

}

void Component::Render(double dt)
{

}

void Component::Finalize()
{

}

void Component::CloneBaseComponentMember(std::shared_ptr<Component> baseComponent)
{
	m_name = baseComponent->m_name;
	m_UID = m_lastComponentID++;
}

std::shared_ptr<Transform> Component::GetTransform()
{
	std::shared_ptr<Entity> owner = m_owner.lock();
	if (owner)
	{
		return owner->GetComponent<Transform>();
	}

	return nullptr;
}
