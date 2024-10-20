#pragma once
#include "IComponent.h"

class Transform;
class CameraModule;

/// <summary>
/// IComponent�� �������̽��� ��ӹ޴� �����̵� Component �߻�Ŭ����
/// ������ : �赿�� - Start �Լ����� owner ������ ���ְ� ����  
///   -> �ʿ� �������µ� Start �Լ��� �� ���� ���� �� ���Ƽ� �ϴ� ����. abstract �ϸ� ��� ������Ʈ �߰��ؾ��ϴ� �ϴ� ���߽��ϴ�.
/// ������ : �ռ��� - owner (private -> protected), ����׷��� �߰�
///			-> FixedUpdate �߰�
/// </summary>
class Component : public IComponent
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Component)

public:
	// Serialize
	virtual void Serialize(nlohmann::json& json) abstract;
	virtual void Deserialize(const nlohmann::json& json) abstract;

public:
	Component();
	Component(const std::string& name);
	virtual ~Component();

	virtual void Start();
	virtual void Update(double dt);
	virtual void FixedUpdate(double FixedTime);
	virtual void Render(double dt);
	virtual void Finalize();
	virtual void DebugRender() {};

	// ���纻�� ������ ��ȯ.
	virtual std::shared_ptr<Component> Clone() const abstract;
	void CloneBaseComponentMember(std::shared_ptr<Component> baseComponent);

	int GetUID() const { return m_UID; }
	std::string GetName() const { return m_name; }
	void SetName(std::string name) { m_name = name; }
	std::weak_ptr<Entity> GetOwner() const { return m_owner; }
	void SetOwner(std::weak_ptr<Entity> entity) { m_owner = entity; }

	std::shared_ptr<Transform> GetTransform();

protected:
	std::string m_name;

	static unsigned int m_lastComponentID;
	int m_UID;

	std::weak_ptr<Entity> m_owner;
};

