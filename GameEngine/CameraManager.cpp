#include "GameEngine_pch.h"
#include "CameraManager.h"
#include "CameraModule.h"
#include "GraphicsEngine.h"

#include "InputManager.h"
#include "../GameDemo/imgui.h"

CameraManager::CameraManager()
	: m_isArrived(true)
	, m_cameraSpeed(100.f)
	, m_elapsedTime(0.f)
	, m_cameraToTargetSecond(0.25f)
{
}

void CameraManager::Initialize()
{

}

void CameraManager::Initialize(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	int screenHeight = rect.bottom - rect.top;
	int screenWidth = rect.right - rect.left;

	m_mainCamera = std::make_shared<CameraModule>(screenWidth, screenHeight);
	m_mainCamera->SetPosition(Vector3(0, 3, -15));
}

void CameraManager::Update(double dt)
{
	float speed = 10.0f * dt;
	float mouseSpeed = 0.1f;

	if (InputManager::GetInstance()->IsKeyPress(VK_SHIFT))
	{
		speed *= 2;
		mouseSpeed *= 2;
	}


	// 바라보는 방향 기준으로 이동
	Vector3 v = m_mainCamera->GetLookVector();
	Vector2 frontVec(v.x, v.z);
	frontVec.Normalize();
	Vector2 leftVec(-frontVec.y, frontVec.x);


	// 상하 이동
	if (InputManager::GetInstance()->GetKeyState('Q') == KEYSTATE::Down
		|| InputManager::GetInstance()->GetKeyState('Q') == KEYSTATE::Press)
	{
		m_mainCamera->AddPosition(Vector3(0.f, speed, 0.f));
	}

	if (InputManager::GetInstance()->GetKeyState('E') == KEYSTATE::Down
		|| InputManager::GetInstance()->GetKeyState('E') == KEYSTATE::Press)
	{
		m_mainCamera->AddPosition(Vector3(0.f, -speed, 0.f));
	}


	// 앞뒤 이동
	if (InputManager::GetInstance()->GetKeyState(VK_UP) == KEYSTATE::Down
		|| InputManager::GetInstance()->GetKeyState(VK_UP) == KEYSTATE::Press)
	{
		m_mainCamera->AddPosition(Vector3(frontVec.x * speed, 0.f, frontVec.y * speed));
	}

	if (InputManager::GetInstance()->GetKeyState(VK_DOWN) == KEYSTATE::Down
		|| InputManager::GetInstance()->GetKeyState(VK_DOWN )== KEYSTATE::Press)
	{
		m_mainCamera->AddPosition(Vector3(-frontVec.x * speed, 0.f, -frontVec.y * speed));
	}

	// 좌우 이동
	if (InputManager::GetInstance()->GetKeyState(VK_LEFT) == KEYSTATE::Down
		|| InputManager::GetInstance()->GetKeyState(VK_LEFT) == KEYSTATE::Press)
	{
		m_mainCamera->AddPosition(Vector3(leftVec.x * speed, 0.f, leftVec.y * speed));
	}

	if (InputManager::GetInstance()->GetKeyState(VK_RIGHT) == KEYSTATE::Down
		|| InputManager::GetInstance()->GetKeyState(VK_RIGHT) == KEYSTATE::Press)
			{
		m_mainCamera->AddPosition(Vector3(-leftVec.x * speed, 0.f, -leftVec.y * speed));
	}

#ifdef _DEBUG
	if (!ImGui::GetIO().WantCaptureMouse)
	{
#endif	
		//마우스 회전
		if (InputManager::GetInstance()->GetKeyState(VK_RBUTTON) == KEYSTATE::Press
			|| InputManager::GetInstance()->GetKeyState(VK_RBUTTON) == KEYSTATE::Down)
		{
			Vector2 dxdy = InputManager::GetInstance()->GetMouseDxdy();
			m_mainCamera->AddLookRadian(Vector2(dt * mouseSpeed * dxdy.x, dt * mouseSpeed * -dxdy.y));

			/*int dx = m_curMousePosX - m_preMousePosX;
			int dy = m_curMousePosY - m_preMousePosY;

			float mouseSpeed = 0.01f;
			m_mainCamera->AddLookRadian(Vector2(static_cast<float>(dt * mouseSpeed * dx), static_cast<float>(dt * mouseSpeed * -dy)));*/
		}

#ifdef _DEBUG
	}
#endif	

	/// Entity로 카메라 이동처리
	if (m_isArrived == false)
	{
		m_elapsedTime += dt;

		Vector3 cameraPos = m_mainCamera->GetPosition();

		Vector3 dir = m_targetPosition - cameraPos;
		dir.Normalize();
		cameraPos += dir * m_cameraSpeed * dt;

		m_mainCamera->SetPosition(cameraPos);

		if (m_elapsedTime >= m_cameraToTargetSecond)
		{
			m_isArrived = true;
			m_mainCamera->SetPosition(m_targetPosition);
		}
	}

	m_mainCamera->Update(dt);
}

void CameraManager::Finalize()								
{

}

void CameraManager::LookAt(Vector3 target, float distance)
{
	Matrix rotationMat = Matrix::CreateFromQuaternion(Quaternion::CreateFromRotationMatrix(m_mainCamera->GetCameraWorldMatrix()));
	Vector3 zAxis(rotationMat._31, rotationMat._32, rotationMat._33);
	m_targetPosition = target + zAxis * -distance;

	Vector3 pos = m_mainCamera->GetPosition();

	float length = (pos - m_targetPosition).Length();

	if (length < 1.f)
	{
		m_mainCamera->SetPosition(m_targetPosition);
		return;
	}

	m_cameraSpeed = length / m_cameraToTargetSecond;

	m_isArrived = false;
	m_elapsedTime = 0.f;
}


CameraManager::~CameraManager()
{
}

void CameraManager::CameraMove(int dx, int dy)
{
	m_mainCamera->AddLookRadian(Vector2(static_cast<float>(dx), static_cast<float>(dy)));
}

void CameraManager::Resize(int width, int height)
{
	m_mainCamera->Resize(width, height);
}

///----------------------------------------------------------------------------------------------
///                                       카메라 사용법 예시
///----------------------------------------------------------------------------------------------
/*
	float speed = deltaTime * moveSpeed_;

	// 상하 이동
	if (keyManager_->IsKeyHold(KEY::SPACE))
	{
		camera_->AddPosition(Vector3(0.f, speed, 0.f));
	}

	// 바라보는 방향 기준으로 이동
	{
		Vector3 v = camera_->GetLookVector();
		Vector2 frontVec(v.x, v.z);
		frontVec.Normalize();
		Vector2 leftVec(-frontVec.y, frontVec.x);

		//앞
		if (keyManager_->IsKeyHold(KEY::W))
		{
			camera_->AddPosition(Vector3(frontVec.x * speed, 0.f, frontVec.y * speed));
		}

		//좌
		if (keyManager_->IsKeyHold(KEY::A))
		{
			camera_->AddPosition(Vector3(leftVec.x * speed, 0.f, leftVec.y * speed));
		}
	}

	// 시점 이동
	camera_->AddLookRadian(Vector2(deltaTime * sightSpeed_ * mouseDxdy_.x, deltaTime * sightSpeed_ * -mouseDxdy_.y));
*/