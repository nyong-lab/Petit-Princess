#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

#include "GameEngine_pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "CollisionFilter.h"
#include "ParadisePxEventCallback.h"

void CollisionManager::Initialize()
{
	//foundation ����
	m_foundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		m_allocator,
		m_errorCallback);

#ifdef _DEBUG
	m_pvd = PxCreatePvd(*m_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	//physics ����
	m_physics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*m_foundation,
		PxTolerancesScale(),  // �����ùķ��̼��� ���е� ���� ����. �Ϲ������� �⺻ ���� ����Ѵ�. 
		true,				// �޸� �Ҵ� ��� Ȱ��ȭ -> �޸� ���� Ž��, ����� ��..
		m_pvd);
#else 
	m_physics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*m_foundation,
		PxTolerancesScale(),
		false);
#endif

	// ������ ����
	m_dispatcher = PxDefaultCpuDispatcherCreate(2);  //��Ƽ �������� ����� cpu��

	// �浹 ���͸� 
	static ParadisePxEventCallback myCallback;

	// �� ����
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	//sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	sceneDesc.cpuDispatcher = m_dispatcher;
	sceneDesc.filterShader = ParadiseFilerShader;
	sceneDesc.simulationEventCallback = &myCallback;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS; //
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION; // ����ȭ ���
	//sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;  // CCD Ȱ��ȭ
	//sceneDesc.solverType = PxSolverType::eTGS;  // TGS �ֹ� ��� (�⺻�� ePGS)

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

	// ���͸� ���� ����
	m_collisionFilter = std::make_unique<CollisionFilter>();
}

void CollisionManager::FixedUpdate(double dt)
{
	m_scene->simulate(dt);
	m_scene->fetchResults(true);
	// ���� �ùķ��̼� ������ �Ϸ�� ������ ����ϵ��� �����մϴ�. 
	// �ٽ� ����, �ùķ��̼� ������ �Ϸ�� ������ �޼��尡 ����(blocking)�˴ϴ�.
	// �̰��� �Ϲ������� �� ������ ������ ���� �ùķ��̼��� ������ �Ŀ� ����� ���������� �� �� ���˴ϴ�.

	/// ���� ������ ������Ʈ
	PxU32 nbActiveActors;
	PxActor** activeActors = m_scene->getActiveActors(nbActiveActors);

	// �ùķ��̼� ����� ��ƼƼ�� �����Ѵ�.
	for (PxU32 i = 0; i < nbActiveActors; ++i)
	{
		//PxRigidActor* rigidActor = dynamic_cast<PxRigidActor*>(activeActors[i]);
		PxRigidActor* rigidActor = activeActors[i]->is<PxRigidActor>();
		if (rigidActor)
		{
			ColliderInfo* info = static_cast<ColliderInfo*>(rigidActor->userData);
			Collider* collider = nullptr;
			if (info) collider = info->collider;

			//active ������ ������Ʈ ������ ������. �̷��� �ϸ� �ȵ� �� ������ �ϴ� �־����...
			if (collider)
			{
				// �ùķ��̼� ��� Ʈ������ �� ����
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
	//TODO::m_scene �� null�ϸ��� ���µ� �ڲ� ������;
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
// 		PxVec3(origin.x, origin.y, origin.z),         // ������
// 		PxVec3(unitDir.x, unitDir.y, unitDir.z),      // ��������
// 		PxReal(maxDistance), hit);
// ���� �׳� �̷��� �ϸ� ���;� �ϴ°� �ƴϴ�? 
	
	RayCastInfo info; 
	PxF32 max = (PxF32)maxDistance;

	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	bool find = m_scene->raycast(
		PxVec3(origin.x, origin.y, origin.z),         // ������
		PxVec3(unitDir.x, unitDir.y, unitDir.z),      // ��������
		PxReal(maxDistance),                          // �Ÿ�
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
	
	//������ �ʵ尡 0�� �ƴϰ�, �������� ��Ʈ AND ���� ������ ���� ���� �����Ͱ� 0�̸� �������� �ǳʶݴϴ�.
	PxQueryFilterData filterData = PxQueryFilterData();  //flags(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC)
	//fd.flags |= PxQueryFlag::;

	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	bool find = m_scene->raycast(
		PxVec3(origin.x, origin.y, origin.z),         // ������
		PxVec3(unitDir.x, unitDir.y, unitDir.z),      // ��������
		PxReal(maxDistance),                          // �Ÿ�
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
