#pragma once
#include "Component.h"

#include <vector>


/// ��ü ũ��/��ġ/ȸ���� ����ϴ� Component
/// + Parent - Child ���踦 ���� ������� Transform�� ����
/// 
/// https://docs.unity3d.com/ScriptReference/Transform.html
/// 
/// 24.01.08 �̼��� ���¿� 
/// 
/// 24.01.19 [����] �赿��
/// - ����Ʈ ������ �۾� �� ������ �������� ����
/// 
/// 24.01.25 [����] �ڿ��� 
/// - SimpleMath�� ��ü
/// - local �� world ���� �߰� / UpdateTransform �Լ� �߰�
/// 
/// 24.01.28 [����] �ռ���
/// - �Լ��� Local, World ����
/// 
/// 24.01.28 [����] �赿��
/// - ����, �Լ� Local, World ����Ƽ ó�� �����ϵ��� ����
/// - !!  �������� ���� �� �ڽ� ���� �����̼� * �θ��� ���� ����� ���� ���� ���� ���� �ؾ� �� !! (������, �������� ����)
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
	// Parent - Child ����
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

	// ������Ʈ �ߺ� ����
	void SetIsUpdated(bool value) { m_isUpdated = value; }

	// ������ ����
	// ������ ��ø�������� �� ��, Ʈ�������� ���� ����ϰ� ���������� �������� �̷�µ�
	// �� ��, ���� Ʈ������ ���� ������ ����Ƿ� �� �Լ������� ��¥ parent child ���ø� ���ش�.
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