#pragma once
#include <Script.h>
#include <random>

class PointLight;

/// <summary>
/// ȶ��ó�� �Ϸ��̴� ����Ʈ����Ʈ
/// �ռ���
/// </summary>
class TorchLight : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(TorchLight)
		PROPERTY(m_isLightOn)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TorchLight, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//�����ڿ� �Ҹ��� ---------------------------------------------------------------------------------------
public:
	TorchLight();
	~TorchLight();

// ������Ʈ �Լ� ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual std::shared_ptr<Component> Clone() const override;

// ��� �Լ� ----------------------------------------------------------------------------------------
	

// ��� ���� ----------------------------------------------------------------------------------------
private:
	float m_timer = 0.f;
	std::shared_ptr<PointLight> m_point;

	float m_minLuma = 3.2f; // �ּ� ���
	float m_maxLuma = 3.8f; // �ִ� ���
	Vector3 m_color = Vector3(255, 143, 54);

	float m_onTime;			// ���� ������ �����ϴ� �ð�
	bool m_isLightOn = false;  // ó������ ���� �ִ� ��������
	bool m_isWorkingDone = false;

	//������ ����
	float m_changeTime;
	int m_isGoingLighter;

	// ����
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> changeDist;
	std::uniform_real_distribution<float> onDist;
};