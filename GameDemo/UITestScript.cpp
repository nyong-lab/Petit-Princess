#include "GameDemo_pch.h"
#include "UITestScript.h"

#include "InputManager.h"
#include "Transform.h"

#include "Entity.h"
#include "PlayerFSM.h"
#include "PlayerMoveScript.h"
#include "ShopKeeperFSM.h"
#include "TextRender.h"

#include "SoundManager.h"
#include "../GameEngine/EventManager.h"
#include "SceneManager.h"

UITestScript::UITestScript()
	:m_duration(0.f), m_isEndDuration(false)
{

}

UITestScript::UITestScript(UIType type)
	:m_duration(0.f), m_increase(0.f), m_isEndDuration(false), m_type(type),
	m_isFirstUpdate(true), m_hungryPointer(nullptr), m_playerTM(Matrix::Identity),
	m_player(std::weak_ptr<Entity>{}), m_isFirst(true), m_skillCount(0), m_fade(0.f)
{

}

UITestScript::~UITestScript()
{
	//delete m_hungryPointer;
	m_player.reset();
}

void UITestScript::Update(double dt)
{
	switch (m_type)
	{
		case UIType::STATIC:
		{

		}
		break;
		case UIType::ONMOUSE:
		{
			Vector3 scale = GetTransform()->GetWorldScale();
			if (m_isEndDuration == false)
			{
				m_duration += dt;
				//scale *= 1.002f;
				scale.x *= 1.002f;
			}
			else
			{
				m_duration -= dt;
				scale.x *= 0.998f;
			}
			if (m_duration > 1.f)
			{
				m_isEndDuration = true;
			}
			else if (m_duration <= 0.f)
			{
				m_isEndDuration = false;
			}
			GetTransform()->SetWorldScale(scale);
		}
		break;

		case UIType::GAUGE:
		{
			if (m_isFirstUpdate)
			{
				m_position = GetTransform()->GetWorldPosition();
				m_isFirstUpdate = false;
			}

			m_duration += 0.2f * dt;
			if (m_duration > 1.0f)
			{
				//m_position = GetTransform()->GetWorldPosition();

				m_duration -= 1.0f;
			}

			m_increase += dt;		// 싸인 함수에 넣을 계속 증가 시키는 값
			float distance = 3.0f;	// 떨리는 거리
			float Speed = 100.0f;	// 떨리는 속도

			// 중심에서의 x, y 위치 계산
			float offsetX = distance * sin(m_increase * Speed);

			// 중심을 기준으로 떨리게 설정
			if (m_duration > 0.5f)
			{
				GetTransform()->SetWorldPosition({ m_position.x + offsetX, m_position.y , 0.0f });
			}
		}
		break;

		case UIType::Jiwon:
		{
			if (m_player.lock())
			{
				if (m_player.lock()->HasComponent<PlayerMoveScript>())
				{
					if (m_player.lock()->GetComponent<PlayerMoveScript>()->AbleSkill())
					{
						if (!m_aimModeActivated) // 조준게이지가 안떠있는 평소 상태
						{
							if (InputManager::GetInstance()->IsKeyPress(VK_LBUTTON))
							{
								m_timeSinceLClick += dt;
							}

							if (m_timeSinceLClick > 0.5f) // 0.5초 이상 좌클릭을 누르고있으면 조준게이지가 활성화
							{
								/// 조준게이지 스타트
								m_isFirst = false;

								m_aimModeActivated = true;
								m_gaugePower = 0.f;
								m_duration = m_gaugePower;
								m_player.lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Lifting);
								GetOwner().lock()->SetRender(true);
								SoundManager::GetInstance()->PlaySFX("Potion_Open.mp3");
								SoundManager::GetInstance()->PlaySFX("Potion_Charging_Up.mp3", true);
							}
						}
						else
						{
							/// 조준게이지 업데이트
							// 게이지바가 위아래로 왔다갔다 하는거
							m_gaugePower = m_duration;
							if (m_aimGaugeGoingUp)
							{
								if (m_gaugePower + m_gaugeSpeed * dt < 1.f)
								{
									m_gaugePower += m_gaugeSpeed * dt;
								}
								else
								{
									m_aimGaugeGoingUp = false;
									SoundManager::GetInstance()->StopSFX("Potion_Charging_Up.mp3");
									SoundManager::GetInstance()->PlaySFX("Potion_Charging_Down.mp3", true);
								}
							}
							else
							{
								if (m_gaugePower - m_gaugeSpeed * dt > 0.f)
								{
									m_gaugePower -= m_gaugeSpeed * dt;
								}
								else
								{
									m_aimGaugeGoingUp = true;
									SoundManager::GetInstance()->StopSFX("Potion_Charging_Down.mp3");
									SoundManager::GetInstance()->PlaySFX("Potion_Charging_Up.mp3", true);
								}
							}
							m_duration = m_gaugePower;

							// 우클릭시 조준게이지 취소
							if (InputManager::GetInstance()->IsKeyUp(VK_RBUTTON) || InputManager::GetInstance()->IsKeyDown(VK_RBUTTON))
							{
								m_aimModeActivated = false;
								m_timeSinceLClick = 0.f;
								GetOwner().lock()->SetRender(false);
								m_player.lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);
								m_player.lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);
								SoundManager::GetInstance()->StopSFX("Potion_Charging_Up.mp3");
								SoundManager::GetInstance()->StopSFX("Potion_Charging_Down.mp3");
								break;
							}

							if (InputManager::GetInstance()->IsKeyUp(VK_LBUTTON))
							{
								/// 조준게이지 끝, 발사
								m_aimModeActivated = false;
								m_timeSinceLClick = 0.f;
								GetOwner().lock()->SetRender(false);
								SoundManager::GetInstance()->PlaySFX("Potion_Throw.mp3");
								SoundManager::GetInstance()->StopSFX("Potion_Charging_Up.mp3");
								SoundManager::GetInstance()->StopSFX("Potion_Charging_Down.mp3");

								m_player.lock()->GetComponent<PlayerMoveScript>()->ThrowItem(m_gaugePower);
								m_player.lock()->GetComponent<PlayerFSM>()->ChangeState(PlayerFSM::Player::Idle);

								//GetOwner().lock()->
								//m_player->GetComponent<PlayerMoveScript>()->ShootingTest(m_gaugePower); // 아이템들은 씬에 있어야할것 같아서 발사 부분을 플레이어 스크립트에서 빼봄
							}
						}
					}
			}
		
			
			}
			break;
		}
		case UIType::HUNGRY:
		{
			if (m_isFirstUpdate)
			{
				m_position = GetTransform()->GetWorldPosition();
				m_isFirstUpdate = false;

				m_textRender = m_owner.lock()->GetComponent<TextRender>();
				m_textRender->m_leftTop = Vector2(1780, 930);
				m_textRender->m_color = Vector4(0, 0, 0, 0.5);
				m_textRender->m_fontSize = 30;

			}
			//*m_hungryPointer = 0.9f;
// 			*m_hungryPointer -= 0.2f * dt;
// 			
// 			if (*m_hungryPointer < 0.0f)
// 			{
// 				//m_position = GetTransform()->GetWorldPosition();
// 
// 				*m_hungryPointer = 1.0f;
// 			}
			// 100보다 더 크면 100으로 세팅
			if (*m_hungryPointer > 1.0f)
			{
				*m_hungryPointer = 1.0f;
			}
			m_textRender->m_text = std::to_wstring(int(*m_hungryPointer * 100)) + L"%";

			// 	m_increase += dt;		// 싸인 함수에 넣을 계속 증가 시키는 값
			// 	float distance = 3.0f;	// 떨리는 거리
			// 	float Speed = 100.0f;	// 떨리는 속도
			// 
			// 	// 중심에서의 x, y 위치 계산
			// 	float offsetX = distance * sin(m_increase * Speed);
			// 
			// 	// 중심을 기준으로 떨리게 설정
			// 	if (m_duration > 0.5f)
			// 	{
			// 		GetTransform()->SetWorldPosition({ m_position.x + offsetX, m_position.y , 0.0f });
			// 	}
		}
		break;
		case UIType::SENSE:
		{
			//m_duration = GetTransform()->GetParent().lock()->GetOwner().lock()->GetComponent<ShopKeeperFSM>()->GetSense();

			//// 눈치 게이지 Off 상태
			//if (!m_isSenseActive)
			//{
			//	if (m_duration > 0)
			//	{
			//		m_isSenseActive = true;
			//		GetOwner().lock()->SetRender(true);
			//	}
			//}
			//// 눈치 게이지 On 상태
			//else
			//{
			//	if (m_duration <= 0)
			//	{
			//		m_isSenseActive = false;
			//		GetOwner().lock()->SetRender(false);
			//	}
			//}
		}
		break;
		case UIType::MINIMAP:
		{
			if (m_isMinimapOn)
			{
				Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();
				if (InputManager::GetInstance()->IsKeyUp(VK_TAB))
				{
					GetOwner().lock()->SetRender(false);
					m_isMinimapOn = false;
				}
				else if (1400.f <= mousePos.x && mousePos.x <= 1500.f
					&& 300.f <= mousePos.y && mousePos.y <= 400.f
					&& InputManager::GetInstance()->IsKeyUp(VK_LBUTTON))
				{
					GetOwner().lock()->SetRender(false);
					m_isMinimapOn = false;
				}
			}
			else
			{
				if (InputManager::GetInstance()->IsKeyUp(VK_TAB))
				{
					GetOwner().lock()->SetRender(true);
					m_isMinimapOn = true;
				}
			}
		}
		break;
		case UIType::MINIMAPCURPOS:
		{
			if (m_isMinimapOn)
			{
				Vector2 mousePos = InputManager::GetInstance()->GetMousePosition();
				if (InputManager::GetInstance()->IsKeyUp(VK_TAB))
				{
					GetOwner().lock()->SetRender(false);
					m_isMinimapOn = false;
					SoundManager::GetInstance()->PlaySFX("MiniMap_Close.wav");
				}
				else if (1400.f <= mousePos.x && mousePos.x <= 1500.f
					&& 300.f <= mousePos.y && mousePos.y <= 400.f
					&& InputManager::GetInstance()->IsKeyUp(VK_LBUTTON))
				{
					GetOwner().lock()->SetRender(false);
					m_isMinimapOn = false;
					SoundManager::GetInstance()->PlaySFX("MiniMap_Close.wav");
				}
			}
			else
			{
				if (InputManager::GetInstance()->IsKeyUp(VK_TAB))
				{
					GetOwner().lock()->SetRender(true);
					m_isMinimapOn = true;
					SoundManager::GetInstance()->PlaySFX("MiniMap_Open.wav");
				}
			}
		}
		break;
		case UIType::SKILL:
		{
			if (m_isFirstUpdate)
			{
				m_position = GetTransform()->GetWorldPosition();
				m_isFirstUpdate = false;

				m_textRender = m_owner.lock()->GetComponent<TextRender>();
				m_textRender->m_leftTop = Vector2(1840, 630);
				m_textRender->m_color = Vector4(0, 0, 0, 0.5);
				m_textRender->m_fontSize = 25;

			}
			m_skillCount = m_player.lock()->GetComponent<PlayerMoveScript>()->GetSkillCount();
 
			m_textRender->m_text = L"x"+std::to_wstring(m_skillCount);
		}

		break;
		case UIType::FADE:
		{
			if (EventManager::GetInstance()->GetCurScene()->GetSceneName() != L"Title")
			{
				//m_duration = 1.f;
				if (m_isFirstUpdate)
				{
					m_duration = 1.5f;
					m_isFirstUpdate = false;
				}

				m_duration -= dt;
				if (m_duration < 0.0f)
				{
					m_duration = 0.0f;
				}
			}

		}
		break;
		default:
			break;
	}

	//GetTransform()->SetWorldScale({ 970 / (float)100 * fScaleWidth * fRateWidth, 15 / (float)100 * fRateHeight, 0 });
	//GetTransform()->SetWorldPosition({ -(970 / (float)100 * fRateWidth), 15 / (float)100 * fRateHeight, 0 });
}

void UITestScript::Render(double dt)
{

}

void UITestScript::Finalize()
{

}

std::shared_ptr<Component> UITestScript::Clone() const
{
	std::shared_ptr<UITestScript> clone = std::make_shared<UITestScript>();

	return clone;
}

DirectX::SimpleMath::Matrix UITestScript::GetPlayerWorldTM()
{
	 return m_player.lock()->GetComponent<Transform>()->GetWorldMatrix(); 
}

DirectX::SimpleMath::Vector3 UITestScript::GetPlayerPosition()
{
	if (m_player.lock()->HasComponent<Transform>())
		return m_player.lock()->GetComponent<Transform>()->GetWorldPosition();

	return Vector3{};
}

