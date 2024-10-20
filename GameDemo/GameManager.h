#pragma once

#include "../GameEngine/Script.h"


/// <summary>
/// 2024.02.15
/// ������ �������ΰ͵��� ������ ��ũ��Ʈ
/// ����Ƽ�� �̷��� �۵��Ѵ�
/// �ۼ� : ���¿�
/// ���� : �ռ��� -> �ð� ���� �κ�
/// </summary>

class GameManager :
    public Script
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(GameManager)
	PROPERTY(m_gameOver)
		PROPERTY(m_isReturn)
		PROPERTY(m_isArrest)
		PROPERTY(m_allClear)
		PROPERTY(m_gameTime)
		PROPERTY(m_timer)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameManager, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	GameManager();
	~GameManager();

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;


	virtual void OnCollisionEnter(const Collision& collision) {};
	virtual void OnCollisionStay(const Collision& collision) {};
	virtual void OnCollisionExit(const Collision& collision) {};
	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnTriggerStay(Collider* collider);
	virtual void OnTriggerExit(Collider* collider);

	virtual std::shared_ptr<Component> Clone() const override;

	void SetPlayer(std::shared_ptr<Entity> player) { m_player = player; }
	double GetGameTime() { return m_gameTime; }
	double GetCurrentGameTime() { return  m_timer; }

private:
	void UpdateTimeInfo();

private:

	//���� ���� ����
	bool m_gameOver;
	//�÷��̾� ���� ����
	bool m_isReturn;
	// ��񺴿��� ü�� ����
	bool m_isArrest;
	//���� Ŭ���� ����
	std::vector<bool> m_isClear;

	//���� �湮 ��� ����
	bool m_allClear;

	//���� �ð�
	double m_gameTime;
	double m_timer;

	//���� ��ƼƼ�� ��Ƴ��߰���? ��ƼƼ�� ������ ������ ������ ���¸� �����ؾ��ϴϱ�
	//�÷��̾��� ���¿� ���� ������ ���� ��ȭ
	std::shared_ptr<Entity> m_player;
	
};

