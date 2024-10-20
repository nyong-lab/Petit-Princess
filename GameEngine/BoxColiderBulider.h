#pragma once
#include "IBuilder.h"
#include "Component.h"

/// <summary>
/// 2024.01.08
/// Colider�� ���� Builder
/// �ۼ� : ���¿�
/// </summary>

class BoxColiderBulider : public IBuilder<Component>
{
public:
	BoxColiderBulider();
	~BoxColiderBulider();

	virtual std::shared_ptr<Component> Create() override;
};