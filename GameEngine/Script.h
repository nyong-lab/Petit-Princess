#pragma once
#include "Component.h"

struct Collision;
class Collider;

/// <summary>
/// 
/// 수정 손서희 -> 충돌 콜백 함수 추가
/// </summary>
class Script : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(Script)

    // Serialize
    virtual void Serialize(nlohmann::json& json) abstract;
    virtual void Deserialize(const nlohmann::json& json) abstract;

public:
    virtual ~Script();

    virtual void Update(double dt) abstract;
    virtual void Render(double dt) {};
    virtual void Finalize() {};

    virtual void OnCollisionEnter(const Collision& collision) {};
    virtual void OnCollisionStay(const Collision& collision) {};
    virtual void OnCollisionExit(const Collision& collision) {};
    virtual void OnTriggerEnter(Collider* collider) {};
    virtual void OnTriggerStay(Collider* collider) {};
    virtual void OnTriggerExit(Collider* collider) {};

	virtual std::shared_ptr<Component> Clone() const abstract;



private:
};

