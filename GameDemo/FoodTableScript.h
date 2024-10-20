#pragma once

#include "Script.h"
#include "MiniGameDefine.h"


class PlayerMoveScript;


/// <summary>
/// 2024.02.14
/// �̴ϰ����� ����,���� �� �����ϴ� ������ Ŭ����.
/// �� Ŭ�������� �̴ϰ��� ��ũ��Ʈ�� ����ִ� ��ƼƼ�� �����ϱ⵵ �ϰ� �����ϱ⵵ �Ѵ�.
/// 
/// �ۼ��� : �ڿ���,�赿��
/// </summary>
class FoodTableScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(FoodTableScript)
	PROPERTY(m_isClear)
	//PROPERTY(m_foodType)
	PROPERTY(m_isDeliciousFood)
	PROPERTY(m_reVisitEndTime)
	PROPERTY(m_merchantSenseTime)
	PROPERTY(m_isGuardArrive)
	
	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FoodTableScript, m_name, m_UID, m_miniGameID, m_isClear, m_foodType
	, m_isDeliciousFood, m_reVisitEndTime, m_merchantSenseTime, m_isGuardArrive)

	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	FoodTableScript();
	FoodTableScript(FoodType type);
	virtual ~FoodTableScript();

	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);
	virtual void Update(double dt) override;
	virtual void Render(double dt)	 override;
	void SetDelicious(bool truefalse) { m_isDeliciousFood = truefalse; }

	virtual std::shared_ptr<Component> Clone() const override;

public:
	bool GetInteractive() const { return m_isInteractiveTrigger; }
	FoodType GetMiniGameType() const { return m_foodType; }

	// FSM�� ����ִ� ���� ��ġ�������� �����ͷ� �����´�.
	// �� FSM�� Start �Լ� ȣ����־�� �Ѵ�.
	void SetPointerSense(double* pointerSense) { m_merchantSense = pointerSense; }
	void SetSense(double value) { *m_merchantSense = value; }

	// �����Ϳ��� ���� ���� ���͸� �� �� ���� Ÿ���� ������ �ش�.
	void SetFoodType(FoodType foodType);
	
	// �÷��̾ ���ĸԱ⸦ ������ �� ���� ��ȭ
	void SetAteFood(bool value) { m_isPlayerAteFood = value; }

public:
	// ���� �Ŵ������� ��� �̴ϰ��� Ŭ���� ������ üũ�ϱ� ���� ID
	static int m_miniGameLastID;
	int m_miniGameID = 0;

	bool m_isInteractiveTrigger = false;
	bool m_isClear = false;
	bool m_isMiniGamePlay = false;

	// Ÿ�Կ� �ش��ϴ� ���������� �̴ϰ��� ������
	std::vector<MiniGameInfo> m_miniGameInformations;

	// ���� ���������� �ش��ϴ� �̴ϰ��� ����
	std::shared_ptr<MiniGameInfo> m_currentMiniGameInfo;

	// �̴ϰ��� Ÿ��
	FoodType m_foodType = FoodType::None;

	// ���� �̴ϰ��� ������ üũ�ϴ� ��ƼƼ (QTE ��ũ��Ʈ�� ������ ����)
	std::shared_ptr<Entity> m_miniGameEntity;

	// ���� ��ġ ������ 
	double* m_merchantSense = nullptr;

	// �÷��̾� ��� ������
	float* m_playerHunger = nullptr;

	// �÷��̾��� �̴ϰ��� ���� ����
	bool* m_isPlayerSuccessedMiniGame = nullptr;

	// �÷��̾��� �̴ϰ��� �÷��� ����
	bool* m_isPlayerPlayMiniGame = nullptr;

	// ���� ����
	bool m_isDeliciousFood = false;

	// �÷��̾ ���� ���ĸԱ⸦ ���´���
	bool m_isPlayerAteFood = false;

	// ��湮 �ð�
	float m_reVisitEndTime = 5.f;
	
	// ��湮 ��� �ð�
	float m_reVisitElapsedTime = 0.f;

	// ���� - ���� ��ġ 100�� �ߵ� �ð�
	float m_merchantSenseTime = 3.f;

	// ����� �����ߴ���
	bool m_isGuardArrive = false;

	// UI���� ����ϱ� ���� (����) �Լ� ȣ���� ���� ��ũ��Ʈ ������
	std::weak_ptr<PlayerMoveScript> m_playerScript;

	float m_senseElapsedTime = 0.f;
};
