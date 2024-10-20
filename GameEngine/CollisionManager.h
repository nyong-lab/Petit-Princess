#pragma once
#include "ISingleton.h"

#include <PxPhysicsAPI.h>
#include "ParadisePxErrorCallback.h"
#include "CollisionFilter.h"
using namespace physx;

// ���������� reference counting�� �Ǳ⶧���� shared_ptr�� ������� �ʾҴ�.

/// <summary>
/// PhysX�� ����� �浹�� �����ϴ� �Ŵ��� Ŭ����
/// 
/// �ռ���
/// </summary>
class CollisionManager : public ISingleton<CollisionManager>
{
	friend class ISingleton<CollisionManager>;

public:
/// �ɹ� �Լ� ---------------------------------------------------------------------
public:
	void Initialize();
	void FixedUpdate(double dt);
	void Finalize();

	// ColliderID�� �߱��ϴ� �Լ�
	static unsigned int GetColliderID() { static unsigned int ID = 10; return ID++; }
	
	// ���� ����
	void AddActorToScene(PxActor* actor);
	void RemoveActorFormScene(PxActor* actor);
	void SetUpFiltering(PxShape* shape, LayerCollision layer);

	// �����ɽ�Ʈ
	RayCastInfo RaycastClosest(Vector3 origin, Vector3 unitDir, float maxDistance);
	std::vector<RayCastInfo> RaycastMultiple(Vector3 origin, Vector3 unitDir, float maxDistance);

	// getter, setter
	PxPhysics* GetPhysics() { return m_physics; }

/// �ɹ� ���� --------------------------------------------------------------------
private:
	CollisionManager() {};

	ParadisePxErrorCallback	m_errorCallback;
	PxDefaultAllocator		m_allocator;
	PxFoundation*			m_foundation;
	PxPhysics*				m_physics;     // �����ùķ��̼� �ھ�
	PxDefaultCpuDispatcher* m_dispatcher;  // ��Ƽ������ ����
	PxScene* m_scene;

	PxPvd*					m_pvd; //�׷��� �����

	std::unique_ptr<CollisionFilter> m_collisionFilter;
};

