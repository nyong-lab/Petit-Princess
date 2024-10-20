#include "GameDemo_pch.h"
#include "FSM.h"

FSM::FSM()
{
}

FSM::~FSM()
{
	m_curState.reset();
}

void FSM::ChangeState(std::shared_ptr<State> state)
{
	//바꿀수 있는 상태인가? 우선 파악후 바꿔야지!


	//이전 상태와 지금 상태가 같은가?
	if (m_curState == state)
	{
		m_curState = state;
		//추격은 매번 목적지를 다시 셋팅
		if (dynamic_pointer_cast<GuardChasing>(state))
		{
			m_curState->OnStateEnter();
			return;
		}

		return;
	}

	//상태가 바뀌어야하니까 기존 상태에서 마지막에 해야할 함수를 호출
	if (state != nullptr)
	{
		if (m_curState != nullptr)
		{
			m_curState->OnStateExit();
		}
	}

	m_curState = state;
	//상태가 바뀌었으니 한번해줄 함수 호출
	m_curState->OnStateEnter();
}
