#include "GameEngine_pch.h"
#include "PlaneCollider.h"
#include "CollisionManager.h"
#include "Scene.h"


PlaneCollider::PlaneCollider()
{
	m_type = BodyType::Static;
	m_shapeType = ShapeType::Plane;

	m_rigidStatic = PxCreatePlane(*m_physics, PxPlane(0, 1, 0, 0), *m_mateiral);
	m_rigidStatic->userData = new ColliderInfo(this, m_colliderID);
}

PlaneCollider::~PlaneCollider()
{
	
}

void PlaneCollider::Start()
{
	if (m_isInitialize) return;
	
	if(m_scene->IsSimulating()) m_collisionManager->AddActorToScene(m_rigidStatic);
	m_isInitialize = true;
}

void PlaneCollider::Update(double dt)
{

}

void PlaneCollider::Render(double dt)
{

}

void PlaneCollider::DebugRender()
{

}

void PlaneCollider::Finalize()
{
	Collider::Finalize();
}

std::shared_ptr<Component> PlaneCollider::Clone() const
{
	std::shared_ptr<PlaneCollider> newPlane = std::make_shared<PlaneCollider>();
	Collider::CopyMembersThisToOther(newPlane);
	newPlane->m_collisionManager = m_collisionManager;

	Matrix ident = Matrix::Identity;
	newPlane->InitialializeClone(ident, LayerCollision::Default);
	return newPlane;
}

void PlaneCollider::InitialializeClone(Matrix worldTransform, LayerCollision layer)
{
	m_colliderID = m_collisionManager->GetInstance()->GetColliderID();
	m_physics = m_collisionManager->GetPhysics();
	m_mateiral = m_physics->createMaterial(m_dynamicFriction, m_staticFriction, m_restitution);

	m_rigidStatic = PxCreatePlane(*m_physics, PxPlane(m_normal.x, m_normal.y, m_normal.z, m_distance), *m_mateiral);
	m_rigidStatic->userData = new ColliderInfo(this, m_colliderID);

	if(m_scene->IsSimulating())m_collisionManager->AddActorToScene(m_rigidStatic);
	m_isInitialize = true;
}

void PlaneCollider::UpdatePhysXProperties()
{
	// 머테리얼
	m_mateiral->setStaticFriction(m_staticFriction);
	m_mateiral->setDynamicFriction(m_dynamicFriction);
	m_mateiral->setRestitution(m_restitution);

	// 사이즈 -> pxPlane은 불변하기 때문에 새로 생성해야 한다.
	if (m_scene->IsPrevSimulating()) m_collisionManager->RemoveActorFormScene(m_rigidStatic);
	
	ColliderInfo* userdt = static_cast<ColliderInfo*>(m_rigidStatic->userData);
	m_rigidStatic->release();
	m_rigidStatic = PxCreatePlane(*m_physics, PxPlane(m_normal.x, m_normal.y, m_normal.z, m_distance), *m_mateiral);
	m_rigidStatic->userData = userdt;
	if (m_scene->IsSimulating()) m_collisionManager->AddActorToScene(m_rigidStatic);
}

void PlaneCollider::SetNormalAndDistance(Vector3 normal, float distance)
{
	m_normal = normal;
	m_distance = distance;

	// 사이즈 -> pxPlane은 불변하기 때문에 새로 생성해야 한다.
	if (m_scene->IsPrevSimulating()) m_collisionManager->RemoveActorFormScene(m_rigidStatic);
	ColliderInfo* userdt = static_cast<ColliderInfo*>(m_rigidStatic->userData);
	m_rigidStatic->release();
	m_rigidStatic = PxCreatePlane(*m_physics, PxPlane(m_normal.x, m_normal.y, m_normal.z, m_distance), *m_mateiral);
	m_rigidStatic->userData = userdt;
	if (m_scene->IsSimulating()) m_collisionManager->AddActorToScene(m_rigidStatic);
}
