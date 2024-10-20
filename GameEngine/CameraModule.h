#pragma once

struct InitialCameraData
{
	Vector3 position;
	float yawRadian;
	float pitchRadian;
	float maxPitch;
	float minPitch;
	float nearClipDistance;
	float farClipDistance;
	float fov;
	ProjectionMode projectionMode;
};

enum class ProjectionMode
{
	Perspective,
	Orthographic
};

/// <summary>
/// 카메라
/// 나중에 예쁘게 감싸서 인터페이스화 하자
/// </summary>
class CameraModule
{
	GENERATE_CLASS_TYPE_INFO(CameraModule)
		PROPERTY(m_position)
		PROPERTY(m_pitchRadian)
		PROPERTY(m_yawRadian)
		PROPERTY(m_nearClipDistance)
		PROPERTY(m_farClipDistance)
		PROPERTY(m_projectionMode)
		PROPERTY(m_fov)

public:
/// 생성자와 소멸자 ---------------------------------------------------
	CameraModule(int windowWidth, int windowHeight);
	~CameraModule();

	/// 멤버 함수 ---------------------------------------------------
public:
	void Update(float deltaTime);
	void Resize(int width, int height);

	const Matrix& GetViewProjectionMatrix() { return m_viewProjMatrix; }
	const Matrix& GetViewMatrix() { return m_viewMatrix; }
	const Matrix& GetProjMatrix() { return m_projMatrix; }
	const Matrix& GetOrthoMatrix() { return m_viewOrthoMatrix; }
	//const dx::XMMATRIX& GetProjectionMatrix();

//------- getter, setter ---------------------------------------
	Vector3	 GetPosition() const			{ return m_position; }
	Vector3& GetPosition()					{ return m_position; }  // ImGui를 위해 레퍼런스 반환 함수 생성 - Dongil		
	Matrix GetCameraWorldMatrix()			{ return Matrix::CreateTranslation(m_position.x, m_position.y, m_position.z); }
	void SetPosition(Vector3 val)			{ m_position = val; }
	void AddPosition(Vector3 val)			{ m_position += val; }
	Vector2	GetLookRadian() const			{ return Vector2(m_yawRadian, m_pitchRadian); }
	void SetLookRadian(Vector2 val)			{ m_yawRadian = val.x, m_pitchRadian = val.y; }
	void AddLookRadian(Vector2 val)			{ m_yawRadian += val.x, m_pitchRadian += val.y; }
	Vector3	GetLookVector()					{ return m_lookVector; }
	
	float GetMinClipDistance() const		{ return m_nearClipDistance; }
	void SetMinClipDistance(float val)		{ m_nearClipDistance = val; }
	float GetMaxClipDistance() const		{ return m_farClipDistance; }
	void SetMaxClipDistance(float val)		{ m_farClipDistance = val; }
	
	ProjectionMode GetProjectionMode()		{ return m_projectionMode; }
	void SetProjectionMode(ProjectionMode mode) { m_projectionMode = mode; }

	void SetInitialData();
	
	float GetFOV() const					{ return m_fov; }
	void SetFOV(float val)					{ m_fov = val; }
	void AddFOV(float val)					{ m_fov += val; }

	Vector2 GetScreenSize()					{ return m_size; }

	/// 멤버 변수 ---------------------------------------------------
private:
	Vector3 m_position;
	Matrix m_cameraWorldMatrix;

	float m_pitchRadian;
	float m_yawRadian;
	Vector3 m_lookVector;

	float m_nearClipDistance;
	float m_farClipDistance;

	float m_maxPitch;
	float m_minPitch;

	ProjectionMode m_projectionMode;
	float m_fov; // 가로 폭 시야각(radian), pers일때만 사용.  

	Vector2 m_size; //클리핑 평면의 크기(width, height), orth일때만 사용. 화면의 종횡비를 유지해야한다. 클수록 많이 보인다
	float m_aspectRatio;

	Matrix m_viewMatrix;
	Matrix m_projMatrix;
	Matrix m_viewProjMatrix;
	Matrix m_viewOrthoMatrix;

	InitialCameraData m_initialData;

	///꿀팁
	// 1. 속도감을 느끼려면 시야각을 크게 하자
	// 2. 바짝붙은 면끼리 깜빡거린다면 근접 평면을 가능한 크게 설정하라 
	// 3. fov ~ 일반적인 3D : 60~90degree, 시뮬레이션 및 비행 90~, 아케이드 45~60
	// 45deg : 0.7854rad  ~ 60deg : 1.0472rad ~ 90 : 1.5708 rad
};
