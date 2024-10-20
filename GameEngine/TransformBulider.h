#pragma once
#include "IBuilder.h"
#include "Component.h"
#include "Transform.h"

/// <summary>
/// 2024.01.08
/// Transform을 만들어낼 Builder
/// 작성 : 유승운
/// </summary>


class TransformBuilder : public IBuilder<Component>
{
public:
public:
	TransformBuilder(const Vector3 inPosition);
	TransformBuilder(const Matrix& inMatrix);
	TransformBuilder(const Vector3& inPosition, const Quaternion& inRotation, const Vector3& inScale);

	TransformBuilder();
	~TransformBuilder();

	virtual std::shared_ptr<Component> Create() override;

private:
	std::shared_ptr<Transform> m_returnTransform;


};