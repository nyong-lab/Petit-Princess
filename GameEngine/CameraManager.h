#pragma once
#include "IManager.h"
#include <memory>

class CameraModule;
class IGraphics;

/// <summary>
/// 카메라를 제어하는 매니저 클래스
/// 
/// 수정자 : 김동일 -> 선택된 엔티티에 부드럽게 접근하기 위한 변수 및 함수 추가
/// </summary>
class CameraManager : public IManager
{
public:
	CameraManager();
	virtual ~CameraManager();

	CameraManager(const CameraManager& manager) = delete;
	CameraManager& operator=(const CameraManager& manager) = delete;

	CameraManager(const CameraManager&& manager) = delete;
	CameraManager& operator=(const CameraManager&& manager) = delete;

	virtual void Initialize() override;
	virtual void Initialize(HWND hwnd);
	virtual void Start() override {};
	virtual void Update(double dt) override;
	virtual void Finalize() override;

	void LookAt(Vector3 target, float distance);
	std::shared_ptr<CameraModule> GetMainCamera() const { return m_mainCamera; }

	void CameraMove(int dx, int  dy);
	void Resize(int width, int height);

private:
	std::shared_ptr<CameraModule> m_mainCamera;

	const float m_cameraToTargetSecond = 0.25f;

	float m_elapsedTime;
	Vector3 m_targetPosition;
	float m_cameraSpeed;
	bool m_isArrived;
};

