#pragma once
#include "Component.h"

#include <vector>


/// 물체 크기/위치/회전을 담당하는 Component
/// + Parent - Child 관계를 통해 상대적인 Transform을 관리
/// 
/// https://docs.unity3d.com/ScriptReference/Transform.html
/// 
/// 24.01.08 이서영 유승운 
/// 
/// 24.01.19 [수정] 김동일
/// - 스마트 포인터 작업 및 에디터 계층구조 관련
/// 
/// 24.01.25 [수정] 박연하 
/// - SimpleMath로 교체
/// - local 및 world 변수 추가 / UpdateTransform 함수 추가
/// 
/// 24.01.28 [수정] 손서희
/// - 함수명 Local, World 구분
/// 
/// 24.01.28 [수정] 김동일
/// - 변수, 함수 Local, World 유니티 처럼 동작하도록 수정
/// - !!  계층구조 변경 시 자식 로컬 로테이션 * 부모의 월드 역행렬 계산식 오류 있음 개선 해야 함 !! (포지션, 스케일은 정상)
class Transform :
	public Component, public std::enable_shared_from_this<Transform>
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Transform)
		PROPERTY(m_localPosition)
		PROPERTY(m_localRotation)
		PROPERTY(m_localScale)

	// Serialize
	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	// Constructors
	Transform();
	Transform(const Vector3 inPosition);
	Transform(const Vector3& inPosition, const Quaternion& inRotation, const Vector3& inScale);
	Transform(Matrix inWorldMatrix);

	// Component Process
	virtual ~Transform() override;
	virtual void Render(double dt) override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

	// Clone
	virtual std::shared_ptr<Component> Clone() const override;
	//Transform* CloneWithHierarchy(Scene* scene, const Transform* parent = nullptr) const;

	// Public Methods
	// Getters & Setters
	// Local
	Vector3 GetLocalPosition() const { return m_localPosition; }
	Quaternion GetLocalRotation() const { return m_localRotation; }
	Vector3 GetLocalScale() const { return m_localScale; }
	Matrix GetLocalMatrix() const { return m_localMatrix; }

	void SetLocalPosition(const Vector3& inLocalPosition);
	void SetLocalRotation(const Vector3& inLocalRotation);
	void SetLocalRotation(const Quaternion& inLocalRotation);
	void SetLocalScale(const Vector3& inLocalScale);

	void AddPosition(const Vector3& inLocalPosition);
	void AddScale(const Vector3& inLocalScale);

	void AddYAxisRotation(float radian);
	void AddZAxisRotation(float radian);
	void AddXAxisRotation(float radian);

	// World
	Vector3 GetWorldPosition() const { return m_worldPosition; }
	Quaternion GetWorldRotation() const { return m_worldRotation; }
	Vector3 GetWorldScale() const { return m_worldScale; }
	Matrix GetWorldMatrix() const { return m_worldMatrix; }

	void SetWorldPosition(const Vector3& inWorldPosition);
	void SetWorldRotation(const Vector3& inWorldRotation);
	void SetWorldRotation(const Quaternion& inWorldRotation);
	void SetWorldScale(const Vector3& inWorldScale);
	void SetWorldMatrix(const Matrix& matrix);

public:
	// Parent - Child 관계
	std::weak_ptr<Transform> GetParent() const;
	std::weak_ptr<Transform> GetChild(int index) const;
	const std::vector<std::shared_ptr<Transform>>& GetChildren() const;

	void EditorLoadSetParent(std::weak_ptr<Transform> inParent);
	void SetParent(std::weak_ptr<Transform> inParent);
	void SetChild(std::weak_ptr<Transform> inChild);

	bool HasParent() { return m_parent.lock() != nullptr; }
	bool HasChild(std::shared_ptr<Transform> child);

	bool IsChildOf(const std::weak_ptr<Transform> inParent) const;
	void RemoveChild(std::shared_ptr<Transform> inChild);

	bool InChildCheckAncestor(std::weak_ptr<Transform> parent, std::weak_ptr<Transform> checkChild);

	// 업데이트 중복 방지
	void SetIsUpdated(bool value) { m_isUpdated = value; }

	// 프리팹 전용
	// 프리팹 디시리얼라이즈 할 때, 트랜스폼을 먼저 계산하고서 계층구조를 마지막에 이루는데
	// 이 때, 기존 트랜스폼 값에 변동이 생기므로 이 함수에서는 진짜 parent child 세팅만 해준다.
	void SetPrefabParent(std::weak_ptr<Transform> inParent);

private:
	void UpdateTransform();
	void InterpolateLocalTransform(const Vector3& targetPosition, const Quaternion& targetRotation, float factor);

private:
	// Local
	Vector3 m_localPosition;
	//Vector3 m_localRotation;
	Vector3 m_localScale;
	Matrix m_localMatrix;

	Quaternion m_localRotation;

	// World
	Vector3 m_worldPosition;
	// Vector3 m_worldRotation;
	Vector3 m_worldScale;
	Matrix m_worldMatrix;

	Quaternion m_worldRotation;

	// Hierarchy
	std::weak_ptr<Transform> m_parent;
	std::vector<std::shared_ptr<Transform>> m_children;
	bool m_isUpdated;
};