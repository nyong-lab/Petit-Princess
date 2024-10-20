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
	//�ٲܼ� �ִ� �����ΰ�? �켱 �ľ��� �ٲ����!


	//���� ���¿� ���� ���°� ������?
	if (m_curState == state)
	{
		m_curState = state;
		//�߰��� �Ź� �������� �ٽ� ����
		if (dynamic_pointer_cast<GuardChasing>(state))
		{
			m_curState->OnStateEnter();
			return;
		}

		return;
	}

	//���°� �ٲ����ϴϱ� ���� ���¿��� �������� �ؾ��� �Լ��� ȣ��
	if (state != nullptr)
	{
		if (m_curState != nullptr)
		{
			m_curState->OnStateExit();
		}
	}

	m_curState = state;
	//���°� �ٲ������ �ѹ����� �Լ� ȣ��
	m_curState->OnStateEnter();
}
