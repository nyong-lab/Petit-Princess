#pragma once
#include "IBuilder.h"
#include "Component.h"

/// <summary>
/// 2024.01.08
/// Colider를 만들어낼 Builder
/// 작성 : 유승운
/// </summary>

class BoxColiderBulider : public IBuilder<Component>
{
public:
	BoxColiderBulider();
	~BoxColiderBulider();

	virtual std::shared_ptr<Component> Create() override;
};