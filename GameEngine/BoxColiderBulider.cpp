#include "GameEngine_pch.h"
#include "BoxColiderBulider.h"
#include "BoxCollider.h"

BoxColiderBulider::BoxColiderBulider()
{

}

BoxColiderBulider::~BoxColiderBulider()
{

}

std::shared_ptr<Component> BoxColiderBulider::Create()
{
	return std::make_shared<BoxCollider>();
}
