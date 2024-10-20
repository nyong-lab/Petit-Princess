#include "GameDemo_pch.h"
#include "autoBush.h"
#include "SphereCollider.h"
#include "MeshRenderer.h"
#include "HideAutomaticScript.h"

autoBush::autoBush() : Entity("AutoBush")
{

}

autoBush::autoBush(const std::shared_ptr<autoBush>& other) : Entity(other)
{

}

autoBush::~autoBush()
{

}

void autoBush::Start()
{
	
	SetName("HideAutomaticObject");
	AddComponent<MeshRenderer>("bush");

	AddComponent<MeshFilter>("bushMesh");
	GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::TextureBox);

	AddComponent<SphereCollider>();
	GetComponent<SphereCollider>()->SetRadius(1.5f);
	GetComponent<SphereCollider>()->SetDisableGravity(true);
	GetComponent<SphereCollider>()->SetIsTrigger(true);
	AddComponent<HideAutomaticScript>();

	Entity::Start();
}

void autoBush::SetPos(Vector3 pos)
{
	GetComponent<Transform>()->SetLocalPosition(Vector3(pos));
}
