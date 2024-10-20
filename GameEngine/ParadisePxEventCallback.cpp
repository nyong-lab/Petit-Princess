#include "GameEngine_pch.h"
#include "ParadisePxEventCallback.h"
#include "EntityDefine.h"
#include "Collider.h"

// AllocConsole();
// HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// DWORD dwByte(0);
// std::string str = "Collision Enter, thiscid : " + std::to_string(m_colliderID);
// str += '\n';
// WriteFile(hConsole, str.c_str(), str.length(), &dwByte, nullptr);


// 하나라도 트리거 상태일 때
void ParadisePxEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	while (count--)
	{
		const PxTriggerPair& current = *pairs++;

		const PxActor* actor1 = current.triggerActor;
		const PxActor* actor2 = current.otherActor;

		ColliderInfo* colliderInfo1 = static_cast<ColliderInfo*>(actor1->userData);
		ColliderInfo* colliderInfo2 = static_cast<ColliderInfo*>(actor2->userData);

		Collider* collider1 = nullptr;
		Collider* collider2 = nullptr;

		if(colliderInfo1) collider1 = colliderInfo1->collider;
		if(colliderInfo2) collider2 = colliderInfo2->collider;

		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (collider1 && collider2)
			{
				//1에게 보낼 데이터
				collider1->OnTriggerEnter(colliderInfo2);

				//2에게 보낼 데이터
				collider2->OnTriggerEnter(colliderInfo1);
			}
		}

		if (current.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//1에게 보낼 데이터
			if (collider1 != nullptr) collider1->OnTriggerExit(colliderInfo2);

			//2에게 보낼 데이터
			if (collider2 != nullptr) collider2->OnTriggerExit(colliderInfo1);
		}
	}
}


// 둘 다 트리거가 아닐 때
void ParadisePxEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	while (nbPairs--)
	{
		const PxContactPair& current = *pairs++;

		const PxRigidActor* actor1 = current.shapes[0]->getActor();
		const PxRigidActor* actor2 = current.shapes[1]->getActor();

		ColliderInfo* colliderInfo1 = static_cast<ColliderInfo*>(actor1->userData);
		ColliderInfo* colliderInfo2 = static_cast<ColliderInfo*>(actor2->userData);

		Collider* collider1 = nullptr;
		Collider* collider2 = nullptr;

		if (colliderInfo1) collider1 = colliderInfo1->collider;
		if (colliderInfo2) collider2 = colliderInfo2->collider;

		if (current.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			if (collider1 && collider2)
			{
				//1에게 보낼 데이터
				CollisionInfo collision1{ colliderInfo1, colliderInfo2 };
				collider1->OnCollisionEnter(collision1);

				//2에게 보낼 데이터
				CollisionInfo collision2{ colliderInfo2, colliderInfo1 };
				collider2->OnCollisionEnter(collision2);
			}
		}

		if (current.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//1에게 보낼 데이터
			if (collider1 != nullptr)
			{
				CollisionInfo collision1{ colliderInfo1, colliderInfo2 };
				collider1->OnCollisionExit(collision1);
			}

			//2에게 보낼 데이터
			if (collider2 != nullptr)
			{
				CollisionInfo collision2{ colliderInfo2, colliderInfo1 };
				collider2->OnCollisionExit(collision2);
			}
		}
	}
}


physx::PxFilterFlags ParadiseFilerShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	//return PxFilterFlag::eSUPPRESS;

	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;

	if ( (filterData0.word0 == 128 && filterData1.word0 == 8)
		|| (filterData1.word0 == 128 && filterData0.word0 == 8))
		int a = 0;


	// 충돌을 무시할 아이들
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		return PxFilterFlag::eSUPPRESS;
	}
	else
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	}

	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlag::eDEFAULT;
}
