#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

#include "GameEngine_pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "CollisionFilter.h"
#include "ParadisePxEventCallback.h"

void CollisionManager::Initialize()
{
	//foundation 생성
	m_foundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		m_allocator,
		m_errorCallback);

#ifdef _DEBUG
	m_pvd = PxCreatePvd(*m_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	//physics 생성
	m_physics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*m_foundation,
		PxTolerancesScale(),  // 물리시뮬레이션의 정밀도 관련 설정. 일반적으로 기본 값을 사용한다. 
		true,				// 메모리 할당 기록 활성화 -> 메모리 누수 탐지, 디버깅 등..
		m_pvd);
#else 
	m_physics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*m_foundation,
		PxTolerancesScale(),
		false);
#endif

	// 디스패쳐 생성
	m_dispatcher = PxDefaultCpuDispatcherCreate(2);  //멀티 스레딩에 사용할 cpu수

	// 충돌 필터링 
	static ParadisePxEventCallback myCallback;

	// 씬 생성
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	//sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	sceneDesc.cpuDispatcher = m_dispatcher;
	sceneDesc.filterShader = ParadiseFilerShader;
	sceneDesc.simulationEventCallback = &myCallback;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS; //
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION; // 안정화 사용
	//sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;  // CCD 활성화
	//sceneDesc.solverType = PxSolverType::eTGS;  // TGS 솔버 사용 (기본값 ePGS)

	m_scene = m_physics->createScene(sceneDesc);

#ifdef _DEBUG

	PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif 

	// 필터링 헬퍼 생성
	m_collisionFilter = std::make_unique<CollisionFilter>();
}

void CollisionManager::FixedUpdate(double dt)
{
	m_scene->simulate(dt);
	m_scene->fetchResults(true);
	// 현재 시뮬레이션 스텝이 완료될 때까지 대기하도록 지시합니다. 
	// 다시 말해, 시뮬레이션 스텝이 완료될 때까지 메서드가 차단(blocking)됩니다.
	// 이것은 일반적으로 한 프레임 내에서 물리 시뮬레이션을 진행한 후에 결과를 가져오고자 할 때 사용됩니다.

	/// 유저 데이터 업데이트
	PxU32 nbActiveActors;
	PxActor** activeActors = m_scene->getActiveActors(nbActiveActors);

	// 시뮬레이션 결과를 엔티티로 전달한다.
	for (PxU32 i = 0; i < nbActiveActors; ++i)
	{
		//PxRigidActor* rigidActor = dynamic_cast<PxRigidActor*>(activeActors[i]);
		PxRigidActor* rigidActor = activeActors[i]->is<PxRigidActor>();
		if (rigidActor)
		{
			ColliderInfo* info = static_cast<ColliderInfo*>(rigidActor->userData);
			Collider* collider = nullptr;
			if (info) collider = info->collider;

			//active 상태의 컴포넌트 삭제시 터진다. 이렇게 하면 안될 것 같은데 일단 넣어놨다...
			if (collider)
			{
				// 시뮬레이션 결과 트랜스폼 값 전달
				PxVec3 pos = rigidActor->getGlobalPose().p;
				PxQuat rotq = rigidActor->getGlobalPose().q;
				Quaternion rot{ rotq.x, rotq.y, rotq.z, rotq.w };

				collider->UpdateTransform(Vector3(pos.x, pos.y, pos.z), rot);
			}
		}
	}
}

void CollisionManager::Finalize()
{
	PX_RELEASE(m_scene);
	PX_RELEASE(m_dispatcher);
	PX_RELEASE(m_physics);
	if (m_pvd)
	{
		PxPvdTransport* transport = m_pvd->getTransport();
		m_pvd->release();	m_pvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_foundation);
}

void CollisionManager::AddActorToScene(PxActor* actor)
{
	m_scene->addActor(*actor);
}

void CollisionManager::RemoveActorFormScene(PxActor* actor)
{
	if (actor != nullptr && m_scene != nullptr)
		m_scene->removeActor(*actor);
	//TODO::m_scene 이 null일리가 없는데 자꾸 터진다;
}

void CollisionManager::SetUpFiltering(PxShape* shape, LayerCollision layer)
{
	m_collisionFilter->SetupFiltering(shape, layer);
}

RayCastInfo CollisionManager::RaycastClosest(Vector3 origin, Vector3 unitDir, float maxDistance)
{
// 	RayCastInfo info;
// 	
// 	PxRaycastBuffer hit;  // [out] User provided buffer for results
// 	bool find = m_scene->raycast(
// 		PxVec3(origin.x, origin.y, origin.z),         // 시작점
// 		PxVec3(unitDir.x, unitDir.y, unitDir.z),      // 단위벡터
// 		PxReal(maxDistance), hit);
// 원래 그냥 이렇게 하면 나와야 하는거 아니니? 
	
	RayCastInfo info; 
	PxF32 max = (PxF32)maxDistance;

	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	bool find = m_scene->raycast(
		PxVec3(origin.x, origin.y, origin.z),         // 시작점
		PxVec3(unitDir.x, unitDir.y, unitDir.z),      // 단위벡터
		PxReal(maxDistance),                          // 거리
		buf);										  // PxRaycastCallback& hitCall

	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		ColliderInfo* colInfo = static_cast<ColliderInfo*>(buf.getTouch(i).actor->userData);

		PxVec3 p = buf.getTouch(i).position;
		PxVec3 n = buf.getTouch(i).normal;
		PxF32 d = buf.getTouch(i).distance;

		if (d < maxDistance)
		{
			Vector3 pos = Vector3((float)p.x, (float)p.y, (float)p.z);
			Vector3 normal = Vector3((float)n.x, (float)n.y, (float)n.z);
			float dist = (float)d;

			info = RayCastInfo(colInfo->collider, pos, normal, dist);
		}
	}
	return info;
}

std::vector<RayCastInfo> CollisionManager::RaycastMultiple(Vector3 origin, Vector3 unitDir, float maxDistance)
{
	std::vector<RayCastInfo> result;
	
	//데이터 필드가 0이 아니고, 데이터의 비트 AND 값과 도형의 쿼리 필터 데이터가 0이면 셰이프를 건너뜁니다.
	PxQueryFilterData filterData = PxQueryFilterData();  //flags(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC)
	//fd.flags |= PxQueryFlag::;

	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	bool find = m_scene->raycast(
		PxVec3(origin.x, origin.y, origin.z),         // 시작점
		PxVec3(unitDir.x, unitDir.y, unitDir.z),      // 단위벡터
		PxReal(maxDistance),                          // 거리
		buf,                                          // PxRaycastCallback& hitCall
		PxHitFlags(PxHitFlag::eDEFAULT),              // PxHitFlags hitFlags = PxHitFlag::eDEFAULT,
		filterData);                                  // const PxQueryFilterData& filterData = PxQueryFilterData()

	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		ColliderInfo* colInfo = static_cast<ColliderInfo*>(buf.getTouch(i).actor->userData);

		PxVec3 p = buf.getTouch(i).position;
		PxVec3 n = buf.getTouch(i).normal;
		PxF32 d = buf.getTouch(i).distance;

		Vector3 pos = Vector3((float)p.x, (float)p.y, (float)p.z);
		Vector3 normal = Vector3((float)n.x, (float)n.y, (float)n.z);
		float dist = (float)d;

		RayCastInfo info(colInfo->collider, pos, normal, dist);
		result.push_back(info);
	}

	return result;
}
