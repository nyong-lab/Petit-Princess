#include "GameDemo_pch.h"
#include "InteractionBush.h"

#include "SphereCollider.h"
#include "MeshRenderer.h"
#include "HideInteractionScript.h"
#include "UITestScript.h"
#include "SceneManager.h"
#include "Scene.h"

InteractionBush::InteractionBush() : Entity("InteractionBush")
{

}

InteractionBush::InteractionBush(const std::shared_ptr<InteractionBush>& other) : Entity(other)
{

}

InteractionBush::~InteractionBush()
{

}

void InteractionBush::Start()
{
	// 03. 상호작용 숨기 오브젝트 - 다가가면 UI 띄워지고, 버튼 눌러야 숨어짐, 쿨타임 3초 있음 
	SetName("HideInteractionObject");

	AddComponent<MeshRenderer>("bush");
	AddComponent<MeshFilter>("bushMesh");
	GetComponent<MeshFilter>()->SetModelKey(GraphicsTransfer::ModelKey::TextureBox);

	AddComponent<SphereCollider>();
	GetComponent<SphereCollider>()->SetRadius(1.5f);
	GetComponent<SphereCollider>()->SetDisableGravity(true);
	GetComponent<SphereCollider>()->SetIsTrigger(true);
	AddComponent<HideInteractionScript>();

	//숨기 가능 표시용 UI
	std::shared_ptr<UI> m_hideInteractionUI= std::make_shared<UI>();
	m_hideInteractionUI->SetName("PressToHide");
	m_hideInteractionUI->GetComponent<Transform>()->SetWorldPosition({ 500,000,0 });
	m_hideInteractionUI->GetComponent<Transform>()->SetWorldScale({ 1,1,1 });
	m_hideInteractionUI->AddComponent<UITestScript>(UIType::STATIC);
	m_hideInteractionUI->SetRender(false);

	//// 부쉬에 ui 붙임
	GetComponent<Transform>()->SetChild(m_hideInteractionUI->GetComponent<Transform>());
	m_hideInteractionUI->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 0));
	m_sceneManager.lock()->GetCurScene()->AddEntity(m_hideInteractionUI);
	m_hideInteractionUI->Start();


	Entity::Start();
}

void InteractionBush::SetPos(Vector3 pos)
{
	GetComponent<Transform>()->SetLocalPosition(pos);
}
