#pragma once
#include "IComponent.h"

class Transform;
class CameraModule;

/// <summary>
/// IComponent의 인터페이스를 상속받는 구현이들어간 Component 추상클래스
/// 수정자 : 김동일 - Start 함수에서 owner 설정을 해주게 만듬  
///   -> 필요 없어졌는데 Start 함수는 쓸 수도 있을 것 같아서 일단 냅둠. abstract 하면 모든 컴포넌트 추가해야하니 일단 안했습니다.
/// 수정자 : 손서희 - owner (private -> protected), 디버그랜더 추가
///			-> FixedUpdate 추가
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

	// 복사본을 만들어내서 반환.
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

