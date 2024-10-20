#include "GameEngine_pch.h"
#include "StatBuilder.h"
#include "Stat.h"

StatBuilder::StatBuilder()
{

}

StatBuilder::~StatBuilder()
{

}

std::shared_ptr<Component> StatBuilder::Create()
{
	return std::make_shared<Stat>();
}
