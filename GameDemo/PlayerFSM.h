#pragma once
#include "FSM.h"

class PlayerFSM :
    public FSM, public std::enable_shared_from_this<PlayerFSM>
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(PlayerFSM)
	/*PROPERTY(m_currentState)
	PROPERTY(m_preState)*/
	PROPERTY(cheat)
	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerFSM, m_name, m_UID)
	void Serialize(nlohmann::json& json) override { to_json(json, *this); }
	void Deserialize(const nlohmann::json& json) override { if (!json.is_null()) from_json(json, *this); }

public:
	enum class Player
	{
		Idle,
		Eat,
		Hide,
		Run,
		Arrest,
		Lifting,
		Throw,

		None,

	};

	PlayerFSM();
	~PlayerFSM();

	void Start();

	virtual void OnStateEnter() override;	//상태 진입시 한번만 발생하는 함수
	virtual void OnStateExit() override;	//상태 변화에 발생하는 함수

	virtual void Update(double dt) override;
	virtual void Render(double dt) override;
	virtual void Finalize() override;

	void ChangeState(PlayerFSM::Player state);

	virtual std::shared_ptr<Component> Clone() const override;


	PlayerFSM::Player GetState() const { return m_currentState; }


private:
	std::unordered_map<PlayerFSM::Player, std::shared_ptr<State>> m_playerStateList;

	Player m_currentState;
	Player m_preState;

	bool cheat;
};

