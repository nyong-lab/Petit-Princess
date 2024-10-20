#pragma once
#include <Script.h>

class TextureBox;
class FoodTableScript;
class ScoreUI;

class PlayerMoveScript : public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(PlayerMoveScript)
		PROPERTY(m_moveSpeed)
		PROPERTY(m_hunger)
		PROPERTY(m_notEatenTime)
		PROPERTY(m_hugerDecreaseTime)
		PROPERTY(m_hungerSatisfactionState)
		PROPERTY(m_hungerNormalState)
		PROPERTY(m_satisfactionMoveSpeed)
		PROPERTY(m_normalMoveSpeed)
		PROPERTY(m_dissatisfactionMoveSpeed)
		PROPERTY(m_speedUpBuffTime)
		PROPERTY(m_blockadeActionTime)
		PROPERTY(m_decreaseHunger)
		PROPERTY(m_iSuccessedMiniGame)
		PROPERTY(m_isFoodIntake)
		PROPERTY(m_isPlayerHidden)
		PROPERTY(m_isBlockadeAction)
		PROPERTY(m_skillCount)
		PROPERTY(m_score)
		PROPERTY(m_deliciousScoreIncrease)
		PROPERTY(m_basicScoreIncrease)

		// Serialize
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerMoveScript, m_name, m_UID, m_moveSpeed, m_hunger
		, m_notEatenTime, m_notEatenTime, m_hungerSatisfactionState, m_hungerNormalState
		, m_satisfactionMoveSpeed, m_normalMoveSpeed, m_dissatisfactionMoveSpeed
		, m_speedUpBuffTime, m_blockadeActionTime, m_decreaseHunger, m_isPlayerHidden
		, m_isBlockadeAction, m_skillCount, m_score, m_basicScoreIncrease, m_deliciousScoreIncrease)
	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	~PlayerMoveScript() {};

	void Start() override;
	void Update(double dt) override;
	std::shared_ptr<Component> Clone() const override;

	// �ݹ�
	virtual void OnCollisionEnter(const Collision& collision);
	virtual void OnCollisionStay(const Collision& collision);
	virtual void OnCollisionExit(const Collision& collision);
	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	// ������ ������ (��Ŭ��)
	void ThrowItem(float power);

	// Ǫ�� ���̺� ��ũ��Ʈ ����
	float* GetHungerPointer() { return &m_hunger; }
	bool* GetSuccessedMiniGameBoolPointer() { return &m_iSuccessedMiniGame; }
	bool* GetPlayMiniGameBoolPointer() { return &m_isPlayMiniGame; }

	// �÷��̾��� ���� ���� ������ ��ȯ
	// ���� ������Ʈ�� �ش� bool ���� �����Ѵ�.
	bool* GetPlayerHiddenPointer() { return &m_isPlayerHidden; }

	// �÷��̾��� �ൿ ���� ���� ������ ��ȯ
	// ��� ������Ʈ�� �ش� bool ���� �����Ѵ�.
	bool* GetPlayerBlockadeActionPointer() { return &m_isBlockadeAction; }

	// ��� ī��Ʈ ������ ��ȯ
	// ��� ������Ʈ�� �ش� bool ���� �����Ѵ�.
	int* GetPlayerSkillCountPointer() { return &m_skillCount; }

	// ��� ī��Ʈ ��ȯ
	int GetSkillCount() const { return m_skillCount; }
	bool AbleSkill() const { return m_ableSkill; }

	// �̴ϰ��ӿ��� ���� �� �÷��̾� ����
	void BasicScoreIncrease();
	void DeliciousScoreIncrease();

public:
	std::shared_ptr<TextureBox> m_throwObject;
	std::shared_ptr<ScoreUI> m_scoreUI;

	// ĳ���� �̵��ӵ�
	float m_moveSpeed = 10.f;						

	// �ε巯�� ������ȯ
	float m_turnSpeed = 7.f;						// ������ȯ �ӵ�: ���� Ŭ���� ����
	Vector3 m_targetVector = { 0.f, 0.f, 0.f };		// ������ ����: ����Ű�� ���� �ٷ� ��ȯ�ȴ�
	Vector3 m_headVector = { 0.f, 0.f, -1.f };		// ĳ���Ͱ� �ٶ󺸴� ����: m_targetVector�� ���� �ε巴�� ����

	// ������
	Vector3 m_bulletforcevector;
	std::vector<std::shared_ptr<Entity>> m_techItemList;

	// Ǫ�� ���̺� ��ũ��Ʈ
	// �ݹ� enter, exit �� �����Ѵ�.
	std::shared_ptr<FoodTableScript> m_foodTableScript;


	// �ѤѤѤѤѤѤ� �÷��̾� ���� ���� �ѤѤѤѤѤѤѤ� 
	// ����� ��ġ
	float m_hunger = 0.35; /// 100���� 1�� ����

	// �̴ϰ��� ���� (�÷��̾� �ൿ ����)
	bool m_iSuccessedMiniGame = false;

	// ���� ���� (n�ʰ� �̵� �ӵ� ����)
	bool m_isFoodIntake = false;

	// �̴ϰ��� ���ΰ�? (�÷��̾� ������ ������� �̴ϰ��� �巡�� Ű�� �ߺ��̹Ƿ� ����ó��)
	bool m_isPlayMiniGame = false;

	// ���� �̼��� �ð�
	float m_notEatenTime = 5.f;
	
	// ��� ���� �ð�
	float m_hugerDecreaseTime = 1.f;

	// ���� ����
	float m_hungerSatisfactionState = 0.7f;

	// ���� ����
	float m_hungerNormalState = 0.5f;

	// ���� ���� �̵� �ӵ�
	float m_satisfactionMoveSpeed = 10.f;

	// ���� ���� �̵� �ӵ�
	float m_normalMoveSpeed = 8.5f;

	// ���� ���� �̵� �ӵ�
	float m_dissatisfactionMoveSpeed = 6.5f;

	// ���� ���� �� ���� �ð�
	float m_speedUpBuffTime = 3.f;

	// �ൿ ���� �ð�
	float m_blockadeActionTime = 1.5f;

	// N�� ���� �������� ����
	float m_decreaseHunger = 0.0175f; /// 0.5���� 0.005�� ����

	// �÷��̾ ���� �������� 
	bool m_isPlayerHidden = false;


	// ��� ������Ʈ�� ��� �� �÷��̾� �ൿ ���� bool ��
	bool m_isBlockadeAction = false;

	// ��� ī��Ʈ
	int m_skillCount = 0;
	//��ų ��Ÿ��
	double m_skillCoolTime = 3;
	//
	bool m_ableSkill = false;

	// �÷��̾� ����
	int m_score = 0;

	// �⺻ ���İ��� ����
	int m_basicScoreIncrease = 1000;

	// ���� ����
	int m_deliciousScoreIncrease = 2000;

	bool m_hungerSound = false;
	bool m_hungerSecondSound = false;

	float m_rotationLockTime = -1.f;
};