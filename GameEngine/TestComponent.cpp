#include "GameEngine_pch.h"
#include "TestComponent.h"

#include "Entity.h"


int TestComponent::m_staticInt = 0;

TestComponent::TestComponent()
{

}

TestComponent::~TestComponent()
{

}

void TestComponent::Start()
{
	/*m_vectorEntity.push_back(std::make_shared<Entity>("VectorType EntityClass Test 1"));
	m_vectorEntity.push_back(std::make_shared<Entity>("VectorType EntityClass Test 2"));
	m_vectorEntity.push_back(std::make_shared<Entity>("VectorType EntityClass Test 3"));
	m_unorderedMapEntity.insert({ "key1", std::make_shared<Entity>("VectorType EntityClass Test 1") });
	m_unorderedMapEntity.insert({ "key2", std::make_shared<Entity>("VectorType EntityClass Test 2") });
	m_unorderedMapEntity.insert({ "key3", std::make_shared<Entity>("VectorType EntityClass Test 3") });*/
}

void TestComponent::Update(double dt)
{
}

void TestComponent::Render(double dt)
{

}

void TestComponent::Finalize()
{

}

std::shared_ptr<Component> TestComponent::Clone() const
{
	std::shared_ptr<TestComponent> clone = std::make_shared<TestComponent>();

	//clone->m_staticInt = m_staticInt;
	clone->m_int = m_int;
	clone->m_unsigedInt = m_unsigedInt;
	clone->m_double = m_double;
	clone->m_float = m_float;
	clone->m_bool = m_bool;

	clone->m_vector3 = m_vector3;

	clone->m_string = m_string;
	clone->m_wstring = m_wstring;

	clone->m_vectorEntity = m_vectorEntity;
	clone->m_unorderedMapEntity = m_unorderedMapEntity;

	return clone;
}

