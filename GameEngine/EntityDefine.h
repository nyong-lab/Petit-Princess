#pragma once

/// <summary>
/// 엔티티를 업데이하는 순서나 렌더 순서를 구분하기 위한 enum class
/// 
/// 유승운
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
/// CollisionManager에서 충돌을 처리할 때 사용하는 Layer
/// 
/// https://docs.unity3d.com/Manual/class-TagManager.html
/// https://docs.unity3d.com/Manual/Tags.html
/// 240112 이서영 손서희
/// ***** 무언가 변경된다면 CollisionFilter.cpp 파일도 수정해야 정상작동 합니다. *****
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

/// 충돌 발생시 충돌점과 강도 정보를 저장하는 구조체
struct ContactInfo
{
	Vector3 contactPoint;
	Vector3 impulse;
};

/// 콜라이더의 정보를 저장하는 구조체.
/// 콜라이더의 유저 데이터와 콜라이더 내부 트리거 콜백으로 사용된다.
struct ColliderInfo
{
	Collider* collider;
	unsigned int id;

	ColliderInfo(Collider* collider, unsigned int id)
		:collider(collider), id(id)
	{ }
};

/// <summary>
/// Collision 정보는 Collider.OnCollisionEnter, Collider.OnCollisionStay 및 Collider.OnCollisionExit 이벤트에 전달됩니다.
/// 
/// https://docs.unity3d.com/ScriptReference/Collision.html
/// https://docs.unity3d.com/ScriptReference/ContactPoint.html
/// 240112 이서영 손서희
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

/// 콜라이더 내부 충돌 콜백의 정보 전달로 사용되는 구조체.
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