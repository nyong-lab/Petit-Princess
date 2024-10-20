#pragma once
#include "ISingleton.h"

#include <PxPhysicsAPI.h>
#include "ParadisePxErrorCallback.h"
#include "CollisionFilter.h"
using namespace physx;

// 내부적으로 reference counting이 되기때문에 shared_ptr은 사용하지 않았다.

/// <summary>
/// PhysX를 사용한 충돌을 관리하는 매니저 클래스
/// 
/// 손서희
/// </summary>
class CollisionManager : public ISingleton<CollisionManager>
{
	friend class ISingleton<CollisionManager>;

public:
/// 맴버 함수 ---------------------------------------------------------------------
public:
	void Initialize();
	void FixedUpdate(double dt);
	void Finalize();

	// ColliderID를 발급하는 함수
	static unsigned int GetColliderID() { static unsigned int ID = 10; return ID++; }
	
	// 액터 관리
	void AddActorToScene(PxActor* actor);
	void RemoveActorFormScene(PxActor* actor);
	void SetUpFiltering(PxShape* shape, LayerCollision layer);

	// 래이케스트
	RayCastInfo RaycastClosest(Vector3 origin, Vector3 unitDir, float maxDistance);
	std::vector<RayCastInfo> RaycastMultiple(Vector3 origin, Vector3 unitDir, float maxDistance);

	// getter, setter
	PxPhysics* GetPhysics() { return m_physics; }

/// 맴버 변수 --------------------------------------------------------------------
private:
	CollisionManager() {};

	ParadisePxErrorCallback	m_errorCallback;
	PxDefaultAllocator		m_allocator;
	PxFoundation*			m_foundation;
	PxPhysics*				m_physics;     // 물리시뮬레이션 코어
	PxDefaultCpuDispatcher* m_dispatcher;  // 멀티스레딩 연산
	PxScene* m_scene;

	PxPvd*					m_pvd; //그래픽 디버거

	std::unique_ptr<CollisionFilter> m_collisionFilter;
};

