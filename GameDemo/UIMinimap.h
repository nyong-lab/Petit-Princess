#pragma once

#include <Entity.h>


class UIMinimap :  public Entity
{
public:
	UIMinimap();
	UIMinimap(const std::shared_ptr<UIMinimap>& other);
	~UIMinimap();
public:
	virtual void Start() override;
};

