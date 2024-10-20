#include "GameEngine_pch.h"
#include "CameraModule.h"


CameraModule::CameraModule(int windowWidth, int windowHeight)
	: m_position{0.f,0.f,0.f}, m_yawRadian(0.f), m_pitchRadian(0.f)
	, m_maxPitch(1.5), m_minPitch(-1.5)
	, m_nearClipDistance(0.1f), m_farClipDistance(10000.f)
	, m_projectionMode(ProjectionMode::Perspective)
	, m_fov(1.0472f)
	, m_size{ static_cast<float>(windowWidth), static_cast<float>(windowHeight) }
	, m_aspectRatio(windowWidth / static_cast<float> (windowHeight))

{
	Vector3 direction;
	direction.x = cosf(m_pitchRadian) * sinf(m_yawRadian);
	direction.y = sinf(m_pitchRadian);
	direction.z = cosf(m_pitchRadian) * cosf(m_yawRadian);

	m_lookVector = DirectX::XMVector3Normalize(XMLoadFloat3(&direction));

	m_initialData.position = m_position;
	m_initialData.yawRadian = m_yawRadian;
	m_initialData.pitchRadian = m_pitchRadian;
	m_initialData.maxPitch = m_maxPitch;
	m_initialData.minPitch = m_minPitch;
	m_initialData.nearClipDistance = m_nearClipDistance;
	m_initialData.farClipDistance = m_farClipDistance;
	m_initialData.fov = m_fov;
	m_initialData.projectionMode = m_projectionMode;
}

CameraModule::~CameraModule()
{
}

void CameraModule::Update(float deltaTime)
{
	///카메라 회전 값 계산
// 	yawRadian_ = XMConvertToRadians(yawRadian_);
// 	pitchRadian_ = XMConvertToRadians(pitchRadian_);

	if (m_pitchRadian > m_maxPitch) m_pitchRadian = m_maxPitch;
	else if (m_pitchRadian < m_minPitch) m_pitchRadian = m_minPitch;

	Vector3 direction;
	direction.x = cosf(m_pitchRadian) * sinf(m_yawRadian);
	direction.y = sinf(m_pitchRadian);
	direction.z = cosf(m_pitchRadian) * cosf(m_yawRadian);
	m_lookVector = DirectX::XMVector3Normalize(XMLoadFloat3(&direction));

	/// 뷰 행렬 만들기
	m_viewMatrix = XMMatrixLookToLH(
		DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 1.f),  // 카메라 위치
		m_lookVector, // 바라보는 각도?
		DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f) //업벡터
	);

	if (m_nearClipDistance < 0.1f)
		m_nearClipDistance = 0.1f;
	if (m_farClipDistance < 0.5f)
		m_farClipDistance = 0.5f;


	/// 프로젝션 행렬 만들기
	if (m_projectionMode == ProjectionMode::Perspective)
	{
		m_projMatrix = DirectX::XMMatrixPerspectiveFovLH(
			m_fov, //시야각
			m_aspectRatio, //화면 가로세로 비율
			m_nearClipDistance,
			m_farClipDistance
		);
		//		projectionMatrix_ = dx::XMMatrixPerspectiveLH(1.f, 9.f / 14.f, 0.5f, 10.f);  //clip Z


	}
	else //ORTHOGRAPHIC
	{
		m_projMatrix = DirectX::XMMatrixOrthographicLH(
			m_size.x * 0.1f,  //화면 가로 크기?
			m_size.y * 0.1f,  //화면 세로 크기??
			m_nearClipDistance,
			m_farClipDistance
		);
	}

	//UI test
	m_viewOrthoMatrix = DirectX::XMMatrixOrthographicLH(
		m_size.x,  //화면 가로 크기?
		m_size.y,  //화면 세로 크기??
		m_nearClipDistance,
		m_farClipDistance
	);

	/// 두 행렬 곱하기
	m_viewProjMatrix = m_viewMatrix * m_projMatrix;
}

void CameraModule::Resize(int width, int height)
{
	m_size = Vector2(width, height);
	m_aspectRatio = width / static_cast<float> (height);
}

void CameraModule::SetInitialData()
{
	m_position = m_initialData.position;
	m_yawRadian = m_initialData.yawRadian;
	m_pitchRadian = m_pitchRadian = m_initialData.pitchRadian;
	m_maxPitch = m_initialData.maxPitch;
	m_minPitch = m_initialData.minPitch;
	m_nearClipDistance = m_initialData.nearClipDistance;
	m_farClipDistance = m_initialData.farClipDistance;
	m_fov = m_initialData.fov;
	m_projectionMode = m_initialData.projectionMode;
}
