#pragma once

#include <Script.h>


/// <summary>
/// ������ ��ġ�������� �����ϴ� Ŭ����
/// 
/// �ۼ��� : �赿��
/// </summary>
class MerchantScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MerchantScript)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MerchantScript, m_name, m_UID)

	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	MerchantScript() {};
	virtual ~MerchantScript() {};

	void Update(double dt) override;
	std::shared_ptr<Component> Clone() const override;

private:

};

