#pragma once
#include <Script.h>

class GameManager;
class DirectionalLight;

/// <summary>
/// �ð��� ���� ���ϴ� �¾�
/// �ռ���
/// </summary>
class SunLight : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(SunLight)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SunLight, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); };
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); };

//�����ڿ� �Ҹ��� ---------------------------------------------------------------------------------------
public:
	SunLight();
	~SunLight();


// ������Ʈ �Լ� ----------------------------------------------------------------------------------------
	virtual void Start() override;
	virtual void Update(double dt) override;
	virtual std::shared_ptr<Component> Clone() const override;

// ��� �Լ� ----------------------------------------------------------------------------------------
	

// ��� ���� ----------------------------------------------------------------------------------------
private:
	double m_timer = 0.f;   // Ÿ�̸� -> GameManager���� Update���� �޾ƿ���
	double m_endTime = 0.f; // ��ü �ð� �帧 -> GameManager���� �޾ƿ���
	std::shared_ptr<GameManager> m_gameMgr;
	std::shared_ptr<DirectionalLight> m_dir;

	// ��ħ
	float m_morningTime = 0.f; // endTime�� �޾ƿͼ� ���ؼ� ����
	Vector4 m_morningAmb = Vector4(172,215,250,180);
	Vector3 m_morningColor = Vector3(219,228,240);
	float m_morningIntensity = 0.1f;

	// ��
	float m_noonTime = 0.f;
	Vector4 m_noonAmb = Vector4(255, 255, 255, 247);
	Vector3 m_noonColor = Vector3(255, 255, 255);
	float m_noonIntensity = 0.14f;

	// ����
	float m_evenStartTime = 0.f;
	float m_evenTime = 0.f;
	Vector4 m_evenAmb = Vector4(230, 230, 255, 235);
	Vector3 m_evenColor = Vector3(200, 41, 150);
	float m_evenIntensity = 0.07f;

	// ��
	float m_nightTime = 0.f;
	Vector4 m_nightAmb = Vector4(81, 78, 136, 235);
	Vector3 m_nightColor = Vector3(145, 143, 192);;
	float m_nightIntensity = 0.03f;

	// ��~
	Vector4 m_midNightAmb = Vector4(156, 199, 255, 120);

	Vector3 m_startDir = Vector3(-3.8f, -8.1f, 4.8f);
	Vector3 m_finalDir = Vector3(6.5f, -8.1f, 4.8f);
};