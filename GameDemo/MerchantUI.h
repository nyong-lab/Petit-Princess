#pragma once
#include "Script.h"

class FoodTableScript;

/// <summary>
/// �ռ���
/// 
/// ������ �ڽ����� �־�� �Ѵ�.
/// </summary>
class MerchantUI : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(MerchantUI)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MerchantUI, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//�����ڿ� �Ҹ��� ---------------------------------------------------------------------------------------
public:
	MerchantUI();
	~MerchantUI();

// ������Ʈ �Լ� ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual void Render(double dt);
	virtual std::shared_ptr<Component> Clone() const override;

// ��� �Լ� ----------------------------------------------------------------------------------------


// ��� ���� ----------------------------------------------------------------------------------------
public:
	int m_state;	 // ������ �����? 
	// 0 : �ƹ��͵� ����� �ʰڴ� 
	// 1 : ���� �غ� 
	// 2 : �ٸ��� ����
	// 3 : ��湮 ��Ÿ��
	// 4 : ��ġ������
	// 5 : ���� 

	int m_foodtype;  // ���� ����
	// 0 : ���
	// 1 : ���(����)
	// 2 : �ٰ�Ʈ(��)
	// 3 : ��(�ػ깰)
	// 4 : ������(����Ʈ)

	float m_ratio;     // ��ġ������ or ��湮 ������

	Vector2 m_pos;
	const Vector2 m_size = Vector2(90, 90);

private:
	std::shared_ptr<FoodTableScript> m_foodTable;

};

