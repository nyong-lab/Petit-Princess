#pragma once
#include <Component.h>
class UserScript :
    public Component
{
public:
	virtual void Update(double dt) override;
	virtual void Render() override;
	virtual void Finalize() override;

	// 복사본을 만들어내서 반환.
	virtual std::shared_ptr<Component> Clone() const override;
};

