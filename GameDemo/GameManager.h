#pragma once

#include "../GameEngine/Script.h"


/// <summary>
/// 2024.02.15
/// 게임의 전반적인것들을 관리할 스크립트
/// 유니티도 이렇게 작동한다
/// 작성 : 유승운
/// 수정 : 손서희 -> 시간 관련 부분
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

	//게임 종료 여부
	bool m_gameOver;
	//플레이어 복귀 여부
	bool m_isReturn;
	// 경비병에게 체포 여부
	bool m_isArrest;
	//맛집 클리어 여부
	std::vector<bool> m_isClear;

	//맛집 방문 모두 성공
	bool m_allClear;

	//게임 시간
	double m_gameTime;
	double m_timer;

	//각종 엔티티를 담아놔야겠지? 엔티티의 정보를 가지고 게임의 상태를 관리해야하니까
	//플레이어의 상태에 따른 게임의 상태 변화
	std::shared_ptr<Entity> m_player;
	
};

