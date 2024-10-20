#pragma once

#include <Script.h>


/// <summary>
/// ��� ������Ʈ�� �� ��ũ��Ʈ
/// �÷��̾�� ��� ������Ʈ�� ��ȣ�ۿ� �Ͽ� �������� ȹ���� �� �ִ�.
/// �ۼ��� : �赿��
/// </summary>
class TechniquesScript :public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(TechniquesScript)
		PROPERTY(m_isPlayerGetItem)
		PROPERTY(m_isInteractiveTrigger)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TechniquesScript, m_UID, m_name, m_isPlayerGetItem, m_isInteractiveTrigger)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	TechniquesScript() {};
	virtual ~TechniquesScript() {};

	virtual void Update(double dt) override;

	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	virtual std::shared_ptr<Component> Clone() const override;

	bool GetInteractive() const { return m_isInteractiveTrigger; }

private:
	// �÷��̾� �ൿ���� ������ ����
	bool* m_isPlayerBlockadeAction = nullptr;

	// �÷��̾� ��� ī��Ʈ ������ ����
	int* m_playerSkillCount = nullptr;

	// ��ȣ�ۿ� UI Ʈ����
	bool m_isInteractiveTrigger = false;

	// �÷��̾ �������� ������
	bool m_isPlayerGetItem = false;
};

