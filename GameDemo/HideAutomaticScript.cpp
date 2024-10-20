#include "GameDemo_pch.h"
#include "HideAutomaticScript.h"

#include "SoundManager.h"

#include "BoxCollider.h"
#include "TextureBox.h"
#include "PlayerFSM.h"
#include "PlayerMoveScript.h"
#include "MeshFilter.h"

void HideAutomaticScript::Update(double dt)
{

}


void HideAutomaticScript::OnTriggerEnter(Collider* collider)
{
	if (!collider) return;

	// 플레이어가 접근 했다면 
	// 플레이어 렌더 false + 숨기 상태로 변환
	if (collider->GetOwner().lock()->GetName() == "Player")
	{
		if (collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{
			m_isPlayerHidden = collider->GetOwner().lock()->GetComponent<PlayerMoveScript>()->GetPlayerHiddenPointer();
			*m_isPlayerHidden = true;

			//collider->GetOwner().lock()->SetRender(false);
			collider->GetOwner().lock()->GetComponent<MeshFilter>()->GetPBRSet()->
				SetBaseColor(Vector4(147,109,109,255));
			collider->GetOwner().lock()->GetComponent<MeshFilter>()->GetPBRSet()->
				useBaseColorMap = false;

			collider->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Hide);

			SoundManager::GetInstance()->PlaySFX("Hide_On_Bush.mp3");
		}
	}
}

void HideAutomaticScript::OnTriggerStay(Collider* collider)
{
	
}

void HideAutomaticScript::OnTriggerExit(Collider* collider)
{
	if (!collider) return;

	// 플레이어가 빠져 나갔다면
	// 플레이어 렌더 true + 이동 상태로 변환
	if (collider->GetOwner().lock()->GetName() == "Player")
	{
		if (collider->GetOwner().lock()->GetComponent<PlayerFSM>() != nullptr)
		{
			*m_isPlayerHidden = false;
			m_isPlayerHidden = nullptr;

			collider->GetOwner().lock()->GetComponent<MeshFilter>()->GetPBRSet()->
				useBaseColorMap = true;

			collider->GetOwner().lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Run);
		}
	}
}

std::shared_ptr<Component> HideAutomaticScript::Clone() const
{
	std::shared_ptr<HideAutomaticScript> clone = std::make_shared<HideAutomaticScript>();

	return clone;
}