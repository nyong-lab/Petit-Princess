#pragma once
#include <Component.h>
class UserScript :
    public Component
{
public:
	virtual void Update(double dt) override;
	virtual void Render() override;
	virtual void Finalize() override;

	// ���纻�� ������ ��ȯ.
	virtual std::shared_ptr<Component> Clone() const override;
};

