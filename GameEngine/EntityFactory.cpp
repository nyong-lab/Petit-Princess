#include "GameEngine_pch.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "IBuilder.h"

EntityFactory::EntityFactory()
{

}

EntityFactory::~EntityFactory()
{

}

std::shared_ptr<Entity> EntityFactory::Create(std::string name, std::shared_ptr<IBuilder<Component>> builder)
{
	std::shared_ptr<Entity> returnEntity = std::make_shared<Entity>(name);
	returnEntity->AddComponent(builder);
	return returnEntity;
}

std::shared_ptr<Entity> EntityFactory::Create(int count, std::string name, std::shared_ptr<IBuilder<Component>> builder...)
{
	m_returnEntity = std::make_shared<Entity>(name);

	va_list arg;
	va_start(arg, builder);
	m_returnEntity->AddComponent(builder);

	for (int i = 0; i < count - 1; i++)
	{
		m_returnEntity->AddComponent((va_arg(arg, std::shared_ptr<IBuilder<Component>>)));
	}

	va_end(arg);

	return m_returnEntity;
}


std::shared_ptr<Entity> EntityFactory::Create(int count, std::string name, IBuilder<Component>&& builder...)
{
	m_returnEntity = std::make_shared<Entity>(name);

	va_list arg;
	va_start(arg, &builder);

	std::shared_ptr<Component> temp = builder.Create();

	m_returnEntity->AddComponent(temp);

	for (int i = 0; i < count - 1; i++)
	{
		m_returnEntity->AddComponent((va_arg(arg, std::shared_ptr<IBuilder<Component>>)));
	}

	va_end(arg);

	return m_returnEntity;
}

void EntityFactory::AddComponents(int count, std::shared_ptr <Entity> entity, std::shared_ptr<IBuilder<Component>> builder...)
{
	va_list arg;
	va_start(arg, builder);
	entity->AddComponent(builder);

	for (int i = 0; i < count - 1; i++)
	{
		entity->AddComponent((va_arg(arg, std::shared_ptr<IBuilder<Component>>)));
	}

	va_end(arg);
}
