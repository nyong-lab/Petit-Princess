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
/// ī�޶�
/// ���߿� ���ڰ� ���μ� �������̽�ȭ ����
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
/// �����ڿ� �Ҹ��� ---------------------------------------------------
	CameraModule(int windowWidth, int windowHeight);
	~CameraModule();

	/// ��� �Լ� ---------------------------------------------------
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
	Vector3& GetPosition()					{ return m_position; }  // ImGui�� ���� ���۷��� ��ȯ �Լ� ���� - Dongil		
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

	/// ��� ���� ---------------------------------------------------
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
	float m_fov; // ���� �� �þ߰�(radian), pers�϶��� ���.  

	Vector2 m_size; //Ŭ���� ����� ũ��(width, height), orth�϶��� ���. ȭ���� ��Ⱦ�� �����ؾ��Ѵ�. Ŭ���� ���� ���δ�
	float m_aspectRatio;

	Matrix m_viewMatrix;
	Matrix m_projMatrix;
	Matrix m_viewProjMatrix;
	Matrix m_viewOrthoMatrix;

	InitialCameraData m_initialData;

	///����
	// 1. �ӵ����� �������� �þ߰��� ũ�� ����
	// 2. ��¦���� �鳢�� �����Ÿ��ٸ� ���� ����� ������ ũ�� �����϶� 
	// 3. fov ~ �Ϲ����� 3D : 60~90degree, �ùķ��̼� �� ���� 90~, �����̵� 45~60
	// 45deg : 0.7854rad  ~ 60deg : 1.0472rad ~ 90 : 1.5708 rad
};
