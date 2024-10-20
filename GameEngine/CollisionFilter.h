#pragma once

#include <PxPhysicsAPI.h>
using namespace physx;

#include "EntityDefine.h"
#include <unordered_map>


class CollisionFilter
{
public:
	CollisionFilter();
	void SetupFiltering(PxShape* shape, LayerCollision layer);

private:
	void InitializeCollisionFilter();
	void InitializeQueryFilter();

private:
	std::unordered_map<LayerCollision, PxU32> m_simulFilters;
	std::unordered_map<LayerCollision, PxU32> m_queryFilters;
};
