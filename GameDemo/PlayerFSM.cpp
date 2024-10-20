#include "GameDemo_pch.h"
#include "PlayerFSM.h"
#include "InputManager.h"



PlayerFSM::PlayerFSM() : cheat(false)
{

}

PlayerFSM::~PlayerFSM()
{

}

void PlayerFSM::Start()
{
	m_playerStateList[Player::Idle] = std::make_shared<PlayerIdle>(shared_from_this());
	m_playerStateList[Player::Eat] = std::make_shared<PlayerEat>(shared_from_this());
	m_playerStateList[Player::Hide] = std::make_shared<PlayerHide>(shared_from_this());
	m_playerStateList[Player::Run] = std::make_shared<PlayeRun>(shared_from_this());
	m_playerStateList[Player::Throw] = std::make_shared<PlayerThrow>(shared_from_this());
	m_playerStateList[Player::Lifting] = std::make_shared<PlayerLifting>(shared_from_this());



	std::vector<bool> idleTransition = { true,true, true, true, true, true, true };
	m_transition.push_back(idleTransition);
	std::vector<bool> eatTransition = { true,true, false, true, true, false, false };
	m_transition.push_back(eatTransition);
	std::vector<bool> hideTransition = { true,false, true, true, true, false, false };
	m_transition.push_back(hideTransition);
	std::vector<bool> runTransition = { true,true, true, true, true, true, true };
	m_transition.push_back(runTransition);
	std::vector<bool> arrestTransition = { true,false, false, false, false, false, false };
	m_transition.push_back(arrestTransition);
	std::vector<bool> liftingTransition = { true,false, false, false, true, true, true };
	m_transition.push_back(liftingTransition);
	std::vector<bool> throwTrasition = { true,false, false, false, true, false, true };
	m_transition.push_back(throwTrasition);

	m_curState = nullptr;
	m_preState = Player::Idle;
	FSM::ChangeState(m_playerStateList[Player::Idle]);




}

void PlayerFSM::OnStateEnter()
{
	m_curState->OnStateEnter();

}

void PlayerFSM::OnStateExit()
{
	m_curState->OnStateExit();

}

void PlayerFSM::Update(double dt)
{
	if (InputManager::GetInstance()->IsKeyDown(0x34) || InputManager::GetInstance()->IsKeyPress(0x34))
	{
		cheat = !cheat;
	}


	static double time = 0;
	time += dt;
	double checkstate = 0.5;

	if (time > checkstate)
	{
		time -= checkstate;

		m_curState->Update(dt);
	}
}

void PlayerFSM::Render(double dt)
{

}

void PlayerFSM::Finalize()
{
	for (auto state : m_playerStateList)
	{
		state.second->Finalize();
		state.second.reset();
	}

	m_curState.reset();
	m_playerStateList.clear();


}

void PlayerFSM::ChangeState(Player state)
{
	if (cheat)
	{
		state = Player::Hide;
	}

	if (m_transition[static_cast<int>(m_preState)][static_cast<int>(state)])
	{
		m_currentState = state;


		switch (state)
		{
			case PlayerFSM::Player::Idle:
				m_preState = Player::Idle;
				FSM::ChangeState(m_playerStateList[Player::Idle]);
				break;
			case PlayerFSM::Player::Eat:
				m_preState = Player::Eat;
				FSM::ChangeState(m_playerStateList[Player::Eat]);
				break;
			case PlayerFSM::Player::Hide:
				m_preState = Player::Hide;
				FSM::ChangeState(m_playerStateList[Player::Hide]);
				break;
			case PlayerFSM::Player::Lifting:
				m_preState = Player::Lifting;
				FSM::ChangeState(m_playerStateList[Player::Lifting]);
				break;
			case PlayerFSM::Player::Run:
				m_preState = Player::Run;
				FSM::ChangeState(m_playerStateList[Player::Run]);
				break;
			case PlayerFSM::Player::Arrest:
				m_preState = Player::Arrest;
				m_currentState = Player::Arrest;
				break;
			case PlayerFSM::Player::Throw:
				m_preState = Player::Throw;
				m_currentState = Player::Throw;
				FSM::ChangeState(m_playerStateList[Player::Throw]);
				break;

			default:
				break;
		}
	}
}

std::shared_ptr<Component> PlayerFSM::Clone() const
{
	std::shared_ptr<PlayerFSM> clone = std::make_shared<PlayerFSM>();

	//clone->Start();

	return clone;
}
