#include "GameEngine_pch.h"
#include "Stat.h"

Stat::Stat()
{

}

Stat::~Stat()
{

}

void Stat::Update(double dt)
{

}

void Stat::Render(double dt)
{

}

void Stat::Finalize()
{

}

std::shared_ptr<Component> Stat::Clone() const
{
	std::shared_ptr<Stat> clone = std::make_shared<Stat>();

	return clone;
}
