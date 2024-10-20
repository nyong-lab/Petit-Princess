#pragma once
#include "Entity.h"
#include "UI.h"

class InteractionBush : public Entity
{
public:

	InteractionBush();
	InteractionBush(const std::shared_ptr<InteractionBush>& other);
	~InteractionBush();

	void Start();
	void SetPos(Vector3 pos);

private:

};

