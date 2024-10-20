#pragma once

/// <summary>
/// ��ƼƼ�� �������ϴ� ������ ���� ������ �����ϱ� ���� enum class
/// 
/// ���¿�
/// </summary>
enum class Layers
{
	None,
	Background,
	Player,
	Monster,
	Trap,
	Object,
	UI,
	End = 32
};

/// <summary>
/// CollisionManager���� �浹�� ó���� �� ����ϴ� Layer
/// 
/// https://docs.unity3d.com/Manual/class-TagManager.html
/// https://docs.unity3d.com/Manual/Tags.html
/// 240112 �̼��� �ռ���
/// ***** ���� ����ȴٸ� CollisionFilter.cpp ���ϵ� �����ؾ� �����۵� �մϴ�. *****
/// </summary>
enum class LayerCollision			
{
	Default			= (1 << 0),  // 1
	UI				= (1 << 1),  // 2
	Ground			= (1 << 2),  // 4
	Player			= (1 << 3),  // 8
	Interactable	= (1 << 4),  // 16
	Wall			= (1 << 5),  // 32
	CallingRange	= (1 << 6),  // 64
	Store			= (1 << 7),  // 128
	Merchant		= (1 << 8),  // 256
	Guard			= (1 << 9),  // 512
	OnlyPlayer		= (1 << 10)  // 1024
};

/// �浹 �߻��� �浹���� ���� ������ �����ϴ� ����ü
struct ContactInfo
{
	Vector3 contactPoint;
	Vector3 impulse;
};

/// �ݶ��̴��� ������ �����ϴ� ����ü.
/// �ݶ��̴��� ���� �����Ϳ� �ݶ��̴� ���� Ʈ���� �ݹ����� ���ȴ�.
struct ColliderInfo
{
	Collider* collider;
	unsigned int id;

	ColliderInfo(Collider* collider, unsigned int id)
		:collider(collider), id(id)
	{ }
};

/// <summary>
/// Collision ������ Collider.OnCollisionEnter, Collider.OnCollisionStay �� Collider.OnCollisionExit �̺�Ʈ�� ���޵˴ϴ�.
/// 
/// https://docs.unity3d.com/ScriptReference/Collision.html
/// https://docs.unity3d.com/ScriptReference/ContactPoint.html
/// 240112 �̼��� �ռ���
/// </summary>
struct Collision
{
	Collider* thisCollider;
 	Collider* otherCollider;
	ContactInfo contact;

	Collision(Collider* thisCol, Collider* otherCol)
		:thisCollider(thisCol), otherCollider(otherCol), contact()
	{ }

	Collision()
		:thisCollider(nullptr), otherCollider(nullptr), contact()
	{ }
};

/// �ݶ��̴� ���� �浹 �ݹ��� ���� ���޷� ���Ǵ� ����ü.
struct CollisionInfo
{
	Collision collision;
	unsigned int thisId;
	unsigned int otherId = 0;

	CollisionInfo(ColliderInfo* thisColInfo, ColliderInfo* otherColInfo)
	{
		collision.thisCollider = thisColInfo->collider;
		thisId = thisColInfo->id;

		if (otherColInfo != nullptr)
		{
			collision.otherCollider = otherColInfo->collider;
			otherId = otherColInfo->id;
		}
	}
};

struct RayCastInfo
{
	Collider* collider = nullptr;
	Vector3 position;
	Vector3 normal;
	float distance;

	RayCastInfo(Collider* collider, Vector3 pos, Vector3 norm, float dist)
		:collider(collider), position(pos), normal(norm), distance(dist)
	{ }

	RayCastInfo()
		:position(Vector3()), normal(Vector3()), distance(0)
	{ }
};