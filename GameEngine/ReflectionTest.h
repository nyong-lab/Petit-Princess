#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"

class Entity;

/// <summary>
/// 리플렉션 테스트를 위한 클래스
/// 작성자 : 김동일
/// </summary>
class ReflectionTest : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(ReflectionTest)
		PROPERTY(m_staticInt)
		PROPERTY(m_int)
		PROPERTY(m_unsigedInt)
		PROPERTY(m_float)
		PROPERTY(m_bool)
		PROPERTY(m_vector3)
		PROPERTY(m_quaternion)
		PROPERTY(m_string)
		PROPERTY(m_wstring)
		PROPERTY(m_vector)
		//PROPERTY(m_unorderedMap)

public:
	ReflectionTest();
	~ReflectionTest();

	virtual void Update() override;
	virtual void Finalize() override;
	virtual std::shared_ptr<Component> Clone() const override;

private:
	static int m_staticInt;
	int m_int = 0;
	unsigned int m_unsigedInt= 0;
	float m_float = 0.f;
	double m_double = 0.f;
	bool m_bool = true;

	Vector3 m_vector3 = {};
	Quaternion m_quaternion = {};

	std::string m_string = "";
	std::wstring m_wstring = L"";
	std::vector<std::shared_ptr<Entity>> m_vector;
	//std::unordered_map<Vector3, Quaternion> m_unorderedMap;
};

