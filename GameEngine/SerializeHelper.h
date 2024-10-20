#pragma once


#define GENERATE_SERIALIZE(NAME, ...) \
NLOHMANN_DEFINE_TYPE_INTRUSIVE(Stat, m_name, m_UID, __VA_ARGS__)\
void Serialize(nlohmann::json& json) override { to_json(json, *this); }\
void Deserialize(const nlohmann::json& json) override\
{\
	if (!json.is_null())\
	{\
		from_json(json, *this);\
	}\
}\
