#include "GameEngine_pch.h"
#include "UIRenderer.h"

#include "Transform.h"
#include "Entity.h"
#include "CameraManager.h"
#include "../GameDemo/MiniGameManager.h"
#include "../GameDemo/UITestScript.h"
#include "../GameDemo/QuickTimeEventScript.h"
#include "../GameDemo/InterKeyScript.h"
#include "../GameDemo/TimerUI.h"
#include "../GameDemo/MainUI.h"
#include "../GameDemo/ScoreUI.h"
#include "../GameDemo/PopUpUI.h"
#include "../GameDemo/MerchantUI.h"
#include "../GameDemo/HideInterKeyScript.h"
#include "../GameDemo/GuardUI.h"

UIRenderer::UIRenderer()
	: Renderer()
{

}

UIRenderer::UIRenderer(const std::string& name)
	: Renderer(name)
{
 
}

void UIRenderer::Update(double dt)
{

}

void UIRenderer::Finalize()
{

}

void UIRenderer::Render(double dt)
{
	auto pMesh = GetOwner().lock()->GetComponent<MeshFilter>();
	if (!pMesh) return;

	auto transform = GetTransform();
	auto camera = GetOwner().lock()->GetCameraManager().lock()->GetMainCamera();
	auto UIScript = GetOwner().lock()->GetComponent<UITestScript>();
	auto MiniGameScript = GetOwner().lock()->GetComponent<QuickTimeEventScript>();
	auto InterScript = GetOwner().lock()->GetComponent<InterKeyScript>();
	auto timerUI = GetOwner().lock()->GetComponent<TimerUI>();
	auto mainUI = GetOwner().lock()->GetComponent<MainUI>();
	auto scoreUI = GetOwner().lock()->GetComponent<ScoreUI>();
	auto popup = GetOwner().lock()->GetComponent<PopUpUI>();
	auto merchant = GetOwner().lock()->GetComponent<MerchantUI>();
	auto miniGameManager = MiniGameManager::GetInstance();
	auto hideInterScript = GetOwner().lock()->GetComponent<HideInterKeyScript>();
	auto guard = GetOwner().lock()->GetComponent<GuardUI>();

	UITransform uitransform;
	CbUI cbUI;

	uitransform.world = transform->GetWorldMatrix();

	/// 미니게임 매니저에서 받아오는 것들은 맨 위로
	/// 다른곳에서 값 겹칠수도 있어서 순서 바꾸면 안됨
	cbUI.eatFoods = miniGameManager->GetClearEatSpotList();
	cbUI.meshCount = 0;

	if (UIScript)
	{
		cbUI.typeNum = static_cast<int>(UIScript->GetType());

		if (UIScript->GetType() == UIType::HUNGRY)
		{
			cbUI.textureTranslation = UIScript->GetHungry();
			uitransform.worldViewProj = camera->GetOrthoMatrix();
		}
		else if (UIScript->GetType() == UIType::Jiwon)
		{
			cbUI.textureTranslation = UIScript->GetDuration();
			cbUI.isFirst = UIScript->GetIsFirst();

			Vector2 cameraLookRadian = camera->GetLookRadian();
			Matrix newScalse = Matrix::CreateScale(0.05);
			Matrix newRotation = Matrix::CreateRotationX(-cameraLookRadian.y);
			Vector3 offset = {0,3,0};
			uitransform.world = newScalse * newRotation *Matrix::CreateTranslation(UIScript->GetPlayerPosition()+ offset);
			uitransform.worldViewProj =  camera->GetViewProjectionMatrix();
			//uitransform.worldViewProj = Matrix::CreateTranslation(UIScript->GetPlayerPosition()) * camera->GetViewProjectionMatrix();
		}
		else if (UIScript->GetType() == UIType::SKILL)
		{
			uitransform.worldViewProj = camera->GetOrthoMatrix();
		}
		else if (UIScript->GetType() == UIType::FADE)
		{
			cbUI.textureTranslation = UIScript->GetDuration();
			uitransform.worldViewProj = camera->GetOrthoMatrix();
		}
		else
		{
			cbUI.textureTranslation = UIScript->GetDuration();
			uitransform.worldViewProj = camera->GetOrthoMatrix();
		}
	}

	if (InterScript)
	{
		cbUI.isActive = InterScript->GetInteractive();

		Vector2 cameraLookRadian = camera->GetLookRadian();
		Matrix newScalse = Matrix::CreateScale(0.01);
		Matrix newRotation = Matrix::CreateRotationX(-cameraLookRadian.y);
		Vector3 offset = { 0,1,0 };
		uitransform.world = newScalse * newRotation * Matrix::CreateTranslation(InterScript->GetObjectPosition() + offset);
		uitransform.worldViewProj = camera->GetViewProjectionMatrix();
	}

	if (hideInterScript)
	{
		//cbUI.isActive = hideInterScript->GetInteractive();
		//uitransform.worldViewProj = transform->GetWorldMatrix() * camera->GetViewProjectionMatrix();

		Vector2 cameraLookRadian = camera->GetLookRadian();
		Matrix newScalse = Matrix::CreateScale(0.01);
		Matrix newRotation = Matrix::CreateRotationX(-cameraLookRadian.y);
		Vector3 offset = { 0,1,0 };
		uitransform.world = newScalse * newRotation * Matrix::CreateTranslation(hideInterScript->GetObjectPosition() + offset);
		uitransform.worldViewProj = camera->GetViewProjectionMatrix();
	}

	if (MiniGameScript)
	{
		auto info = MiniGameScript->GetMiniGameInfo();
		cbUI.stage = info->stage;
		cbUI.progress = info->progress;
		cbUI.typeNum = static_cast<int>(MiniGameScript->GetFoodType());
		cbUI.typeNum2 = static_cast<int>(MiniGameScript->GetMiniGameType());
		cbUI.meshCount = 0;
		cbUI.direction = static_cast<int>(MiniGameScript->GetDragDirection());
		cbUI.pos = MiniGameScript->GetUIPosition();
		cbUI.size = MiniGameScript->GetUISize();
		cbUI.keyChains = MiniGameScript->GetKeyChains();
	}

	if (timerUI)
	{
		cbUI.pos = timerUI->m_position;
		cbUI.size = timerUI->m_imgSize;
		cbUI.textureTranslation = timerUI->m_curAngle;
	}
	
	if (scoreUI)
	{
		cbUI.pos = scoreUI->m_basePos;
		cbUI.size = scoreUI->m_imgSize;
	}

	if (popup)
	{
		cbUI.pos = popup->m_pos;
		cbUI.size = popup->m_size;
	}

	if (mainUI)
	{
		cbUI.isActive = mainUI->m_isOnExit;
		cbUI.isActive2 = mainUI->m_isOnCredit;
		cbUI.fade = mainUI->GetFade();
	}

	if (merchant)
	{
		cbUI.typeNum = merchant->m_state;
		cbUI.typeNum2 = merchant->m_foodtype;
		cbUI.textureTranslation = merchant->m_ratio;

		cbUI.pos = merchant->m_pos;
		cbUI.size = merchant->m_size;
	}

	if (guard)
	{
		cbUI.typeNum = guard->m_state;

		Vector2 cameraLookRadian = camera->GetLookRadian();
		Matrix newScalse = Matrix::CreateScale(0.02);
		Matrix newRotation = Matrix::CreateRotationX(-cameraLookRadian.y);
		Vector3 offset = { 0,3.5,0 };
		uitransform.world = newScalse * newRotation * Matrix::CreateTranslation(guard->m_pos + offset);
		uitransform.worldViewProj = camera->GetViewProjectionMatrix();
	}

	ModelData UIData;
	UIData.modelKey = pMesh->GetModelKey();
	UIData.uiTransform = uitransform;
	UIData.ui = cbUI;

	GetOwner().lock()->GetGraphics().lock()->UIRender(UIData);
}

std::shared_ptr<Component> UIRenderer::Clone() const
{
	std::shared_ptr<UIRenderer> clone = std::make_shared<UIRenderer>();

	return clone;
}
