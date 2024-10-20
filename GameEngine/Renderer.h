#pragma once
#include "Component.h"
#include "CameraModule.h"
#include "IGraphics.h"
#include "MeshFilter.h"

class IGraphics;
class CameraModule;

/// <summary>
/// ���� : �赿�� -> �����ڿ� �Ŵ������� �� ������Ʈ���� �� �ε� �ϴ� �������� 
/// �⺻ �����ڷ� ������Ʈ�� ����� ������ �߰��� �׷��Ƚ��� ī�޶� �о�־��ִ� �Լ� ���� 
/// </summary>
class Renderer : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Renderer)

	// Serialize
	virtual void Serialize(nlohmann::json& json) abstract;
	virtual void Deserialize(const nlohmann::json& json) abstract;

public:
	Renderer();
	Renderer(const std::string& name);
	virtual ~Renderer();

public:
	virtual void Update(double dt) abstract override;
	virtual void Finalize() override;
	virtual void Render(double dt) abstract override;

	virtual std::shared_ptr<Component> Clone() const override;
};

