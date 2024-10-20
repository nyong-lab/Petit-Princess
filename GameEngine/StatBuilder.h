#pragma once
#include "IBuilder.h"
#include "Component.h"

/// <summary>
/// 2024.01.08
/// Battle을 만들어낼 Builder
/// 작성 : 유승운
/// </summary>

class StatBuilder : public IBuilder<Component>
{
public:
	StatBuilder();
	~StatBuilder();

	virtual std::shared_ptr<Component> Create() override;

};