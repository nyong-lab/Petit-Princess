#include "GameEngine_pch.h"
#include "TransformBulider.h"
#include "Transform.h"

TransformBuilder::TransformBuilder()
{
	m_returnTransform = std::make_shared<Transform>();
}

TransformBuilder::TransformBuilder(const Vector3 inPosition)
{
	m_returnTransform = std::make_shared<Transform>(inPosition);
}

TransformBuilder::TransformBuilder(const Vector3& inPosition, const Quaternion& inRotation, const Vector3& inScale)
{
	m_returnTransform = std::make_shared<Transform>(inPosition, inRotation, inScale);
} 

TransformBuilder::TransformBuilder(const Matrix& inMatrix)
{
	m_returnTransform = std::make_shared<Transform>(inMatrix);
}

TransformBuilder::~TransformBuilder()
{

}

std::shared_ptr<Component> TransformBuilder::Create()
{
	//TODO:��������ڸ� ���� returnTransform�� �����ؼ� ���� ������ؾָ� ����������
	return std::make_shared<Transform>();

	//return new Transform(m_returnTransform);
	//return m_returnTransform;//TODO:��������ڸ� ���� returnTransform�� �����ؼ� ���� ������ؾָ� ����������
}
