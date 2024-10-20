#pragma once

#include <Entity.h>

class UIMinimapCurPos :
    public Entity
{
public:
	UIMinimapCurPos();
	UIMinimapCurPos(const std::shared_ptr<UIMinimapCurPos>& other);
	~UIMinimapCurPos();
public:
	virtual void Start() override;
};
