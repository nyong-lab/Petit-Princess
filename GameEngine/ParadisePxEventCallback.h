#pragma once

#include <PxPhysicsAPI.h>
using namespace physx;

// bool isTrigger(const PxFilterData& data)
// {
// 	if (data.word0 != 0xffffffff)
// 		return false;
// 	if (data.word1 != 0xffffffff)
// 		return false;
// 	if (data.word2 != 0xffffffff)
// 		return false;
// 	if (data.word3 != 0xffffffff)
// 		return false;
// 	return true;
// }

PxFilterFlags ParadiseFilerShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);


/// <summary>
/// 충돌과 관련된 이벤트 콜백을 관리하는 클래스
/// 
/// 손서희
/// </summary>
class ParadisePxEventCallback : public physx::PxSimulationEventCallback
{
	virtual void onTrigger(
		PxTriggerPair* pairs, 
		PxU32 count);

	virtual void onContact(
		const PxContactPairHeader& pairHeader, 
		const PxContactPair* pairs, 
		PxU32 nbPairs);

	void onWake(PxActor** /*actors*/, PxU32 /*count*/) {}
	void onSleep(PxActor** /*actors*/, PxU32 /*count*/) {}
	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {}
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}
};

