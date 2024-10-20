#pragma once

#include "Component.h"
#include "EntityDefine.h"

class Entity;

/// <summary>
/// 리플렉션 테스트를 위한 클래스
/// 작성자 : 김동일
/// </summary>
class TestComponent : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(TestComponent)
		PROPERTY(m_staticInt, false)
		PROPERTY(m_int, false)
		PROPERTY(m_unsigedInt)
		PROPERTY(m_float)
		PROPERTY(m_double)
		PROPERTY(m_bool)

		PROPERTY(m_vector3)

		PROPERTY(m_string)
		PROPERTY(m_wstring)

		PROPERTY(m_vectorEntity)
		PROPERTY(m_unorderedMapEntity)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TestComponent,
		m_name, m_UID,
		m_staticInt, m_int, m_unsigedInt, m_float, m_double,
		m_bool, m_string, m_wstring)

	virtual void Serialize(nlohmann::json& json) override
	{
		to_json(json, *this);

		json["m_vector3.x"] = m_vector3.x;
		json["m_vector3.y"] = m_vector3.y;
		json["m_vector3.z"] = m_vector3.z;
	}
	virtual void Deserialize(const nlohmann::json& json) override
	{
		if (!json.is_null()) 
			from_json(json, *this); 

		if (json.contains("m_vector3"))
		{
			json.at("m_vector3.x").get_to(m_vector3.x);
			json.at("m_vector3.y").get_to(m_vector3.y);
			json.at("m_vector3.z").get_to(m_vector3.z);
		}
	}

public:
	TestComponent();
	~TestComponent();

	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	virtual std::shared_ptr<Component> Clone() const override;

private:
	static int m_staticInt;
	int m_int = 35;
	unsigned int m_unsigedInt = 9999;
	float m_float = 24.34f;
	double m_double = 24.356;
	bool m_bool = true;

	Vector3 m_vector3 = { 1.f, 5.f, 10.f };

	std::string m_string = "string test";
	std::wstring m_wstring = L"wstring test";
	std::vector<std::shared_ptr<Entity>> m_vectorEntity;
	std::unordered_map<std::string, std::shared_ptr<Entity>> m_unorderedMapEntity;
};

