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

		// 로컬 행렬을 만든다
		Matrix localScale = Matrix::CreateScale(m_localScale.x, m_localScale.y, m_localScale.z);

		// 기존 오일러값
	/*	Quaternion q = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);
		Matrix localRotation = Matrix::CreateFromQuaternion(q);*/

		// 쿼터니언으로 교체
		Matrix localRotation = Matrix::CreateFromQuaternion(m_localRotation);

		Matrix localTranslate = Matrix::CreateTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);

		m_localMatrix = localScale * localRotation * localTranslate;

		// 부모가 있으면 로컬에 부모의 월드 행렬을 곱해주고
		if (m_parent.lock())
		{
			m_worldMatrix = m_localMatrix * m_parent.lock()->GetWorldMatrix();
		}
		// 부모가 없으면 로컬 행렬이 월드행렬이다
		else
		{
			m_worldMatrix = m_localMatrix;
		}

		//// 월드 행렬에서 성분 분해 후 값 넣어주기
		//Quaternion quat;
		//m_worldMatrix.Decompose(m_worldScale, quat, m_worldPosition);
		//m_worldRotation = QuaternionUtils::ToEulerAngles(quat);

		// 월드 행렬에서 성분 분해 후 값 넣어주기
		m_worldMatrix.Decompose(m_worldScale, m_worldRotation, m_worldPosition);

		// 자식 업데이트
		for (const auto& child : m_children)
		{
			child->UpdateTransform();
		}
	}
}

void Transform::InterpolateLocalTransform(const Vector3& targetPosition, const Quaternion& targetRotation, float factor)
{
	// 현재의 로컬 변환과 목표로 하는 로컬 변환을 보간하여 부드럽게 업데이트
	m_localPosition = Vector3::Lerp(m_localPosition, targetPosition, factor);
	m_localRotation = Quaternion::Slerp(m_localRotation, targetRotation, factor);
}

void Transform::Finalize()
{
	// 부모가 있다면 부모에게서 나를 끊어낸다.
	if (m_parent.lock())
	{
		m_parent.lock()->RemoveChild(shared_from_this());
		m_parent.reset();
	}

	// 자식들을 끊어내면서 자식의 부모도 연결을 끊는다.
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

// Entity의 복사생성자에서 호출된 Clone
// 이 Clone 함수는 계층구조는 clone 시키지 않는다.
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

/// 디그리 넣기 
void Transform::SetLocalRotation(const Vector3& inLocalRotation)
{
	// 디그리에서 라디안으로 변환
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


// 에디터 로드 전용 setparent 함수.
// 씬 세이브 할 때 계층구조가 있는 상태라면
// 부모가 있는 기준으로 로컬 트랜스폼을 저장하는데
// 씬 로드 할 때 setparent 를 하면서 부모의 역행렬을 곱해져서 문제가 있었다.
void Transform::EditorLoadSetParent(std::weak_ptr<Transform> inParent)
{
	// # 부모가 없을 경우 : 부모를 추가하고 부모의 자식으로 추가
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

/// 디그리 넣기
void Transform::SetWorldRotation(const Vector3& inWorldRotation)
{
	if (HasParent())
	{
		// 디그리에서 라디안으로 변환
		Vector3 radiansRotation;
		radiansRotation.x = DirectX::XMConvertToRadians(inWorldRotation.x);
		radiansRotation.y = DirectX::XMConvertToRadians(inWorldRotation.y);
		radiansRotation.z = DirectX::XMConvertToRadians(inWorldRotation.z);

		Quaternion q = Quaternion::CreateFromYawPitchRoll(radiansRotation.y, radiansRotation.x, radiansRotation.z);

		Matrix inverseMatrix = m_parent.lock()->GetWorldMatrix().Invert();

		// 자식 로컬 쿼터니언 * 부모 월드 역행렬의 쿼터니언 
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

		// 자식 로컬 쿼터니언 * 부모 월드 역행렬의 쿼터니언 
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
	// # 부모가 없을 경우 : 부모를 추가하고 부모의 자식으로 추가
	if (this->m_parent.lock() == nullptr)
	{
		this->m_parent = inParent;

		if (inParent.lock())
		{
			inParent.lock()->m_children.push_back(shared_from_this());

			// 새로운 부모의 월드 역행렬을 사용하여 로컬 좌표 조정
			Matrix worldToParentLocalMatrix = inParent.lock()->GetWorldMatrix().Invert();

			//m_localRotation = m_localRotation.TransformNormal(m_localRotation, worldToParentLocalMatrix);

			////오일러 각도를 쿼터니언으로 변환
			//Quaternion localRotationQuat = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			//// 부모의 월드 역행렬로 변환된 쿼터니언을 얻음
			//Quaternion parentRotationQuat = Quaternion::CreateFromRotationMatrix(DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationMatrix(worldToParentLocalMatrix)));

			//// 로컬 회전을 부모의 로컬 역행렬로 변환
			//localRotationQuat = XMQuaternionMultiply(localRotationQuat, parentRotationQuat);

			//// 회전 변환한 쿼터니언값을 다시 오일러 값으로 변환
			//m_localRotation = QuaternionUtils::ToEulerAngles(localRotationQuat);

			// 자식 객체의 로컬 쿼터니언
			//Quaternion localRotation = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			// 자식 로컬 쿼터니언 * 부모 월드 역행렬의 쿼터니언 
			//Quaternion transformedRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * localRotation;

			//m_localRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * m_localRotation;

			//// 변환된 쿼터니언을 오일러 각도로 변환
			//m_localRotation = QuaternionUtils::ToEulerAngles(transformedRotation);

			// 포지션 적용
			m_localPosition = m_localPosition.Transform(m_localPosition, worldToParentLocalMatrix);

			// 로테이션 적용
			m_localRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * m_localRotation;

			// 스케일 적용
			Vector3 parentScale = inParent.lock()->GetWorldScale();
			m_localScale.x /= parentScale.x;
			m_localScale.y /= parentScale.y;
			m_localScale.z /= parentScale.z;

			UpdateTransform();
			m_isUpdated = false;
		}
	}

	// # 부모가 있을 경우 : 기존 부모의 자식 목록에서 제거하고 부모를 추가하고 부모의 자식으로 추가
	else if (this->m_parent.lock() != nullptr)
	{
		// 기존 부모의 자식 목록에서 제거
		for (auto iter = this->m_parent.lock()->m_children.begin(); iter != this->m_parent.lock()->m_children.end(); ++iter)
		{
			if ((*iter).get() == this)
			{
				this->m_parent.lock()->m_children.erase(iter);

				// 부모연결이 끊어짐으로써 로컬 좌표계의 값은 즉 월드좌표계의 값이다.
				m_localPosition = m_localPosition.Transform(m_localPosition, m_worldMatrix);

				//m_localRotation = m_localRotation.TransformNormal(m_localRotation, m_worldMatrix);

				// // 오일러 각도를 쿼터니언으로 변환
				//Quaternion localRotationQuat = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

				//// 부모의 로컬 역행렬로 변환된 쿼터니언을 얻음
				//Quaternion parentRotationQuat = Quaternion::CreateFromRotationMatrix(DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationMatrix(m_worldMatrix)));

				//// 로컬 회전을 부모의 로컬 역행렬로 변환
				//localRotationQuat = XMQuaternionMultiply(localRotationQuat, parentRotationQuat);

				//// 회전 변환한 쿼터니언값을 다시 오일러 값으로 변환
				//m_localRotation = QuaternionUtils::ToEulerAngles(localRotationQuat);

				// 자식 객체의 로컬 쿼터니언
				//Quaternion localRotation = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

				// 자식 로컬 쿼터니언 * 부모 월드 역행렬의 쿼터니언 
				//Quaternion transformedRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * localRotation;
				m_localRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * m_localRotation;

				// 변환된 쿼터니언을 오일러 각도로 변환
				//m_localRotation = QuaternionUtils::ToEulerAngles(transformedRotation);

				// Scale의 경우, 부모의 월드 스케일을 사용하여 조정
				m_localScale.x *= m_parent.lock()->m_worldScale.x;
				m_localScale.y *= m_parent.lock()->m_worldScale.y;
				m_localScale.z *= m_parent.lock()->m_worldScale.z;

				UpdateTransform();
				m_isUpdated = false;
				break;
			}
		}

		// 부모를 추가하고 부모의 자식으로 추가
		this->m_parent = inParent;

		if (inParent.lock())
		{
			inParent.lock()->m_children.push_back(shared_from_this());

			// 새로운 부모의 월드 역행렬을 사용하여 로컬 좌표 조정
			Matrix worldToParentLocalMatrix = inParent.lock()->GetWorldMatrix().Invert();

			//m_localRotation = m_localRotation.TransformNormal(m_localRotation, worldToParentLocalMatrix);

			//// 오일러 각도를 쿼터니언으로 변환
			//Quaternion localRotationQuat = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			//// 부모의 로컬 역행렬로 변환된 쿼터니언을 얻음
			//Quaternion parentRotationQuat = Quaternion::CreateFromRotationMatrix(DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationMatrix(worldToParentLocalMatrix)));

			//// 로컬 회전을 부모의 로컬 역행렬로 변환
			//localRotationQuat = XMQuaternionMultiply(localRotationQuat, parentRotationQuat);

			//// 회전 변환한 쿼터니언값을 다시 오일러 값으로 변환
			//m_localRotation = QuaternionUtils::ToEulerAngles(localRotationQuat);

			// 자식 객체의 로컬 쿼터니언
			//Quaternion localRotation = Quaternion::CreateFromYawPitchRoll(m_localRotation.y, m_localRotation.x, m_localRotation.z);

			// 자식 로컬 쿼터니언 * 부모 월드 역행렬의 쿼터니언 
			//Quaternion transformedRotation = Quaternion::CreateFromRotationMatrix(worldToParentLocalMatrix) * localRotation;

			// 변환된 쿼터니언을 오일러 각도로 변환
			//m_localRotation = QuaternionUtils::ToEulerAngles(transformedRotation);

			m_localPosition = m_localPosition.Transform(m_localPosition, worldToParentLocalMatrix);

			m_localRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * m_localRotation;

			// Scale의 경우, 부모의 월드 스케일을 사용하여 조정
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
	// 자식이 나 자신이거나 이미 내 자식이라면 리턴 (계층구조 변화 x)
	if (inChild.lock() == shared_from_this() || HasChild(inChild.lock()))
	{
		return;
	}

	// 자식으로 들어올 애가 내 부모의 부모의 부모의 부모를 쭉 타고 올라갔을때 조상님격이라면 
	// return 시킨다. 재귀적으로 체크.
	if (m_parent.lock() != nullptr)
	{
		if (InChildCheckAncestor(m_parent, inChild))
		{
			return;
		}
	}


	// 자식의 부모가 있었다면 그 부모에게서 자식을 삭제시키고 부모 연결을 끊는다.
	std::weak_ptr<Transform> prevParent = inChild.lock()->GetParent();
	if (prevParent.lock() != nullptr)
	{
		prevParent.lock()->RemoveChild(inChild.lock());
	}

	// 나를 부모로 지정해준다.
	inChild.lock()->SetParent(shared_from_this());

	// 자식의 부모가 되었지만 알고보니 내가 자식의 자식이었다면
	// 계층구조상에서 둘다 서로의 부모라는 뜻이므로 
	// 자식의 자식인 나를 제거한다.
	if (m_parent.lock() == inChild.lock())
	{
		inChild.lock()->RemoveChild(shared_from_this());

		return;
	}
}

// 매개변수로 들어온 child를 children 컨테이너에서 찾아내서 지운다.
// 후에 child의 parent 연결도 끊어낸다.
void Transform::RemoveChild(std::shared_ptr<Transform> inChild)
{
	// 자식 연결 끊가
	auto iter = std::remove(m_children.begin(), m_children.end(), inChild);
	m_children.erase(iter, m_children.end());

	// 부모 연결 끊기
	inChild->SetParent(std::weak_ptr<Transform>{ });

	// 부모연결이 끊어짐으로써 로컬 좌표계의 값은 즉 월드좌표계의 값이다.
	inChild->m_localPosition = inChild->m_localPosition.Transform(inChild->m_localPosition, m_worldMatrix);

	//inChild->m_localRotation = inChild->m_localRotation.TransformNormal(inChild->m_localRotation, m_worldMatrix);

	inChild->m_localRotation = Quaternion::CreateFromRotationMatrix(m_worldMatrix) * inChild->m_localRotation;

	// Scale의 경우, 부모의 월드 스케일을 사용하여 조정
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
	// 이미 내 자식인지 확인
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
	// 부모를 찾아서 부모가 있는지 확인
	std::weak_ptr<Transform> thisTransform = GetParent();

	// while로 위로 올라가면서 부모가 있는지 확인
	while (thisTransform.lock() != nullptr)
	{
		// # 위에 부모가 없을 경우
		if (thisTransform.lock()->GetParent().lock() == nullptr)
		{
			return false;
		}

		// # 위에 부모가 inParent와 같을 경우
		else if (thisTransform.lock() == inParent.lock())
		{
			return true;
		}

		thisTransform = thisTransform.lock()->m_parent;
	}

	return false;
}

/// TODO::동일 - Clone 버튼 (자식 계층구조 까지 Clone 되게 만들기)
//// 계층구조까지 Clone 시키는 함수
//// 재귀적으로 호출하여 자식 계층구조를 clone 한다.
//Transform* Transform::CloneWithHierarchy(Scene* scene, const Transform* parent /*= nullptr*/) const
//{
//	Transform* cloneTransform = new Transform();
//
//	cloneTransform->m_position = m_position;
//	cloneTransform->m_rotation = m_rotation;
//	cloneTransform->m_scale = m_scale;
//
//	// 계층구조는 유니티처럼 자식들만 clone 한다. (부모 x)
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