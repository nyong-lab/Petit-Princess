#include "GameEngine_pch.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Transform.h"


RigidBody::RigidBody()
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::Update(double dt)
{

}

void RigidBody::Render(double dt)
{

}

void RigidBody::Finalize()
{

}

std::shared_ptr<Component> RigidBody::Clone() const
{
	return nullptr; // 임시 반환
}

void RigidBody::AddForce(const Vector3& force)
{

}

void RigidBody::MovePosition(const Vector3& position)
{
	std::shared_ptr<Entity> owner = GetOwner().lock();
	owner->GetComponent<Transform>()->AddLocalPosition(position);
}

/// <summary>
/// Degrees 단위로 회전합니다.
/// </summary>
/// <param name="rotation"></param>
void RigidBody::MoveRotation(const Vector3& rotation)
{
	std::shared_ptr<Entity> owner = GetOwner().lock();
	//Rotator rotator = owner->GetComponent<Transform>()->GetRotation().ToRotator();

	//owner->GetComponent<Transform>()->AddRotation(rotator);
}
