#pragma once

#include <Script.h>


/// <summary>
/// 기술 오브젝트에 들어갈 스크립트
/// 플레이어는 기술 오브젝트와 상호작용 하여 아이템을 획득할 수 있다.
/// 작성자 : 김동일
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
	// 플레이어 행동봉쇄 포인터 조작
	bool* m_isPlayerBlockadeAction = nullptr;

	// 플레이어 기술 카운트 포인터 조작
	int* m_playerSkillCount = nullptr;

	// 상호작용 UI 트리거
	bool m_isInteractiveTrigger = false;

	// 플레이어가 아이템을 얻어갔는지
	bool m_isPlayerGetItem = false;
};

