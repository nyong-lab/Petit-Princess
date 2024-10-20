#include "GameDemo_pch.h"
#include "ShopKeeperFSM.h"

#include "GuardFSM.h"
#include "Transform.h"
#include "Entity.h"
#include "SphereCollider.h"


void ShopKeeperFSM::Start()
{
	m_shopkeeperStateList[ShopKeeper::idle] = std::make_shared<ShopKeeperIdle>(shared_from_this());
	m_shopkeeperStateList[ShopKeeper::doubt] = std::make_shared<ShopKeeperDoubt>(shared_from_this());
	m_shopkeeperStateList[ShopKeeper::calling] = std::make_shared<ShopKeeperCalling>(shared_from_this());

	m_curState = nullptr;
	ChangeState(ShopKeeper::idle);
	m_curEnumState = ShopKeeper::idle;

	dynamic_pointer_cast<ShopKeeperState>(m_shopkeeperStateList[ShopKeeper::idle])->SetSense(&m_sense);
	dynamic_pointer_cast<ShopKeeperState>(m_shopkeeperStateList[ShopKeeper::doubt])->SetSense(&m_sense);
	dynamic_pointer_cast<ShopKeeperState>(m_shopkeeperStateList[ShopKeeper::calling])->SetSense(&m_sense);
}

void ShopKeeperFSM::OnStateEnter()
{
	m_curState->OnStateEnter();

}

void ShopKeeperFSM::OnStateExit()
{
	m_curState->OnStateExit();

}

void ShopKeeperFSM::Update(double dt)
{
	m_curState->Update(dt);
}

void ShopKeeperFSM::Render(double dt)
{

}

void ShopKeeperFSM::Finalize()
{
	for (auto state : m_shopkeeperStateList)
	{
		state.second->Finalize();
		state.second.reset();
	}

	m_curState.reset();
	m_shopkeeperStateList.clear();

}

void ShopKeeperFSM::ChangeState(ShopKeeper state)
{
	m_curEnumState = state;

	switch (state)
	{
		case ShopKeeperFSM::ShopKeeper::idle:
			FSM::ChangeState(m_shopkeeperStateList[ShopKeeper::idle]);
			break;
		case ShopKeeperFSM::ShopKeeper::calling:
			FSM::ChangeState(m_shopkeeperStateList[ShopKeeper::calling]);
			break;
		case ShopKeeperFSM::ShopKeeper::doubt:
			FSM::ChangeState(m_shopkeeperStateList[ShopKeeper::doubt]);
			break;
		default:
			break;
	}
}

std::shared_ptr<Component> ShopKeeperFSM::Clone() const
{
	std::shared_ptr<ShopKeeperFSM> clone = std::make_shared<ShopKeeperFSM>();
	clone->Start();

	return clone;
}

double* ShopKeeperFSM::GetPointerSense()
{
	return &m_sense;
}

double ShopKeeperFSM::GetSense()
{
	return m_sense;
}
