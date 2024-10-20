#include "GameEngine_pch.h"
#include "ReflectionTest.h"



int ReflectionTest::m_staticInt = 0;

ReflectionTest::ReflectionTest()
{

}

ReflectionTest::~ReflectionTest()
{

}

void ReflectionTest::Update()
{
}

void ReflectionTest::Finalize()
{
}

std::shared_ptr<Component> ReflectionTest::Clone() const
{
    return nullptr;
}

