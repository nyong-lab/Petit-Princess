#include "GameEngine_pch.h"
#include "Transform.h"
#include "Util.h"
#include <cassert>
#include "QuaternionUtils.h"

#include <DirectXMath.h>
#include "Entity.h"

Transform::Transform()
	: m_localScale{ 1.f,1.f,1.f }
	//, m_localRotation{ 0.f, 0.f, 0.f }
	, m_localRotation{ 0.f, 0.f, 0.f, 1.f }
	, m_localPosition{ 0.f, 0.f, 0.f }
	, m_localMatrix{ Matrix::Identity }
	, m_worldScale{ 1.f, 1.f, 1.f }
	//, m_worldRotation{ 0.f, 0.f, 0.f }
	, m_worldRotation{ 0.f, 0.f, 0.f , 1.f }
	, m_worldPosition{ 0.f, 0.f, 0.f }
	, m_worldMatrix{ Matrix::Identity }
	, m_isUpdated(false)
{

}

Transform::Transform(Matrix inWorldMatrix)
	: m_localScale{ 1.f,1.f,1.f }
	//, m_localRotation{ 0.f, 0.f, 0.f }
	, m_localRotation{ 0.f, 0.f, 0.f, 1.f }
	, m_localPosition{ 0.f, 0.f, 0.f }
	, m_localMatrix{ Matrix::Identity }
	, m_worldScale{ 1.f, 1.f, 1.f }
	//, m_worldRotation{ 0.f, 0.f, 0.f }
	, m_worldRotation{ 0.f, 0.f, 0.f , 1.f }
	, m_worldPosition{ 0.f, 0.f, 0.f }
	, m_worldMatrix{ Matrix::Identity }
	, m_isUpdated(false)
{
	/*Quaternion quat;
	inWorldMatrix.Decompose(m_localScale, quat, m_localPosition);
	m_localRotation = QuaternionUtils::ToEulerAngles(quat);*/

	inWorldMatrix.Decompose(m_localScale, m_localRotation, m_localPosition);

	UpdateTransform();
	m_isUpdated = false;
}

Transform::Transform(const Vector3 inPosition)
	: m_worldPosition(inPosition)
	, m_isUpdated(false)
{

}

Transform::Transform(const Vector3& inPosition, const Quaternion& inRotation, const Vector3& inScale)
	: m_worldPosition(inPosition)
	, m_worldRotation(inRotation)
	, m_worldScale(inScale)
	, m_isUpdated(false)
{

}

Transform::~Transform()
{

}

void Transform::Render(double dt)
{

}

void Transform::Update(double dt)
{
	UpdateTransform();
}

void Transform::UpdateTransform()
{
	if (!m_isUpdated)
	{
		m_isUpdated = true;

		// ���� ����� �����
		Matrix localScale = Matrix::CreateScale(m_localScale.x, m_localScale.y, m_localScale.z);

		// ���� ���Ϸ���
	/*	Quaternion q = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);
		Matrix localRotation = Matrix::CreateFromQuaternion(q);*/

		// ���ʹϾ����� ��ü
		Matrix localRotation = Matrix::CreateFromQuaternion(m_localRotation);

		Matrix localTranslate = Matrix::CreateTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);

		m_localMatrix = localScale * localRotation * localTranslate;

		// �θ� ������ ���ÿ� �θ��� ���� ����� �����ְ�
		if (m_parent.lock())
		{
			m_worldMatrix = m_localMatrix * m_parent.lock()->GetWorldMatrix();
		}
		// �θ� ������ ���� ����� ��������̴�
		else
		{
			m_worldMatrix = m_localMatrix;
		}

		//// ���� ��Ŀ��� ���� ���� �� �� �־��ֱ�
		//Quaternion quat;
		//m_worldMatrix.Decompose(m_worldScale, quat, m_worldPosition);
		//m_worldRotation = QuaternionUtils::ToEulerAngles(quat);

		// ���� ��Ŀ��� ���� ���� �� �� �־��ֱ�
		m_worldMatrix.Decompose(m_worldScale, m_worldRotation, m_worldPosition);

		// �ڽ� ������Ʈ
		for (const auto& child : m_children)
		{
			child->UpdateTransform();
		}
	}
}

void Transform::InterpolateLocalTransform(const Vector3& targetPosition, const Quaternion& targetRotation, float factor)
{
	// ������ ���� ��ȯ�� ��ǥ�� �ϴ� ���� ��ȯ�� �����Ͽ� �ε巴�� ������Ʈ
	m_localPosition = Vector3::Lerp(m_localPosition, targetPosition, factor);
	m_localRotation = Quaternion::Slerp(m_localRotation, targetRotation, factor);
}

void Transform::Finalize()
{
	// �θ� �ִٸ� �θ𿡰Լ� ���� �����.
	if (m_parent.lock())
	{
		m_parent.lock()->RemoveChild(shared_from_this());
		m_parent.reset();
	}

	// �ڽĵ��� ����鼭 �ڽ��� �θ� ������ ���´�.
	for (auto& child : m_children)
	{
		RemoveChild(child);
	}
}

void Transform::Serialize(nlohmann::json& json)
{
	json["m_localPosition"] =
	{
		{"x", m_localPosition.x},
		{"y", m_localPosition.y},
		{"z", m_localPosition.z}
	};

	json["m_localRotation"] =
	{
		{"x", m_localRotation.x},
		{"y", m_localRotation.y},
		{"z", m_localRotation.z},
		{"w", m_localRotation.w}
	};

	json["m_localScale"] =
	{
		{"x", m_localScale.x},
		{"y", m_localScale.y},
		{"z", m_localScale.z}
	};
}

void Transform::Deserialize(const nlohmann::json& json)
{
	if (json.contains("m_localPosition"))
	{
		m_localPosition.x = json["m_localPosition"]["x"];
		m_localPosition.y = json["m_localPosition"]["y"];
		m_localPosition.z = json["m_localPosition"]["z"];
	}

	if (json.contains("m_localRotation"))
	{
		m_localRotation.x = json["m_localRotation"]["x"];
		m_localRotation.y = json["m_localRotation"]["y"];
		m_localRotation.z = json["m_localRotation"]["z"];
		m_localRotation.w = json["m_localRotation"]["w"];
	}

	if (json.contains("m_localScale"))
	{
		m_localScale.x = json["m_localScale"]["x"];
		m_localScale.y = json["m_localScale"]["y"];
		m_localScale.z = json["m_localScale"]["z"];
	}

	UpdateTransform();
}

// Entity�� ��������ڿ��� ȣ��� Clone
// �� Clone �Լ��� ���������� clone ��Ű�� �ʴ´�.
std::shared_ptr<Component> Transform::Clone() const
{
	std::shared_ptr<Transform> clone = std::make_shared<Transform>();

	clone->m_localPosition = m_localPosition;
	clone->m_localRotation = m_localRotation;
	clone->m_localScale = m_localScale;
	clone->m_localMatrix = m_localMatrix;

	clone->m_worldPosition = m_worldPosition;
	clone->m_worldRotation = m_worldRotation;
	clone->m_worldScale = m_worldScale;
	clone->m_worldMatrix = m_worldMatrix;

	clone->m_parent.reset();
	clone->m_children = {};

	return clone;
}

void Transform::SetLocalPosition(const Vector3& inLocalPosition)
{
	m_localPosition = inLocalPosition;
	UpdateTransform();
	m_isUpdated = false;
}

//void Transform::SetLocalRotation(const Vector3& inLocalRotation)
//{
//	m_localRotation = inLocalRotation;
//	UpdateTransform();
//	m_isUpdated = false;
//}

void Transform::SetLocalRotation(const Quaternion& inLocalRotation)
{
	m_localRotation = inLocalRotation;
	UpdateTransform();
	m_isUpdated = false;
}

/// ��׸� �ֱ� 
void Transform::SetLocalRotation(const Vector3& inLocalRotation)
{
	// ��׸����� �������� ��ȯ
	Vector3 radiansRotation;
	radiansRotation.x = DirectX::XMConvertToRadians(inLocalRotation.x);
	radiansRotation.y = DirectX::XMConvertToRadians(inLocalRotation.y);
	radiansRotation.z = DirectX::XMConvertToRadians(inLocalRotation.z);

	m_localRotation = Quaternion::CreateFromYawPitchRoll(radiansRotation.y, radiansRotation.x, radiansRotation.z);

	UpdateTransform();
	m_isUpdated = false;
}

void Transform::SetLocalScale(const Vector3& inLocalScale)
{
	m_localScale = inLocalScale;
	UpdateTransform();
	m_isUpdated = false;
}

void Transform::AddPosition(const Vector3& inLocalPosition)
{
	m_localPosition += inLocalPosition;
	UpdateTransform();
	m_isUpdated = false;
}

void Transform::AddScale(const Vector3& inLocalScale)
{
	m_localScale += inLocalScale;
	UpdateTransform();
	m_isUpdated = false;
}

void Transform::AddYAxisRotation(float radian)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::UnitY, radian);
	m_localRotation *= q;
	UpdateTransform();
	m_isUpdated = false;
}

void Transform::AddZAxisRotation(float radian)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::UnitZ, radian);
	m_localRotation *= q;
	UpdateTransform();
	m_isUpdated = false;
}

void Transform::AddXAxisRotation(float radian)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::UnitX, radian);
	m_localRotation *= q;
	UpdateTransform();
	m_isUpdated = false;
}

std::weak_ptr<Transform> Transform::GetChild(int index) const
{
	if (index < 0 || index >= m_children.size())
	{
		assert(index >= 0 && index < m_children.size() && "Transform* Transform::GetChild(int index) const / if(index < 0 || index >= children.size())");
		return std::weak_ptr<Transform> {};
	}
	return m_children[index];
}

const std::vector<std::shared_ptr<Transform>>& Transform::GetChildren() const
{
	return m_children;
}


// ������ �ε� ���� setparent �Լ�.
// �� ���̺� �� �� ���������� �ִ� ���¶��
// �θ� �ִ� �������� ���� Ʈ�������� �����ϴµ�
// �� �ε� �� �� setparent �� �ϸ鼭 �θ��� ������� �������� ������ �־���.
void Transform::EditorLoadSetParent(std::weak_ptr<Transform> inParent)
{
	// # �θ� ���� ��� : �θ� �߰��ϰ� �θ��� �ڽ����� �߰�
	if (this->m_parent.lock() == nullptr)
	{
		this->m_parent = inParent;

		if (inParent.lock())
		{
			inParent.lock()->m_children.push_back(shared_from_this());
		}
	}
}

void Transform::SetWorldPosition(const Vector3& inWorldPosition)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = m_parent.lock()->GetWorldMatrix().Invert();

		Vector3 localPosition;
		localPosition = localPosition.Transform(inWorldPosition, worldToParentLocalMatrix);

		SetLocalPosition(localPosition);
	}
	else
	{
		SetLocalPosition(inWorldPosition);
	}
}

/// ��׸� �ֱ�
void Transform::SetWorldRotation(const Vector3& inWorldRotation)
{
	if (HasParent())
	{
		// ��׸����� �������� ��ȯ
		Vector3 radiansRotation;
		radiansRotation.x = DirectX::XMConvertToRadians(inWorldRotation.x);
		radiansRotation.y = DirectX::XMConvertToRadians(inWorldRotation.y);
		radiansRotation.z = DirectX::XMConvertToRadians(inWorldRotation.z);

		Quaternion q = Quaternion::CreateFromYawPitchRoll(radiansRotation.y, radiansRotation.x, radiansRotation.z);

		Matrix inverseMatrix = m_parent.lock()->GetWorldMatrix().Invert();

		// �ڽ� ���� ���ʹϾ� * �θ� ���� ������� ���ʹϾ� 
		Quaternion localRotation = Quaternion::CreateFromRotationMatrix(inverseMatrix) * q;

		SetLocalRotation(localRotation);
	}
	else
	{
		SetLocalRotation(inWorldRotation);
	}
}

void Transform::SetWorldRotation(const Quaternion& inWorldRotation)
{
	if (HasParent())
	{
		Matrix inverseMatrix = m_parent.lock()->GetWorldMatrix().Invert();

		// �ڽ� ���� ���ʹϾ� * �θ� ���� ������� ���ʹϾ� 
		Quaternion localRotation = Quaternion::CreateFromRotationMatrix(inverseMatrix) * inWorldRotation;

		SetLocalRotation(localRotation);
	}
	else
	{
		SetLocalRotation(inWorldRotation);
	}
}

void Transform::SetWorldScale(const Vector3& inWorldScale)
{
	if (HasParent())
	{
		Vector3 parentScale = m_parent.lock()->GetWorldScale();
		Vector3 scale = inWorldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;

		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(inWorldScale);
	}
}

void Transform::SetWorldMatrix(const Matrix& matrix)
{
	m_worldMatrix = matrix;
}

std::weak_ptr<Transform> Transform::GetParent() const
{
	return m_parent;
}


void Transform::SetParent(std::weak_ptr<Transform> inParent)
{
	// # �θ� ���� ��� : �θ� �߰��ϰ� �θ��� �ڽ����� �߰�
	if (this->m_parent.lock() == nullptr)
	{
		this->m_parent = inParent;

		if (inParent.lock())
		{
			inParent.lock()->m_children.push_back(shared_from_this());

			// ���ο� �θ��� ���� ������� ����Ͽ� ���� ��ǥ ����
			Matrix worldToParentLocalMatrix = inParent.lock()->GetWorldMatrix().Invert();

			//m_localRotation = m_localRotation.TransformNormal(m_localRotation, worldToParentLocalMatrix);

			////���Ϸ� ������ ���ʹϾ����� ��ȯ
			//Quaternion localRotationQuat = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			//// �θ��� ���� ����ķ� ��ȯ�� ���ʹϾ��� ����
			//Quaternion parentRotationQuat = Quaternion::CreateFromRotationMatrix(DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationMatrix(worldToParentLocalMatrix)));

			//// ���� ȸ���� �θ��� ���� ����ķ� ��ȯ
			//localRotationQuat = XMQuaternionMultiply(localRotationQuat, parentRotationQuat);

			//// ȸ�� ��ȯ�� ���ʹϾ��� �ٽ� ���Ϸ� ������ ��ȯ
			//m_localRotation = QuaternionUtils::ToEulerAngles(localRotationQuat);

			// �ڽ� ��ü�� ���� ���ʹϾ�
			//Quaternion localRotation = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			// �ڽ� ���� ���ʹϾ� * �θ� ���� ������� ���ʹϾ� 
			//Quaternion transformedRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * localRotation;

			//m_localRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * m_localRotation;

			//// ��ȯ�� ���ʹϾ��� ���Ϸ� ������ ��ȯ
			//m_localRotation = QuaternionUtils::ToEulerAngles(transformedRotation);

			// ������ ����
			m_localPosition = m_localPosition.Transform(m_localPosition, worldToParentLocalMatrix);

			// �����̼� ����
			m_localRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * m_localRotation;

			// ������ ����
			Vector3 parentScale = inParent.lock()->GetWorldScale();
			m_localScale.x /= parentScale.x;
			m_localScale.y /= parentScale.y;
			m_localScale.z /= parentScale.z;

			UpdateTransform();
			m_isUpdated = false;
		}
	}

	// # �θ� ���� ��� : ���� �θ��� �ڽ� ��Ͽ��� �����ϰ� �θ� �߰��ϰ� �θ��� �ڽ����� �߰�
	else if (this->m_parent.lock() != nullptr)
	{
		// ���� �θ��� �ڽ� ��Ͽ��� ����
		for (auto iter = this->m_parent.lock()->m_children.begin(); iter != this->m_parent.lock()->m_children.end(); ++iter)
		{
			if ((*iter).get() == this)
			{
				this->m_parent.lock()->m_children.erase(iter);

				// �θ𿬰��� ���������ν� ���� ��ǥ���� ���� �� ������ǥ���� ���̴�.
				m_localPosition = m_localPosition.Transform(m_localPosition, m_worldMatrix);

				//m_localRotation = m_localRotation.TransformNormal(m_localRotation, m_worldMatrix);

				// // ���Ϸ� ������ ���ʹϾ����� ��ȯ
				//Quaternion localRotationQuat = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

				//// �θ��� ���� ����ķ� ��ȯ�� ���ʹϾ��� ����
				//Quaternion parentRotationQuat = Quaternion::CreateFromRotationMatrix(DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationMatrix(m_worldMatrix)));

				//// ���� ȸ���� �θ��� ���� ����ķ� ��ȯ
				//localRotationQuat = XMQuaternionMultiply(localRotationQuat, parentRotationQuat);

				//// ȸ�� ��ȯ�� ���ʹϾ��� �ٽ� ���Ϸ� ������ ��ȯ
				//m_localRotation = QuaternionUtils::ToEulerAngles(localRotationQuat);

				// �ڽ� ��ü�� ���� ���ʹϾ�
				//Quaternion localRotation = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

				// �ڽ� ���� ���ʹϾ� * �θ� ���� ������� ���ʹϾ� 
				//Quaternion transformedRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * localRotation;
				m_localRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * m_localRotation;

				// ��ȯ�� ���ʹϾ��� ���Ϸ� ������ ��ȯ
				//m_localRotation = QuaternionUtils::ToEulerAngles(transformedRotation);

				// Scale�� ���, �θ��� ���� �������� ����Ͽ� ����
				m_localScale.x *= m_parent.lock()->m_worldScale.x;
				m_localScale.y *= m_parent.lock()->m_worldScale.y;
				m_localScale.z *= m_parent.lock()->m_worldScale.z;

				UpdateTransform();
				m_isUpdated = false;
				break;
			}
		}

		// �θ� �߰��ϰ� �θ��� �ڽ����� �߰�
		this->m_parent = inParent;

		if (inParent.lock())
		{
			inParent.lock()->m_children.push_back(shared_from_this());

			// ���ο� �θ��� ���� ������� ����Ͽ� ���� ��ǥ ����
			Matrix worldToParentLocalMatrix = inParent.lock()->GetWorldMatrix().Invert();

			//m_localRotation = m_localRotation.TransformNormal(m_localRotation, worldToParentLocalMatrix);

			//// ���Ϸ� ������ ���ʹϾ����� ��ȯ
			//Quaternion localRotationQuat = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			//// �θ��� ���� ����ķ� ��ȯ�� ���ʹϾ��� ����
			//Quaternion parentRotationQuat = Quaternion::CreateFromRotationMatrix(DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationMatrix(worldToParentLocalMatrix)));

			//// ���� ȸ���� �θ��� ���� ����ķ� ��ȯ
			//localRotationQuat = XMQuaternionMultiply(localRotationQuat, parentRotationQuat);

			//// ȸ�� ��ȯ�� ���ʹϾ��� �ٽ� ���Ϸ� ������ ��ȯ
			//m_localRotation = QuaternionUtils::ToEulerAngles(localRotationQuat);

			// �ڽ� ��ü�� ���� ���ʹϾ�
			//Quaternion localRotation = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			// �ڽ� ���� ���ʹϾ� * �θ� ���� ������� ���ʹϾ� 
			//Quaternion transformedRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * localRotation;

			// ��ȯ�� ���ʹϾ��� ���Ϸ� ������ ��ȯ
			//m_localRotation = QuaternionUtils::ToEulerAngles(transformedRotation);

			m_localPosition = m_localPosition.Transform(m_localPosition, worldToParentLocalMatrix);

			m_localRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * m_localRotation;

			// Scale�� ���, �θ��� ���� �������� ����Ͽ� ����
			Vector3 parentScale = inParent.lock()->GetWorldScale();
			m_localScale.x /= parentScale.x;
			m_localScale.y /= parentScale.y;
			m_localScale.z /= parentScale.z;

			//UpdateTransform();
			//m_isUpdated = false;
		}
	}
}

void Transform::SetChild(std::weak_ptr<Transform> inChild)
{
	// �ڽ��� �� �ڽ��̰ų� �̹� �� �ڽ��̶�� ���� (�������� ��ȭ x)
	if (inChild.lock() == shared_from_this() || HasChild(inChild.lock()))
	{
		return;
	}

	// �ڽ����� ���� �ְ� �� �θ��� �θ��� �θ��� �θ� �� Ÿ�� �ö����� ����԰��̶�� 
	// return ��Ų��. ��������� üũ.
	if (m_parent.lock() != nullptr)
	{
		if (InChildCheckAncestor(m_parent, inChild))
		{
			return;
		}
	}


	// �ڽ��� �θ� �־��ٸ� �� �θ𿡰Լ� �ڽ��� ������Ű�� �θ� ������ ���´�.
	std::weak_ptr<Transform> prevParent = inChild.lock()->GetParent();
	if (prevParent.lock() != nullptr)
	{
		prevParent.lock()->RemoveChild(inChild.lock());
	}

	// ���� �θ�� �������ش�.
	inChild.lock()->SetParent(shared_from_this());

	// �ڽ��� �θ� �Ǿ����� �˰��� ���� �ڽ��� �ڽ��̾��ٸ�
	// ���������󿡼� �Ѵ� ������ �θ��� ���̹Ƿ� 
	// �ڽ��� �ڽ��� ���� �����Ѵ�.
	if (m_parent.lock() == inChild.lock())
	{
		inChild.lock()->RemoveChild(shared_from_this());

		return;
	}
}

// �Ű������� ���� child�� children �����̳ʿ��� ã�Ƴ��� �����.
// �Ŀ� child�� parent ���ᵵ �����.
void Transform::RemoveChild(std::shared_ptr<Transform> inChild)
{
	// �ڽ� ���� ����
	auto iter = std::remove(m_children.begin(), m_children.end(), inChild);
	m_children.erase(iter, m_children.end());

	// �θ� ���� ����
	inChild->SetParent(std::weak_ptr<Transform>{ });

	// �θ𿬰��� ���������ν� ���� ��ǥ���� ���� �� ������ǥ���� ���̴�.
	inChild->m_localPosition = inChild->m_localPosition.Transform(inChild->m_localPosition, m_worldMatrix);

	//inChild->m_localRotation = inChild->m_localRotation.TransformNormal(inChild->m_localRotation, m_worldMatrix);

	inChild->m_localRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * inChild->m_localRotation;

	// Scale�� ���, �θ��� ���� �������� ����Ͽ� ����
	inChild->m_localScale.x *= m_worldScale.x;
	inChild->m_localScale.y *= m_worldScale.y;
	inChild->m_localScale.z *= m_worldScale.z;

	inChild->UpdateTransform();
	inChild->m_isUpdated = false;
}

bool Transform::InChildCheckAncestor(std::weak_ptr<Transform> parent, std::weak_ptr<Transform> checkChild)
{
	if (parent.lock()->m_parent.lock() != nullptr)
	{
		if (parent.lock()->m_parent.lock() == checkChild.lock())
			return true;

		return InChildCheckAncestor(parent.lock()->m_parent, checkChild);
	}

	return false;
}

void Transform::SetPrefabParent(std::weak_ptr<Transform> inParent)
{
	this->m_parent = inParent;

	inParent.lock()->m_children.push_back(shared_from_this());
}

bool Transform::HasChild(std::shared_ptr<Transform> child)
{
	// �̹� �� �ڽ����� Ȯ��
	for (const auto& existingChild : m_children)
	{
		if (existingChild == child)
		{
			return true;
		}
	}

	return false;
}

bool Transform::IsChildOf(const std::weak_ptr<Transform> inParent) const
{
	// �θ� ã�Ƽ� �θ� �ִ��� Ȯ��
	std::weak_ptr<Transform> thisTransform = GetParent();

	// while�� ���� �ö󰡸鼭 �θ� �ִ��� Ȯ��
	while (thisTransform.lock() != nullptr)
	{
		// # ���� �θ� ���� ���
		if (thisTransform.lock()->GetParent().lock() == nullptr)
		{
			return false;
		}

		// # ���� �θ� inParent�� ���� ���
		else if (thisTransform.lock() == inParent.lock())
		{
			return true;
		}

		thisTransform = thisTransform.lock()->m_parent;
	}

	return false;
}

/// TODO::���� - Clone ��ư (�ڽ� �������� ���� Clone �ǰ� �����)
//// ������������ Clone ��Ű�� �Լ�
//// ��������� ȣ���Ͽ� �ڽ� ���������� clone �Ѵ�.
//Transform* Transform::CloneWithHierarchy(Scene* scene, const Transform* parent /*= nullptr*/) const
//{
//	Transform* cloneTransform = new Transform();
//
//	cloneTransform->m_position = m_position;
//	cloneTransform->m_rotation = m_rotation;
//	cloneTransform->m_scale = m_scale;
//
//	// ���������� ����Ƽó�� �ڽĵ鸸 clone �Ѵ�. (�θ� x)
//	if (parent)
//	{
//		cloneTransform->m_parent = const_cast<Transform*>(parent)->m_parent;
//	}
//	else
//	{
//		cloneTransform->m_parent = nullptr;
//	}
//
//	for (auto& child : m_children)
//	{
//		if (child)
//		{
//			Entity* childCloneEntity = child->GetOwner()->Clone(scene, cloneTransform);
//
//			cloneTransform->SetChild(childCloneEntity->GetComponent<Transform>());
//			scene->AddEntity(childCloneEntity);
//		}
//	}
//
//	return cloneTransform;
//}